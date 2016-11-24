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
#include "opencctv/db/AnalyticInstanceGateway.hpp"
#include "opencctv/ServerController.hpp"

void exitHandler(int iSignum);

int main(int argc, char *argv[])
{

	// Registering signal SIGINT and signal handler
	signal(SIGINT, exitHandler); // for Ctrl + C keyboard interrupt
	signal(SIGTERM, exitHandler); // for Terminate signal

	// Initialize an instance of the server controller
	opencctv::ServerController *_pServerController = NULL;

	try
	{
		_pServerController = opencctv::ServerController::getInstance();
		opencctv::util::log::Loggers::getDefaultLogger()->info("Starting OpenCCTV server for receiving start/stop analytic instance message.");
	}
	catch (opencctv::Exception &e)
	{
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

				std::stringstream msg;
				msg << "Analytic server " << analyticServer.getIp() << ":" << analyticServer.getPort() << " connected.";
				opencctv::util::log::Loggers::getDefaultLogger()->info(msg.str());
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
	while (true)
	{
		try
		{
			// Read request from the OpenCCTV Web application
			string sRequestStr = _pServerController->readMessage();

			opencctv::util::log::Loggers::getDefaultLogger()->debug(sRequestStr);

			// Determine message type
			string sMessageType;
			unsigned int iAIId;
			int iASId;
			opencctv::util::xml::OpenCCTVServerMessage::extractMessageDetailWithData(sRequestStr, sMessageType, iAIId, iASId);

			// Process the message
			if (sMessageType.compare(opencctv::util::MSG_TYPE_START_ANALYTIC_REQ) == 0 && iAIId > 0 && iASId > 0)
			{
				// Check dupplicate running analytic instance
				analytic::AnalyticServer *pAnalyticServer = analytic::AnalyticServerManager::getInstance()->getAnalyticServer(iASId);

				if (pAnalyticServer)
				{
					if (pAnalyticServer->isAnalyticInstance(iAIId)) // check dupplicate analytic insance running
					{
						opencctv::util::log::Loggers::getDefaultLogger()->error("Cannot start analytic instance: " + boost::lexical_cast<std::string>(iAIId) + " server:  " + boost::lexical_cast<std::string>(iASId) + ". The analytic instance  is being run");
					}
					else
					{
						// Received message is ok and reponse to Web and let web
						opencctv::util::log::Loggers::getDefaultLogger()->debug("Strting analytic instance: " + boost::lexical_cast<std::string>(iAIId) + " server:  " + boost::lexical_cast<std::string>(iASId));
						_pServerController->replyMessage("ok");

						if (pAnalyticServer->startAnalyticInstanceAction(iAIId))
						{
							// update stats to database
							opencctv::db::AnalyticInstanceGateway *pAIGateway = new opencctv::db::AnalyticInstanceGateway();

							if (pAIGateway->updateStatus(iAIId, 2) > 0) // Status "2" = started
							{
								opencctv::util::log::Loggers::getDefaultLogger()->info("Finished starting the analytic instance: " + boost::lexical_cast<std::string>(iAIId));
							}
						}
						else
						{
							opencctv::util::log::Loggers::getDefaultLogger()->info("Failed to start the analytic instance: " + boost::lexical_cast<std::string>(iAIId));
						}
					}
				}
				else
				{
					opencctv::util::log::Loggers::getDefaultLogger()->error("Server: " + boost::lexical_cast<std::string>(iASId) + " is not running.");
				}
			}
			else if (sMessageType.compare(opencctv::util::MSG_TYPE_STOP_ANALYTIC_REQ) == 0  && iAIId > 0 && iASId > 0)
			{
				// Check dupplicate running analytic instance
				analytic::AnalyticServer *pAnalyticServer = analytic::AnalyticServerManager::getInstance()->getAnalyticServer(iASId);

				if (pAnalyticServer)
				{
					if (pAnalyticServer->isAnalyticInstance(iAIId)) // check dupplicate analytic insance running
					{
						// Received message is ok and reponse to Web and let web
						opencctv::util::log::Loggers::getDefaultLogger()->debug("Stopping analytic instance: " + boost::lexical_cast<std::string>(iAIId) + " server:  " + boost::lexical_cast<std::string>(iASId));
						_pServerController->replyMessage("ok");

						if (pAnalyticServer->stopAnalyticInstanceAction(iAIId))
						{
							// Remove analytic instace data in alaytic intance server
							// Do not forget to delete pointer

							// update stats to database
							opencctv::db::AnalyticInstanceGateway *pAIGateway = new opencctv::db::AnalyticInstanceGateway();

							if (pAIGateway->updateStatus(iAIId, 0) > 0) // Status "0" = stop
							{
								opencctv::util::log::Loggers::getDefaultLogger()->info("Finished stopping the analytic instance: " + boost::lexical_cast<std::string>(iAIId));
							}
						}
						else
						{
							opencctv::util::log::Loggers::getDefaultLogger()->info("Failed to stop the analytic instance: " + boost::lexical_cast<std::string>(iAIId));
						}

					}
					else
					{
						opencctv::util::log::Loggers::getDefaultLogger()->error("Cannot stop analytic instance: " + boost::lexical_cast<std::string>(iAIId) + " server:  " + boost::lexical_cast<std::string>(iASId) + ". The analytic instance is not being run");
					}
				}
				else
				{
					opencctv::util::log::Loggers::getDefaultLogger()->error("Server: " + boost::lexical_cast<std::string>(iASId) + " is not running.");
				}
			}
			else if (sMessageType.compare(opencctv::util::MSG_TYPE_INVALID) == 0)
			{
				throw opencctv::Exception("Invalid message type received.");
			}
			else
			{
				throw opencctv::Exception("Unknown message type received.");
			}

		}
		catch (opencctv::Exception &e)
		{
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

		if (pAnalyticServer->stopAllAnalyticInstanceAction())
		{
			opencctv::util::log::Loggers::getDefaultLogger()->info("Stop all analytic server.");
			// Do something when successful send kill all to analytic server
		}
		else
		{
			// When failif
		}
	}

	opencctv::util::log::Loggers::getDefaultLogger()->info("Terminate OpenCCTV server from exitHandler");

	exit(iSignum);
}
