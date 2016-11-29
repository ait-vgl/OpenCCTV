#include "AnalyticServer.hpp"

namespace analytic
{

AnalyticServer::AnalyticServer(const int &analyticServerId, const std::string &sAnalyticServerIp, const std::string &sAnalyticServerPort)
{
	try
	{
		_iServerId = analyticServerId;
		_sAnalyticServerIp = sAnalyticServerIp;
		_pSocket = opencctv::mq::MqUtil::connectToMq(sAnalyticServerIp, sAnalyticServerPort, ZMQ_REQ);
	}
	catch (std::runtime_error &e)
	{
		std::string sErrMsg = "Failed to create ZMQ Socket to connect to Analytic Starter. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}

bool AnalyticServer::startAllAnalyticInstanceAction()
{

	opencctv::db::AnalyticInstanceGateway *pAnalyticInstanceGateway = NULL;

	try
	{
		pAnalyticInstanceGateway = new opencctv::db::AnalyticInstanceGateway();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	std::vector<opencctv::dto::AnalyticInstance> vAnalyticInstances;


	try
	{
		// Get all analytic instance
		pAnalyticInstanceGateway->findAll(vAnalyticInstances);
		opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic Instances Streams loaded.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	// Starting Analytic Instances
	for (size_t j = 0; j < vAnalyticInstances.size(); ++j)
	{
		opencctv::dto::AnalyticInstance analyticInstance = vAnalyticInstances[j];

		if (!isAnalyticInstance(analyticInstance.getId())) // check dupplicate running, in case one analytic have multiple streams
		{
			bool bAIStarted = false;
			std::string sAnalyticQueueInAddress, sAnalyticQueueOutPort;

			try
			{
				// start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
				bAIStarted = startAnalyticInstance(analyticInstance.getId(), analyticInstance.getAnalyticDirLocation(), analyticInstance.getAnalyticFilename(), sAnalyticQueueInAddress, sAnalyticQueueOutPort);

			}
			catch (opencctv::Exception &e)
			{
				std::stringstream ssErrMsg;
				ssErrMsg << "Failed to start Analytic Instance ";
				ssErrMsg << analyticInstance.getId() << ". ";
				ssErrMsg << e.what();
				opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
			}

			if (bAIStarted)
			{
				// store analytic input queue address and output queue address in model

				AnalyticData *pAnalyticInstance = new AnalyticData();

				opencctv::ResultRouterThread *pResultRouter = new opencctv::ResultRouterThread(analyticInstance.getId(), _sAnalyticServerIp, sAnalyticQueueOutPort);
				boost::thread *pResultRouterThread = new boost::thread(*pResultRouter);

				if (pResultRouterThread->joinable())
				{
					pAnalyticInstance->setResultRouterThread(pResultRouterThread);
				}

				// Place to set up analytic id
				setAnalyticData(analyticInstance.getId(), pAnalyticInstance);

				std::stringstream ssMsg;
				ssMsg << "Analytic Instance " << analyticInstance.getId();
				ssMsg << " started.";
				opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
			}
			else
			{
				std::stringstream ssErrMsg;
				ssErrMsg << "Starting Analytic Instance " << analyticInstance.getId();
				ssErrMsg << " failed.";
				opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
			}
		}
	}
	return true;
}

bool AnalyticServer::startAnalyticInstanceAction(unsigned int iAnalyticInstanceId)
{

	//test::gateway::TestAnalyticInstanceStreamGateway analyticInstanceGateway;
	opencctv::db::AnalyticInstanceGateway *pAnalyticInstanceGateway = NULL;

	try
	{
		pAnalyticInstanceGateway = new opencctv::db::AnalyticInstanceGateway();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	std::vector<opencctv::dto::AnalyticInstance> vAnalyticInstances;

	try
	{
		// Get all analytic instance
		pAnalyticInstanceGateway->findAnalyticInstance(vAnalyticInstances, iAnalyticInstanceId);
		opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic Instances Streams loaded.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	// Starting Analytic Instances
	for (size_t j = 0; j < vAnalyticInstances.size(); ++j) 
	{
		opencctv::dto::AnalyticInstance analyticInstance = vAnalyticInstances[j];

		// if the Analytic Instance has not been started yet
		AnalyticData *pAD = getAnalyticData(analyticInstance.getId());

		if (!isAnalyticInstance(analyticInstance.getId()))
		{
			bool bAIStarted = false;
			std::string sAnalyticQueueInAddress, sAnalyticQueueOutPort;
			try
			{
				// start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
				bAIStarted = startAnalyticInstance(analyticInstance.getId(), analyticInstance.getAnalyticDirLocation(), analyticInstance.getAnalyticFilename(), sAnalyticQueueInAddress, sAnalyticQueueOutPort);

			}
			catch (opencctv::Exception &e)
			{
				std::stringstream ssErrMsg;
				ssErrMsg << "Failed to start Analytic Instance ";
				ssErrMsg << analyticInstance.getId() << ". ";
				ssErrMsg << e.what();
				opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
			}

			if (bAIStarted)
			{
				// store analytic input queue address and output queue address in model

				AnalyticData *pAnalyticInstance = new AnalyticData();

				opencctv::ResultRouterThread *pResultRouter = new opencctv::ResultRouterThread(analyticInstance.getId(), _sAnalyticServerIp, sAnalyticQueueOutPort);
				boost::thread *pResultRouterThread = new boost::thread(*pResultRouter);

				if (pResultRouterThread->joinable())
				{
					pAnalyticInstance->setResultRouterThread(pResultRouterThread);
				}
				// Place to set up analytic id
				setAnalyticData(analyticInstance.getId(), pAnalyticInstance);

				//pAD =  setAnalyticInstance(analyticInstance.getId(), pAnalyticInstance);

				std::stringstream ssMsg;
				ssMsg << "Analytic Instance " << analyticInstance.getId();
				ssMsg << " started.";
				opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
			}
			else
			{
				std::stringstream ssErrMsg;
				ssErrMsg << "Starting Analytic Instance " << analyticInstance.getId();
				ssErrMsg << " failed.";
				opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
			}
		}
	}

	return true;
}

bool AnalyticServer::startAnalyticInstance(unsigned int iAnalyticInstanceId, const std::string &sAnalyticPluginDirLocation, const std::string &sAnalyticPluginFilename, std::string &sAnalyticQueueInAddress, std::string &sAnalyticQueueOutAddress)
{
	bool bRet = false;

	if (_pSocket)
	{
		std::string sRequest, sReply;

		try
		{
			sRequest = xml::AnalyticMessage::getAnalyticStartRequest(iAnalyticInstanceId, sAnalyticPluginDirLocation, sAnalyticPluginFilename);
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to generate Analytic Start request. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		bool bRequestSent = false;

		try
		{
			bRequestSent = opencctv::mq::MqUtil::writeToSocket(_pSocket, sRequest);
		}
		catch (std::runtime_error &e)
		{
			std::string sErrMsg = "Failed to send Analytic Start request. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		if (bRequestSent)
		{
			try
			{
				opencctv::mq::MqUtil::readFromSocket(_pSocket, sReply);
			}
			catch (std::runtime_error &e)
			{
				std::string sErrMsg = "Failed to read Analytic Start reply. ";
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}

			if (!sReply.empty() && sReply.compare("Error") != 0)
			{
				try
				{
					analytic::xml::AnalyticMessage::extractAnalyticStartReplyData(sReply, sAnalyticQueueInAddress, sAnalyticQueueOutAddress);
					bRet = true;
				}
				catch (opencctv::Exception &e)
				{
					std::string sErrMsg = "Failed to extract data from Analytic Start reply. ";
					sErrMsg.append(e.what());
					throw opencctv::Exception(sErrMsg);
				}
			}
			else
			{
				throw opencctv::Exception("Error in Analytic Start reply.");
			}
		}
	}

	return bRet;
}

bool AnalyticServer::stopAnalyticInstanceAction(unsigned int iAnalyticInstanceId)
{
	bool bRet = false;

	if (_pSocket)
	{
		std::string sRequest, sReply;

		try
		{
			sRequest = xml::AnalyticMessage::getAnalyticStopRequest(iAnalyticInstanceId);
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to generate Analytic Start request. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		bool bRequestSent = false;

		try
		{
			bRequestSent = opencctv::mq::MqUtil::writeToSocket(_pSocket, sRequest);
		}
		catch (std::runtime_error &e)
		{
			std::string sErrMsg = "Failed to send Analytic Stop request. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		if (bRequestSent)
		{
			try
			{
				opencctv::mq::MqUtil::readFromSocket(_pSocket, sReply);
			}
			catch (std::runtime_error &e)
			{
				std::string sErrMsg = "Failed to read Analytic Stop reply. ";
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}

			if (!sReply.empty() && sReply.compare("Error") != 0)
			{
				try
				{
					analytic::xml::AnalyticMessage::parseStopAnalyticInstanceReply(sReply, bRet);
				}
				catch (opencctv::Exception &e)
				{
					std::string sErrMsg = "Failed to extract data from Analytic Start reply. ";
					sErrMsg.append(e.what());
					throw opencctv::Exception(sErrMsg);
				}
			}
			else
			{
				throw opencctv::Exception("Error in Analytic Start reply.");
			}
		}
	}

	if (bRet)
		return removeAnalyticData(iAnalyticInstanceId);
	else
		return false;
}

bool AnalyticServer::stopAllAnalyticInstanceAction()
{
	bool bDone = false;

	if (_pSocket)
	{
		std::string sMsg, sReply;

		try
		{
			sMsg = xml::AnalyticMessage::getKillAllAnalyticProcessesRequest();
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to generate xml message. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		bool bMsgSent = false;

		try
		{
			bMsgSent = opencctv::mq::MqUtil::writeToSocket(_pSocket, sMsg);
		}
		catch (std::runtime_error &e)
		{
			std::string sErrMsg = "Failed to send Kill All Analytic Instances Message. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		if (bMsgSent)
		{
			try
			{
				opencctv::mq::MqUtil::readFromSocket(_pSocket, sReply);
			}
			catch (std::runtime_error &e)
			{
				std::string sErrMsg = "Failed to read Kill all analytic processes reply. ";
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}

			xml::AnalyticMessage::parseKillAllAnalyticProcessesReply(sReply, bDone);
		}
	}

	return bDone;
}
/*
const std::map<unsigned int, AnalyticData *> &AnalyticServer::getAllAnalyticData()
{
	return _mAnalyticDatas;
}
*/

void AnalyticServer::setAnalyticData(unsigned int iAnalyticInstanceId, AnalyticData *&pAnalyticData)
{

	if (!isAnalyticInstance(iAnalyticInstanceId))
	{
		_mAnalyticDatas[iAnalyticInstanceId] = pAnalyticData; // bool for status, it may be full of analytic data class

		opencctv::util::log::Loggers::getDefaultLogger()->debug("AnalyticServer: Set up AnalyticData: " + boost::lexical_cast<std::string> (iAnalyticInstanceId));
	}
	else
	{
		opencctv::util::log::Loggers::getDefaultLogger()->debug("AnalyticServer: Skip set up AnalyticData: " + boost::lexical_cast<std::string> (iAnalyticInstanceId));
	}
}

AnalyticData *AnalyticServer::getAnalyticData(unsigned int iAnalyticInstanceId)
{

	if (isAnalyticInstance(iAnalyticInstanceId))
	{
		return _mAnalyticDatas[iAnalyticInstanceId];
	}
	else
		return NULL;
}

bool AnalyticServer::isAnalyticInstance(int iAnalyticInstanceId)
{
	if (_mAnalyticDatas.count(iAnalyticInstanceId) > 0 && _mAnalyticDatas[iAnalyticInstanceId])
		return true;
	else
		return false;
}

bool AnalyticServer::removeAnalyticData(unsigned int iAnalyticInstanceId)
{
	if (isAnalyticInstance(iAnalyticInstanceId))
	{
		// Delete from pointer
		if (_mAnalyticDatas[iAnalyticInstanceId])
		{
			delete _mAnalyticDatas[iAnalyticInstanceId];

			//Delete from map
			_mAnalyticDatas.erase(_mAnalyticDatas.find(iAnalyticInstanceId));
			return true;
		}
	}
	else
	{
		return false;
	}
	return true;
}

AnalyticServer::~AnalyticServer()
{
	if (_pSocket)
	{
		_pSocket->close();
		delete _pSocket;
	}

}

} /* namespace analytic */
