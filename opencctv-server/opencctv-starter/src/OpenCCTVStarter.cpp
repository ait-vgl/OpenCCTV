/*
 * main.cpp
 *
 *  Created on: Jan 1, 2015
 *      Author: anjana
 */

#include <iostream>
#include <string>
#include <csignal>
#include <boost/lexical_cast.hpp>
#include "opencctv/ServerController.hpp"
#include "opencctv/Exception.hpp"
#include "opencctv/util/log/Loggers.hpp"
#include "opencctv/util/TypeDefinitions.hpp"
using namespace std;

void exitHandler(int iSignum);

int main(int argc, char* argv[])
{
	// Registering signal SIGINT and signal handler
	signal(SIGINT, exitHandler);

	// Initialize an instance of the server controller
	opencctv::ServerController* _pServerController = NULL;
	try
	{
		_pServerController = opencctv::ServerController::getInstance();
		opencctv::util::log::Loggers::getDefaultLogger()->info("OpenCCTV Server Manager Started.");
	}
	catch(opencctv::Exception &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		return -1;
	}

	// Continue to read the messages from a client(web_app) and respond to the requests
	while(true)
	{
		try
		{
			// Read request from the OpenCCTV Web application
			string sRequestStr = _pServerController->readMessage();
			// Determine message type
			string sMessageType;
			opencctv::util::xml::OpenCCTVServerMessage::extractMessageDetails(sRequestStr,sMessageType);

			// Process the message
			if(sMessageType.compare(opencctv::util::MSG_TYPE_START_REQ) == 0)
			{
				_pServerController->execServerStart();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("OpenCCTVServer started.");

			} else if(sMessageType.compare(opencctv::util::MSG_TYPE_STOP_REQ) == 0)
			{
				_pServerController->execServerStop();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("OpenCCTVServer stopped.");

			} else if(sMessageType.compare(opencctv::util::MSG_TYPE_RESTART_REQ) == 0)
			{
				_pServerController->execServerRestart();

			}else if(sMessageType.compare(opencctv::util::MSG_TYPE_STATUS_REQ) == 0)
			{
				_pServerController->sendStatusReply();

			}else if(sMessageType.compare(opencctv::util::MSG_TYPE_INVALID) == 0)
			{
				throw opencctv::Exception("Invalid message type received.");

			} else
			{
				throw opencctv::Exception("Unknown message type received.");
			}
		}
		catch(opencctv::Exception &e)
		{
			_pServerController->sendErrorReply(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		}
	}
	return 0;
}

void exitHandler( int iSignum )
{
	opencctv::util::log::Loggers::getDefaultLogger()->info("Exiting OpenCCTV Server Starter.");
	exit(iSignum);
}
