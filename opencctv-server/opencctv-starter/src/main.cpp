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

#include "analytic/AnalyticServerManager.hpp"
#include "analytic/AnalyticServer.hpp"

#include "opencctv/OpenCCTVServerManager.hpp"
#include "opencctv/OpenCCTVServer.hpp"
#include "opencctv/controller/MainController.hpp"

using namespace opencctv;
using namespace analytic;

void exitHandler(int iSignum);

int setUpServer();

int iAnalyticServerId = 1;
AnalyticServer* pAS = NULL;

int iOpenCCTVServerId = 1;
OpenCCTVServer* pOS = NULL;

int main(int argc, char* argv[]) {

	// Registering signal SIGINT and signal handler
	signal(SIGINT, exitHandler); // for Ctrl + C keyboard interrupt
	signal(SIGTERM, exitHandler); // for Terminate signal

	// Initialize an instance of the server controller
	opencctv::ServerController* _pServerController = NULL;
	try {
		_pServerController = opencctv::ServerController::getInstance();
		opencctv::util::log::Loggers::getDefaultLogger()->info("Starting OpenCCTV server.");
	} catch (opencctv::Exception &e) {
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		return -1;
	}

	setUpServer();

	controller::MainController* mainCon = NULL;
	if (pAS and pOS) {
		mainCon = new controller::MainController(pAS, pOS);
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

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_START_REQ) == 0) {
				if (mainCon) {
					mainCon->startAllAnalytic();

					opencctv::util::log::Loggers::getDefaultLogger()->debug(
							std::string("Start all analytics.") + "OpenCCTVServer: " + boost::lexical_cast<std::string>(mainCon->getOS()->getServerId()) + " AnalyticServer: "
									+ boost::lexical_cast<std::string>(mainCon->getAS()->getServerId())); // put about opencctv and analytic server id s
				}else{
					opencctv::util::log::Loggers::getDefaultLogger()->error("Cannot start all analytics");
				}
				_pServerController->testReply();

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_STOP_REQ) == 0) {
				//_pServerController->execServerStop();

				//opencctv::util::log::Loggers::getDefaultLogger()->debug("Start to stop all analytic.");
				//kilAllAnalytic(); // 1 is analytic server id
				_pServerController->testReply();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Stop all analytic.");

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_RESTART_REQ) == 0) {
				//_pServerController->execServerRestart();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Restart server.");
				_pServerController->testReply();

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_STATUS_REQ) == 0) {
				//_pServerController->sendStatusReply();

				opencctv::util::log::Loggers::getDefaultLogger()->debug("Status server.");
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
	return 0;
}

void exitHandler(int iSignum) {

	opencctv::util::log::Loggers::getDefaultLogger()->info("Exiting OpenCCTV Server Manager.");

	// stop all
	std::map<unsigned int, AnalyticServer*> mAnalyticServers = AnalyticServerManager::getInstance()->getAllAnalyticServer();
	std::map<unsigned int, AnalyticServer*>::iterator it;

	for (it = mAnalyticServers.begin(); it != mAnalyticServers.end(); ++it) {

		AnalyticServer* pAnalyticServer = it->second;

		if (pAnalyticServer->killAllAnalyticInstances()) {
			opencctv::util::log::Loggers::getDefaultLogger()->info("Stop all analytic server.");
			// Do something when successful send kill all to analytic server
		} else {
			// When fail
		}
	}
	cout << "from exitHandler  without stopAll()" << endl;

	exit(iSignum);
}

int setUpServer() {

	// set up Analytic Server
	if (!pAS) {
		try {
			util::Config* pConfig = util::Config::getInstance();
			AnalyticServer* pAnalyticServer = new AnalyticServer(iAnalyticServerId, pConfig->get(util::PROPERTY_ANALYTIC_SERVER_IP),
					pConfig->get(util::PROPERTY_ANALYTIC_SERVER_PORT));

			if (pAnalyticServer) {
				AnalyticServerManager::getInstance()->setAnalyticServer(iAnalyticServerId, pAnalyticServer);
				pAS = pAnalyticServer;
				opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic server manager and Analytic Server Started.");
			}

		} catch (opencctv::Exception &e) {
			opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
			return -1;
		}
	}

	// Set up OpenCCTV Server
	if (!pOS) {
		try {
			pOS = new OpenCCTVServer(iOpenCCTVServerId);

			if (pOS) {
				OpenCCTVServerManager::getInstance()->setOpenCCTVServer(iOpenCCTVServerId, pOS);
				opencctv::util::log::Loggers::getDefaultLogger()->info("OpenCCTV server manager and OpenCCTV Server Started.");
			}
		} catch (opencctv::Exception &e) {
			opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
			return -1;
		}
	}

	return 0;
}

