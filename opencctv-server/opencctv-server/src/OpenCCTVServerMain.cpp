/* boost includes (/user/local/include) */
/* boost libraries (/usr/local/lib) -lboost_system */

//#include <boost/thread/thread.hpp> // -lboost_thread -pthread
//#include <boost/lockfree/queue.hpp>
//#include <boost/lexical_cast.hpp> // to cast values
#include <signal.h> // to handle terminate signal
#include <iostream>
#include <string>
//#include <csignal>

#include "analytic/AnalyticServerManager.hpp"
//#include "analytic/AnalyticServer.hpp"

#include "opencctv/OpenCCTVServerManager.hpp"
//#include "opencctv/OpenCCTVServer.hpp"

#include "opencctv/dto/AnalyticServerDTO.hpp"
#include "opencctv/db/AnalyticServerGateway.hpp"
#include "opencctv/ServerController.hpp"

void exitHandler(int iSignum);

int main(int argc, char *argv[])
{

	// Registering signal SIGINT and signal handler
	signal(SIGINT, exitHandler); // for Ctrl + C keyboard interrupt
	signal(SIGTERM, exitHandler); // for Terminate signal

	// Initialize an instance of the server controller
	opencctv::ServerController* _pServerController = NULL;
	try {
		_pServerController = opencctv::ServerController::getInstance();
		opencctv::util::log::Loggers::getDefaultLogger()->info("Starting OpenCCTV server for receiving start stop analytic instance message.");
	} catch (opencctv::Exception &e) {
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		return -1;
	}
    
	// Set up OpenCCTV Server
	/*int iOpenCCTVServerId = 1;
	opencctv::OpenCCTVServer *pOS = NULL;

	try
	{
		pOS = new opencctv::OpenCCTVServer(iOpenCCTVServerId);

		if (pOS)
		{
			opencctv::OpenCCTVServerManager::getInstance()->setOpenCCTVServer(iOpenCCTVServerId, pOS);
			opencctv::util::log::Loggers::getDefaultLogger()->info("OpenCCTV server manager and OpenCCTV Server variable loaded.");
		}
	}
	catch (opencctv::Exception &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		return -1;
	} */

	// set up Analytic Server
	std::vector<opencctv::dto::AnalyticServerDTO> vAnalyticServers;

	try
	{
		opencctv::db::AnalyticServerGateway *analyticServerGateway = new opencctv::db::AnalyticServerGateway();
		analyticServerGateway->findAll(vAnalyticServers);

		for (int i = 0 ; i < vAnalyticServers.size(); i++)
		{
			opencctv::dto::AnalyticServerDTO analyticServer = vAnalyticServers[i];
			analytic::AnalyticServer *pAnalyticServer = NULL;

			if (analyticServer.getIp() != "" && analyticServer.getPort() > 0)
			{
				pAnalyticServer = new analytic::AnalyticServer(analyticServer.getAnalyticServerId(), analyticServer.getIp() , boost::lexical_cast<std::string>(analyticServer.getPort()));
			}
			else
			{
				opencctv::util::log::Loggers::getDefaultLogger()->error("Analytic server IP cannot be empty or its port should greater than 0");
				return -1;
			}

			if (pAnalyticServer)
			{
				analytic::AnalyticServerManager::getInstance()->setAnalyticServer(analyticServer.getAnalyticServerId(), pAnalyticServer);
				//pAS = pAnalyticServer;
				opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic server manager and Analytic Server vriable loaded.");
			}

		}

	}
	catch (opencctv::Exception &e)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		return -1;
	}


	// Start all analytic servers
	std::map<unsigned int, analytic::AnalyticServer *> mAnalyticServers = analytic::AnalyticServerManager::getInstance()->getAllAnalyticServer();
	std::map<unsigned int, analytic::AnalyticServer *>::iterator it;

	for (it = mAnalyticServers.begin(); it != mAnalyticServers.end(); ++it)
	{
		analytic::AnalyticServer *pAnalyticServer = it->second;

		if (pAnalyticServer->startAllAnalyticInstanceAction())
		{
			opencctv::util::log::Loggers::getDefaultLogger()->info("Start all analytic in analytic server " + boost::lexical_cast<std::string>(it->first));
			// Do something when successful send kill all to analytic server
		}
		else
		{
			// When fail
		}
	}

	// Continue to read the messages from a client(web_app) and respond to the requests
	while (true) {
		try {
			// Read request from the OpenCCTV Web application
			string sRequestStr = _pServerController->readMessage();

			opencctv::util::log::Loggers::getDefaultLogger()->debug(sRequestStr);

			// Determine message type
			string sMessageType;
			string sData;

			opencctv::util::xml::OpenCCTVServerMessage::extractMessageDetailWithData(sRequestStr, sMessageType, sData);

			// Process the message

			if (sMessageType.compare(opencctv::util::MSG_TYPE_START_ANALYTIC_REQ) == 0) {
				//_pServerController->execServerStart();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Start analytic process by id =>" + sData);
				//_pServerController->startAnalytic();
				//	startAanalyticById(1, atoi(sData.c_str()));
				_pServerController->testReply();

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_STOP_ANALYTIC_REQ) == 0) {
				//_pServerController->execServerStop();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Stop analytic process by id =>" + sData);

				//stopAanalyticById(atoi(sData.c_str()));

				_pServerController->testReply();

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_INVALID) == 0) {
				throw opencctv::Exception("Invalid message type received.");

			} else {
				throw opencctv::Exception("Unknown message type received.");
			}
		} catch (opencctv::Exception &e) {
			_pServerController->sendErrorReply(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		}
	}
    
    opencctv::util::log::Loggers::getDefaultLogger()->info("OpenCCTV server finish working, bye !!!.");
	return 0;
}

void exitHandler(int iSignum)
{

	opencctv::util::log::Loggers::getDefaultLogger()->info("Exiting OpenCCTV Server Manager.");

	// stop all analytic in all analytic servers
	std::map<unsigned int, analytic::AnalyticServer *> mAnalyticServers = analytic::AnalyticServerManager::getInstance()->getAllAnalyticServer();
	std::map<unsigned int, analytic::AnalyticServer *>::iterator it;

	for (it = mAnalyticServers.begin(); it != mAnalyticServers.end(); ++it)
	{

		analytic::AnalyticServer *pAnalyticServer = it->second;

		if (pAnalyticServer->killAllAnalyticInstances())
		{
			opencctv::util::log::Loggers::getDefaultLogger()->info("Stop all analytic server.");
			// Do something when successful send kill all to analytic server
		}
		else
		{
			// When failif
		}
	}

	cout << "from exitHandler  without stopAll()" << endl;

	exit(iSignum);
}
