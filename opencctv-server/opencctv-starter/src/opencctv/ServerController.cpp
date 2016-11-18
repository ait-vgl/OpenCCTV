/*
 * ServerController.cpp
 *
 *  Created on: Jun 1, 2015
 *      Author: anjana
 */

#include "ServerController.hpp"

namespace opencctv {

ServerController* ServerController::_pServerController = NULL;
string ServerController::_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
unsigned int ServerController::_iOpenCCTVServerPort = 0;
zmq::socket_t* ServerController::_tcpMq = NULL;
string ServerController::_sServerPath = "";
Process* ServerController::_pProcess = NULL;

ServerController::ServerController()
{
}

ServerController* ServerController::getInstance()
{
   if (!_pServerController) // Only allow one instance of class to be generated.
   {
	   _pServerController = new ServerController();

	   //Try to read and set the configuration details
	   opencctv::util::Config* pConfig = opencctv::util::Config::getInstance();

		string sStartingPort = pConfig->get(opencctv::util::PROPERTY_OPENCCTV_SERVER_PORT);
		if(sStartingPort.empty())
		{
			throw opencctv::Exception("Failed to retrieve OpenCCTV Server Starter port from the configuration file.");
		}
		_iOpenCCTVServerPort = boost::lexical_cast<unsigned int>(sStartingPort);

		_sServerPath = pConfig->get(opencctv::util::PROPERTY_OPENCCTV_SERVER_DIR);
		if(_sServerPath.empty())
	   	{
	   		throw opencctv::Exception("Failed to retrieve OpenCCTV Server installation directory from the configuration file.");
	   	}

		//Initialize an object to control child processes
		_pProcess = new Process();

	    //Initialize the REQ-REP instance of ZeroMQ
	   	try
	   	{
	   		_tcpMq = opencctv::mq::MqUtil::createNewMq(sStartingPort, ZMQ_REP);
	   	}
	   	catch(std::runtime_error &e)
	   	{
	   		throw opencctv::Exception("Failed to create OpenCCTV Server's request-reply MQ.");
	   	}
   }

	return _pServerController;
}

string ServerController::readMessage()
{
	string message = "";
	try
	{
		opencctv::mq::MqUtil::readFromSocket(_tcpMq, message);
	}
	catch(std::runtime_error &e)
	{
		std::string sErrMsg = "Failed to read request from the request-reply MQ. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}

	return message;
}

void ServerController::execServerStart()
{

	if(_sServerStatus.compare(opencctv::util::SVR_STATUS_STOPPED) == 0 || _sServerStatus.compare(opencctv::util::SVR_STATUS_UNKNOWN) == 0 )
	{
		vector<string> vServerParams;

		try
		{
			_pProcess->start(_sServerPath, vServerParams);

			int iProcessStatus;
			string sReplyMessage;
			if(_pProcess->readStatus(iProcessStatus))
			{
				if(iProcessStatus == 0)
				{
					_sServerStatus = opencctv::util::SVR_STATUS_RUNNING;
					string sContent = "OpenCCTV Server Started";
					opencctv::util::xml::OpenCCTVServerMessage::createStartMessageReply(sContent, _sServerStatus, _pProcess->getPid(), sReplyMessage);
					opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);

				}else if (iProcessStatus > 0)
				{
					_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
					throw opencctv::Exception("Failed to start the OpenCCTV Server : It is in the exit state");
				}
				else
				{
					_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
					throw opencctv::Exception("Failed to start the OpenCCTV Server : Cannot find OpenCCTV Server Process");
				}
			}
			else
			{
				throw opencctv::Exception("Failed to start the OpenCCTV Server : Cannot find the server process");
			}
		}
		catch(opencctv::Exception &e)
		{
			throw e;
		}
		catch(std::runtime_error &e)
		{
			opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		}
	}
	else
	{
		throw opencctv::Exception("The OpenCCTV Server is already running");
	}
}

void ServerController::execServerStop()
{
	_pProcess->stop();
	string sReplyMessage;
	string sContent = "OpenCCTV Server Stopped";
	_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createStopMessageReply(sContent, _sServerStatus, _pProcess->getPid(), sReplyMessage);
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		throw e;
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());;
	}

}

void ServerController::execServerRestart()
{
	_pProcess->stop();
	execServerStart();
}

void ServerController::sendStatusReply()
{
	int iProcessStatus;
	string sReplyMessage;
	string sContent;
	if(_pProcess->readStatus(iProcessStatus))
	{
		if(iProcessStatus == 0)
		{
			_sServerStatus = opencctv::util::SVR_STATUS_RUNNING;
			sContent = "OpenCCTV Server Started";

		}else
		{
			_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
			sContent = "OpenCCTV Server has Stopped";
		}
	}else
	{
		_sServerStatus = opencctv::util::SVR_STATUS_STOPPED;
		sContent = "OpenCCTV Server has Stopped";
	}

	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createStatusReply(sContent, _sServerStatus, _pProcess->getPid(), sReplyMessage);
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		throw e;
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
	}
}

void ServerController::sendErrorReply(const string& sMessageContent)
{
	string sReplyMessage;

	try
	{
		opencctv::util::xml::OpenCCTVServerMessage::createInvalidMessageReply(sMessageContent, _sServerStatus, _pProcess->getPid(), sReplyMessage);
	}
	catch(opencctv::Exception &e)
	{
		ostringstream ossReply;
		ossReply << "<?xml version=\"1.0\" encoding=\"utf-8\"?> <opencctvmsg><type>Error</type><content>";
		ossReply << e.what();
		ossReply << "</content><serverstatus>";
		ossReply << _sServerStatus;
		ossReply << "</serverstatus><serverpid>";
		ossReply << _pProcess->getPid();
		ossReply << "</serverpid></opencctvmsg>";

		sReplyMessage = ossReply.str();
	}

	try
	{
		opencctv::mq::MqUtil::writeToSocket(_tcpMq, sReplyMessage);
	}
	catch(std::runtime_error &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
	}
}

ServerController::~ServerController()
{
	delete _tcpMq; _tcpMq = NULL;
	delete _pProcess; _pProcess = NULL;
}

} /* namespace opencctv */
