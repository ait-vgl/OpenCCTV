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

// add new openCCTV server
#include "test/gateway/TestStreamGateway.hpp"
#include "test/gateway/TestAnalyticInstanceStreamGateway.hpp"
#include "opencctv/util/Config.hpp"
#include "opencctv/ApplicationModel.hpp"
#include "opencctv/Exception.hpp"
#include "analytic/AnalyticInstanceManager.hpp"
#include "opencctv/util/log/Loggers.hpp"
#include "opencctv/util/flow/SimpleFlowController.hpp"
//#include "opencctv/util/flow/BasicFlowController.hpp"
#include "opencctv/ImageMulticaster.hpp"
#include "opencctv/ConsumerThread.hpp"
#include "opencctv/ProducerThread.hpp"
#include "opencctv/ResultRouterThread.hpp"
/* boost includes (/user/local/include) */
/* boost libraries (/usr/local/lib) -lboost_system */
#include <boost/thread/thread.hpp> // -lboost_thread -pthread
#include <boost/lockfree/queue.hpp>
#include <boost/lexical_cast.hpp> // to cast values
#include <signal.h> // to handle terminate signal
#include "opencctv/db/StreamGateway.hpp"
#include "opencctv/db/AnalyticInstanceStreamGateway.hpp"
#include "opencctv/ThreadPool.h"

using namespace std;

void exitHandler(int iSignum);

// add new
void startAllAnalytic();
void kilAllAnalytic();
int startAanalyticById(unsigned int);

//boost::thread_group _producerThreadGroup;
//boost::thread_group _consumerThreadGroup;
//boost::thread_group _resultsRouterThreadGroup;

opencctv::util::Config* pConfig ;
opencctv::ApplicationModel* pModel ;

opencctv::ThreadPool* pThreadPool;

int main(int argc, char* argv[]) {

	// Registering signal SIGINT and signal handler
	signal(SIGINT, exitHandler); // for Ctrl + C keyboard interrupt
	signal(SIGTERM, exitHandler); // for Terminate signal

	// Initialize an instance of the server controller
	opencctv::ServerController* _pServerController = NULL;
	try {
		_pServerController = opencctv::ServerController::getInstance();
		opencctv::util::log::Loggers::getDefaultLogger()->info(
				"OpenCCTV Server Manager Started.");
	} catch (opencctv::Exception &e) {
		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		return -1;
	}

	// Continue to read the messages from a client(web_app) and respond to the requests
	while (true) {
		try {
			// Read request from the OpenCCTV Web application
			string sRequestStr = _pServerController->readMessage();

			opencctv::util::log::Loggers::getDefaultLogger()->debug(
					sRequestStr);

			// Determine message type
			string sMessageType;
			string sData;

			opencctv::util::xml::OpenCCTVServerMessage::extractMessageDetailWithData(
					sRequestStr, sMessageType, sData);

			// Process the message

			if (sMessageType.compare(
					opencctv::util::MSG_TYPE_START_ANALYTIC_REQ) == 0) {
				//_pServerController->execServerStart();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Start analytic process by id =>" + sData);
				//_pServerController->startAnalytic();
				//startAanalyticById(atoi(sData.c_str()));
				_pServerController->testReply();

			} else if (sMessageType.compare(
					opencctv::util::MSG_TYPE_STOP_ANALYTIC_REQ) == 0) {
				//_pServerController->execServerStop();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Stop analytic process by id =>" + sData);

				_pServerController->testReply();

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_START_REQ)
					== 0) {
				startAllAnalytic();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Start all analytics.");
				_pServerController->testReply();

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_STOP_REQ)
					== 0) {
				//_pServerController->execServerStop();

				//opencctv::util::log::Loggers::getDefaultLogger()->debug("Start to stop all analytic.");
				kilAllAnalytic();
				_pServerController->testReply();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Stop all analytic.");


			} else if (sMessageType.compare(
					opencctv::util::MSG_TYPE_RESTART_REQ) == 0) {
				//_pServerController->execServerRestart();
				opencctv::util::log::Loggers::getDefaultLogger()->debug("Restart server.");
				_pServerController->testReply();

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_STATUS_REQ)
					== 0) {
				//_pServerController->sendStatusReply();

				opencctv::util::log::Loggers::getDefaultLogger()->debug("Status server.");
				_pServerController->testReply();

			} else if (sMessageType.compare(opencctv::util::MSG_TYPE_INVALID)
					== 0) {
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

	std::map<unsigned int, analytic::AnalyticInstanceManager*> mAnalyticInstanceManagers = opencctv::ApplicationModel::getInstance()->getAnalyticInstanceManagers();
	std::map<unsigned int, analytic::AnalyticInstanceManager*>::iterator it;

	for (it = mAnalyticInstanceManagers.begin(); it != mAnalyticInstanceManagers.end(); ++it) {

		analytic::AnalyticInstanceManager* pAnalyticInstanceManager = it->second;

		//cout << "No. of map elements: "<< mAnalyticInstanceManagers.size() << endl;

	   // std::cout << it->first << " => " << it->second << '\n';

		if (pAnalyticInstanceManager->killAllAnalyticInstances()) {
			if (pAnalyticInstanceManager) {

				mAnalyticInstanceManagers.erase(it);
				delete pAnalyticInstanceManager;
				pAnalyticInstanceManager = NULL;

			}
		} else {
			// When fail
		}
	}

	//cout << "No. of map elements: "<< mAnalyticInstanceManagers.size() << endl;
	if (!mAnalyticInstanceManagers.empty()) {
		opencctv::util::log::Loggers::getDefaultLogger()->error(
				"Failed to reset all the Analytic Servers.");
	} else {
		opencctv::util::log::Loggers::getDefaultLogger()->info(
				"Reset all the Analytic Servers.");
	}


	// pThreadPool = opencctv::ThreadPool::getInstance();

	// pThreadPool->interruptAll();
	// pThreadPool->joinAll();

	cout << "from signint all without joinAll()" << endl;
		//pThreadPool->joinAll();

	exit(iSignum);

}

void kilAllAnalytic() {

	opencctv::util::log::Loggers::getDefaultLogger()->info("Kill all analytic process.");

	pThreadPool = opencctv::ThreadPool::getInstance();

	//pThreadPool->interruptAll();
	cout << "from kill all" << endl;
	pThreadPool->joinAll();




	std::map<unsigned int, analytic::AnalyticInstanceManager*> mAnalyticInstanceManagers = opencctv::ApplicationModel::getInstance()->getAnalyticInstanceManagers();
	std::map<unsigned int, analytic::AnalyticInstanceManager*>::iterator it;

	//cout << "killAllAnalytic" << endl;
	for (it = mAnalyticInstanceManagers.begin(); it != mAnalyticInstanceManagers.end(); ++it)
	{
		analytic::AnalyticInstanceManager* pAnalyticInstanceManager = it->second;
		if (pAnalyticInstanceManager->killAllAnalyticInstances()) {
				if (pAnalyticInstanceManager) {

					mAnalyticInstanceManagers.erase(it);
					delete pAnalyticInstanceManager;
					pAnalyticInstanceManager = NULL;

				}
				 else {
					// When fail
				}
		}
	}

	if (!mAnalyticInstanceManagers.empty()) {
		opencctv::util::log::Loggers::getDefaultLogger()->error(
				"Failed to reset all the Analytic Servers.");
	} else {
		opencctv::util::log::Loggers::getDefaultLogger()->info(
				"Reset all the Analytic Servers.");
	}

	//cout << "Size of Result" << _resultsRouterThreadGroup.size() << endl;
	//cout << "Size of Result" << _producerThreadGroup.size() << endl;
	//cout << "Size of Result" << _consumerThreadGroup.size() << endl;



}

void startAllAnalytic() {
	// TODO: Handle error to main loop

	// Registering signal handlers
//	signal(SIGTERM, terminateHandler); // for Terminate signal
	//signal(SIGINT, terminateHandler); // for Ctrl + C keyboard interrupt

	// Initializing variables
	//test::gateway::TestStreamGateway streamGateway;
	opencctv::db::StreamGateway* pStreamGateway = NULL;
	try {
		pStreamGateway = new opencctv::db::StreamGateway();
	} catch (opencctv::Exception &e) {
		std::string sErrMsg = "Failed to create StreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		//return -1;
	}

	//test::gateway::TestAnalyticInstanceStreamGateway analyticInstanceGateway;
	opencctv::db::AnalyticInstanceStreamGateway* pAnalyticInstanceGateway = NULL;
	try {
		pAnalyticInstanceGateway = new opencctv::db::AnalyticInstanceStreamGateway();
	} catch (opencctv::Exception &e) {
		std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		//return -1;
	}

	pConfig = opencctv::util::Config::getInstance();
	pModel = opencctv::ApplicationModel::getInstance();

	size_t internalQueueSize = boost::lexical_cast<size_t>(pConfig->get(opencctv::util::PROPERTY_INTERNAL_QUEUE_SIZE));
	size_t remoteQueueSize = boost::lexical_cast<size_t>(pConfig->get(opencctv::util::PROPERTY_REMOTE_QUEUE_SIZE));

	analytic::AnalyticInstanceManager* pAnalyticInstanceManager =new analytic::AnalyticInstanceManager(pConfig->get(opencctv::util::PROPERTY_ANALYTIC_SERVER_IP),
					pConfig->get(opencctv::util::PROPERTY_ANALYTIC_SERVER_PORT));

	pModel->getAnalyticInstanceManagers()[1]= pAnalyticInstanceManager; // [x] x is a analytic server ID (key)

	//opencctv::util::log::Loggers::getDefaultLogger()->info("AnalyticInstanceManager size:" + pModel->getAnalyticInstanceManagers().size());

	opencctv::util::log::Loggers::getDefaultLogger()->info("Initializing variables done.");

	std::vector<opencctv::dto::Stream> vStreams;
	try {
		pStreamGateway->findAll(vStreams);
		opencctv::util::log::Loggers::getDefaultLogger()->info(
				"Streams loaded.");
	} catch (opencctv::Exception &e) {
		std::string sErrMsg = "Failed to find all Streams. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		//return -1;
	}
	for (size_t i = 0; i < vStreams.size(); ++i) {
		opencctv::dto::Stream stream = vStreams[i];
		opencctv::ImageMulticaster* pMulticaster = new opencctv::ImageMulticaster(stream.getId());
		// If a analytic instance use same stream, it should not create new stream variable.
		std::vector<opencctv::dto::AnalyticInstanceStream> vAnalyticInstances;
		try {
			pAnalyticInstanceGateway->findAllForStream(stream.getId(), vAnalyticInstances);
			opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic Instances Streams loaded.");
		} catch (opencctv::Exception &e) {
			std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			//return -1;
		}
		// Starting Analytic Instances
		for (size_t j = 0; j < vAnalyticInstances.size(); ++j) {
			opencctv::dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];
			// if the Analytic Instance has not been started yet
			if (!pModel->containsImageInputQueueAddress(analyticInstance.getAnalyticInstanceId())) {
				bool bAIStarted = false;
				std::string sAnalyticQueueInAddress, sAnalyticQueueOutAddress;
				try {
					// start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
					bAIStarted =
							pAnalyticInstanceManager->startAnalyticInstance(
									analyticInstance.getAnalyticInstanceId(),
									analyticInstance.getAnalyticDirLocation(),
									analyticInstance.getAnalyticFilename(),
									sAnalyticQueueInAddress,
									sAnalyticQueueOutAddress);
				} catch (opencctv::Exception &e) {
					std::stringstream ssErrMsg;
					ssErrMsg << "Failed to start Analytic Instance ";
					ssErrMsg << analyticInstance.getAnalyticInstanceId()
							<< ". ";
					ssErrMsg << e.what();
					opencctv::util::log::Loggers::getDefaultLogger()->error(
							ssErrMsg.str());
				}
				if (bAIStarted) {
					// store analytic input queue address and output queue address in model

					//map<analytic instance id,?>
					pModel->getImageInputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] = sAnalyticQueueInAddress;

					//map<analytic instance id,?>
					pModel->getResultsOutputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] = sAnalyticQueueOutAddress;

					// if Analytic Instance started, construct Flow Controller for Input Analytic queue and store it on Application Model.
					opencctv::util::flow::FlowController* pFlowController = new opencctv::util::flow::SimpleFlowController(remoteQueueSize);
					//opencctv::util::flow::FlowController* pFlowController = new opencctv::util::flow::BasicFlowController();
					pModel->getFlowControllers()[analyticInstance.getAnalyticInstanceId()] = pFlowController;
					std::stringstream ssMsg;
					ssMsg << "Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssMsg << " started.";
					opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
				} else {
					std::stringstream ssErrMsg;
					ssErrMsg << "Starting Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssErrMsg << " failed.";
					opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}
			}
			// Adding Input Image Queue destinations to Image Multicaster
			try {

				pMulticaster->addDestination(analyticInstance);

			} catch (opencctv::Exception &e) {
				std::string sErrMsg = "Failed to add destination to Image Multicaster. ";
				sErrMsg.append(e.what());
				opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}
		}

		opencctv::util::log::Loggers::getDefaultLogger()->info("Starting Analytic Instances done.");

		// Creating threads and internal queues
		opencctv::ConcurrentQueue<opencctv::Image>* pQueue = NULL;
		opencctv::ConsumerThread* pConsumer = NULL;
		opencctv::ProducerThread* pProducer = NULL;

		if (pMulticaster->getNumberOfDestinations() > 0) {

			pQueue = new opencctv::ConcurrentQueue<opencctv::Image>(internalQueueSize);
			pModel->getInternalQueues()[stream.getId()] = pQueue;
			pConsumer = new opencctv::ConsumerThread(stream.getId(),pMulticaster);

			std::string sVmsPluginDirPath = pConfig->get(opencctv::util::PROPERTY_VMS_CONNECTOR_DIR);

			if (*sVmsPluginDirPath.rbegin() != '/') // check last char
					{
				sVmsPluginDirPath.append("/");
			}

			sVmsPluginDirPath.append(stream.getVmsConnectorFilename());

			// Loading VMS Connector Plugin
			opencctv::api::VmsConnector* pVmsConnector = NULL;
			opencctv::PluginLoader<opencctv::api::VmsConnector>* pVmsPluginLoader = NULL;

			// if VMS Plugin Loader has been constructed, use it.
			if (pModel->containsVmsPluginLoader(stream.getVmsTypeId())) {
				pVmsPluginLoader = pModel->getVmsPluginLoaders()[stream.getVmsTypeId()];
			}
			// if VMS Plugin Loader has not been constructed, construct it.
			else {
				try {
					pVmsPluginLoader = new opencctv::PluginLoader<opencctv::api::VmsConnector>();

					std::string sVmsPluginPath;
					opencctv::util::Util::findSharedLibOfPlugin(sVmsPluginDirPath, sVmsPluginPath);
					pVmsPluginLoader->loadPlugin(sVmsPluginPath);
					pModel->getVmsPluginLoaders()[stream.getVmsTypeId()] = pVmsPluginLoader;
				} catch (opencctv::Exception &e) {
					std::string sErrMsg =
							"Failed to load VMS Connector Plugin using VMS Connector Plugin Loader. ";
					sErrMsg.append(e.what());
					opencctv::util::log::Loggers::getDefaultLogger()->error(
							sErrMsg);
				}
			}

			// create VMS Connector using the VMS Plugin Loader
			if (pVmsPluginLoader) {
				try {
					pVmsConnector = pVmsPluginLoader->createPluginInstance();
					std::stringstream ssMsg;
					ssMsg << "VMS Connector plugin for Stream "
							<< stream.getId();
					ssMsg << " constructed.";
					opencctv::util::log::Loggers::getDefaultLogger()->info(
							ssMsg.str());
				} catch (opencctv::Exception &e) {
					std::string sErrMsg =
							"Failed to create VMS Connector plugin. ";
					sErrMsg.append(e.what());
					opencctv::util::log::Loggers::getDefaultLogger()->error(
							sErrMsg);
				}
			}

			// init VMS Connector
			opencctv::api::VmsConnectInfo connInfo = { stream.getVmsServerIp(),
					stream.getVmsServerPort(), stream.getVmsUsername(),
					stream.getVmsPassword(), stream.getCameraId(),
					stream.getWidth(), stream.getHeight(),
					stream.getKeepAspectRatio(), stream.getAllowUpSizing(),
					stream.getCompressionRate() };
			bool bVmsConnInitDone = false;
			try {
				//=======For Test Gateways====================================
				//bVmsConnInitDone = pVmsConnector->init(connInfo, stream.getVmsConnectorDirLocation());
				//============================================================

				bVmsConnInitDone = pVmsConnector->init(connInfo,sVmsPluginDirPath);

			} catch (std::exception &e) {
				std::string sErrMsg = "Failed to initialize VMS Connector plugin. ";
				sErrMsg.append(e.what());
				opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}

			if (pVmsConnector && bVmsConnInitDone) {
				std::stringstream ssMsg;
				ssMsg << "VMS Connector plugin " << stream.getId();
				ssMsg << " init done.";
				opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());

				// set pProducer
				pProducer = new opencctv::ProducerThread(stream.getId(),pVmsConnector);
			}
		} //end if of pMulticast


		pThreadPool = opencctv::ThreadPool::getInstance();

		if (pQueue && pConsumer && pProducer) {
			// Create and start Results Router threads
			for (size_t j = 0; j < vAnalyticInstances.size(); ++j) {
				opencctv::dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];

				if (pModel->containsResultsOutputQueueAddress(analyticInstance.getAnalyticInstanceId())) {

					opencctv::ResultRouterThread* pResultsRouter = new opencctv::ResultRouterThread(analyticInstance.getAnalyticInstanceId());

					boost::thread* pResultsRouterThread = new boost::thread(*pResultsRouter);

					//if (pResultsRouterThread->joinable()) {
					//	_resultsRouterThreadGroup.add_thread(pResultsRouterThread);
					//}

					pThreadPool->setResultRouterThreadManagers(1,pResultsRouterThread);
				}
			}

			// Start Consumer and Producer threads
			boost::thread* pConsumerThread = new boost::thread(*pConsumer);
			if (pConsumerThread->joinable()) {

				pThreadPool->setConsumerThreadManagers(1,pConsumerThread);

				boost::thread* pProducerThread = new boost::thread(*pProducer);
				if (pProducerThread->joinable()) {


					pThreadPool->setProducerThreadManagers(1,pProducerThread);

					//_consumerThreadGroup.add_thread(pConsumerThread);
					//_producerThreadGroup.add_thread(pProducerThread);
				}
			}
		}
	}
	//_resultsRouterThreadGroup.join_all();

}

/*int startAanalyticById(unsigned int analyticInstanceId) {
	// TODO: Handle error to main loop

	// Initializing variables
	//test::gateway::TestStreamGateway streamGateway;
	opencctv::db::StreamGateway* pStreamGateway = NULL;
	try {
		pStreamGateway = new opencctv::db::StreamGateway();
	} catch (opencctv::Exception &e) {
		std::string sErrMsg = "Failed to create StreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	//test::gateway::TestAnalyticInstanceStreamGateway analyticInstanceGateway;
	opencctv::db::AnalyticInstanceStreamGateway* pAnalyticInstanceGateway = NULL;
	try {
		pAnalyticInstanceGateway =
				new opencctv::db::AnalyticInstanceStreamGateway();
	} catch (opencctv::Exception &e) {
		std::string sErrMsg =
				"Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	opencctv::util::Config* pConfig = opencctv::util::Config::getInstance();
	opencctv::ApplicationModel* pModel =opencctv::ApplicationModel::getInstance();

	size_t internalQueueSize = boost::lexical_cast<size_t>(
			pConfig->get(opencctv::util::PROPERTY_INTERNAL_QUEUE_SIZE));
	size_t remoteQueueSize = boost::lexical_cast<size_t>(
			pConfig->get(opencctv::util::PROPERTY_REMOTE_QUEUE_SIZE));
	analytic::AnalyticInstanceManager* pAnalyticInstanceManager =
			new analytic::AnalyticInstanceManager(
					pConfig->get(opencctv::util::PROPERTY_ANALYTIC_SERVER_IP),
					pConfig->get(opencctv::util::PROPERTY_ANALYTIC_SERVER_PORT));

	pModel->getAnalyticInstanceManagers()[1] = pAnalyticInstanceManager;

	opencctv::util::log::Loggers::getDefaultLogger()->info("Initializing variables done.");

	std::vector<opencctv::dto::Stream> vStreams;
	try {
		pStreamGateway->findAllByAnalyticInstanceId(vStreams,analyticInstanceId);
		opencctv::util::log::Loggers::getDefaultLogger()->info("Streams loaded by Analytic instance id: " + analyticInstanceId);
	} catch (opencctv::Exception &e) {
		std::string sErrMsg = "Failed to find all Streams. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}
	for (size_t i = 0; i < vStreams.size(); ++i) {
		opencctv::dto::Stream stream = vStreams[i];
		opencctv::ImageMulticaster* pMulticaster = new opencctv::ImageMulticaster(stream.getId());
		// If a analytic instance use same stream, it should not create new stream variable.
		std::vector<opencctv::dto::AnalyticInstanceStream> vAnalyticInstances;
		try {
			pAnalyticInstanceGateway->findAllForStream(stream.getId(),
					vAnalyticInstances);
			opencctv::util::log::Loggers::getDefaultLogger()->info(
					"Analytic Instances Streams loaded.");
		} catch (opencctv::Exception &e) {
			std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			return -1;
		}


		// Starting Analytic Instances
		for (size_t j = 0; j < vAnalyticInstances.size(); ++j) {
			opencctv::dto::AnalyticInstanceStream analyticInstance =
					vAnalyticInstances[j];
			// if the Analytic Instance has not been started yet
			if (!pModel->containsImageInputQueueAddress(
					analyticInstance.getAnalyticInstanceId())) {
				bool bAIStarted = false;
				std::string sAnalyticQueueInAddress, sAnalyticQueueOutAddress;
				try {
					// start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
					bAIStarted =
							pAnalyticInstanceManager->startAnalyticInstance(
									analyticInstance.getAnalyticInstanceId(),
									analyticInstance.getAnalyticDirLocation(),
									analyticInstance.getAnalyticFilename(),
									sAnalyticQueueInAddress,
									sAnalyticQueueOutAddress);
				} catch (opencctv::Exception &e) {
					std::stringstream ssErrMsg;
					ssErrMsg << "Failed to start Analytic Instance ";
					ssErrMsg << analyticInstance.getAnalyticInstanceId()
							<< ". ";
					ssErrMsg << e.what();
					opencctv::util::log::Loggers::getDefaultLogger()->error(
							ssErrMsg.str());
				}
				if (bAIStarted) {
					// store analytic input queue address and output queue address in model
					pModel->getImageInputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] =
							sAnalyticQueueInAddress;
					pModel->getResultsOutputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] =
							sAnalyticQueueOutAddress;
					// if Analytic Instance started, construct Flow Controller for Input Analytic queue and store it on Application Model.
					opencctv::util::flow::FlowController* pFlowController =
							new opencctv::util::flow::SimpleFlowController(
									remoteQueueSize);
					//opencctv::util::flow::FlowController* pFlowController = new opencctv::util::flow::BasicFlowController();
					pModel->getFlowControllers()[analyticInstance.getAnalyticInstanceId()] =
							pFlowController;
					std::stringstream ssMsg;
					ssMsg << "Analytic Instance "
							<< analyticInstance.getAnalyticInstanceId();
					ssMsg << " started.";
					opencctv::util::log::Loggers::getDefaultLogger()->info(
							ssMsg.str());
				} else {
					std::stringstream ssErrMsg;
					ssErrMsg << "Starting Analytic Instance "
							<< analyticInstance.getAnalyticInstanceId();
					ssErrMsg << " failed.";
					opencctv::util::log::Loggers::getDefaultLogger()->error(
							ssErrMsg.str());
				}
			}
			// Adding Input Image Queue destinations to Image Multicaster
			try {
				pMulticaster->addDestination(analyticInstance);
			} catch (opencctv::Exception &e) {
				std::string sErrMsg =
						"Failed to add destination to Image Multicaster. ";
				sErrMsg.append(e.what());
				opencctv::util::log::Loggers::getDefaultLogger()->error(
						sErrMsg);
			}
		}

		opencctv::util::log::Loggers::getDefaultLogger()->info(
				"Starting Analytic Instances done.");

		// Creating threads and internal queues
		opencctv::ConcurrentQueue<opencctv::Image>* pQueue = NULL;
		opencctv::ConsumerThread* pConsumer = NULL;
		opencctv::ProducerThread* pProducer = NULL;
		if (pMulticaster->getNumberOfDestinations() > 0) {
			pQueue = new opencctv::ConcurrentQueue<opencctv::Image>(
					internalQueueSize);
			pModel->getInternalQueues()[stream.getId()] = pQueue;
			pConsumer = new opencctv::ConsumerThread(stream.getId(),
					pMulticaster);

			std::string sVmsPluginDirPath = pConfig->get(
					opencctv::util::PROPERTY_VMS_CONNECTOR_DIR);
			if (*sVmsPluginDirPath.rbegin() != '/') // check last char
					{
				sVmsPluginDirPath.append("/");
			}
			sVmsPluginDirPath.append(stream.getVmsConnectorFilename());

			// Loading VMS Connector Plugin
			opencctv::api::VmsConnector* pVmsConnector = NULL;
			opencctv::PluginLoader<opencctv::api::VmsConnector>* pVmsPluginLoader =
					NULL;
			// if VMS Plugin Loader has been constructed, use it.
			if (pModel->containsVmsPluginLoader(stream.getVmsTypeId())) {
				pVmsPluginLoader =
						pModel->getVmsPluginLoaders()[stream.getVmsTypeId()];
			}
			// if VMS Plugin Loader has not been constructed, construct it.
			else {
				try {
					pVmsPluginLoader = new opencctv::PluginLoader<
							opencctv::api::VmsConnector>();

					std::string sVmsPluginPath;
					opencctv::util::Util::findSharedLibOfPlugin(
							sVmsPluginDirPath, sVmsPluginPath);
					pVmsPluginLoader->loadPlugin(sVmsPluginPath);
					pModel->getVmsPluginLoaders()[stream.getVmsTypeId()] =
							pVmsPluginLoader;
				} catch (opencctv::Exception &e) {
					std::string sErrMsg =
							"Failed to load VMS Connector Plugin using VMS Connector Plugin Loader. ";
					sErrMsg.append(e.what());
					opencctv::util::log::Loggers::getDefaultLogger()->error(
							sErrMsg);
				}
			}
			// create VMS Connector using the VMS Plugin Loader
			if (pVmsPluginLoader) {
				try {
					pVmsConnector = pVmsPluginLoader->createPluginInstance();
					std::stringstream ssMsg;
					ssMsg << "VMS Connector plugin for Stream "
							<< stream.getId();
					ssMsg << " constructed.";
					opencctv::util::log::Loggers::getDefaultLogger()->info(
							ssMsg.str());
				} catch (opencctv::Exception &e) {
					std::string sErrMsg =
							"Failed to create VMS Connector plugin. ";
					sErrMsg.append(e.what());
					opencctv::util::log::Loggers::getDefaultLogger()->error(
							sErrMsg);
				}
			}
			// init VMS Connector
			opencctv::api::VmsConnectInfo connInfo = { stream.getVmsServerIp(),
					stream.getVmsServerPort(), stream.getVmsUsername(),
					stream.getVmsPassword(), stream.getCameraId(),
					stream.getWidth(), stream.getHeight(),
					stream.getKeepAspectRatio(), stream.getAllowUpSizing(),
					stream.getCompressionRate() };
			bool bVmsConnInitDone = false;
			try {
				//=======For Test Gateways====================================
				//bVmsConnInitDone = pVmsConnector->init(connInfo, stream.getVmsConnectorDirLocation());
				//============================================================
				bVmsConnInitDone = pVmsConnector->init(connInfo,
						sVmsPluginDirPath);
			} catch (std::exception &e) {
				std::string sErrMsg =
						"Failed to initialize VMS Connector plugin. ";
				sErrMsg.append(e.what());
				opencctv::util::log::Loggers::getDefaultLogger()->error(
						sErrMsg);
			}
			if (pVmsConnector && bVmsConnInitDone) {
				std::stringstream ssMsg;
				ssMsg << "VMS Connector plugin " << stream.getId();
				ssMsg << " init done.";
				opencctv::util::log::Loggers::getDefaultLogger()->info(
						ssMsg.str());
				pProducer = new opencctv::ProducerThread(stream.getId(),
						pVmsConnector);
			}
		}
		if (pQueue && pConsumer && pProducer) {
			// Create and start Results Router threads
			for (size_t j = 0; j < vAnalyticInstances.size(); ++j) {
				opencctv::dto::AnalyticInstanceStream analyticInstance =
						vAnalyticInstances[j];
				if (pModel->containsResultsOutputQueueAddress(
						analyticInstance.getAnalyticInstanceId())) {
					opencctv::ResultRouterThread* pResultsRouter =
							new opencctv::ResultRouterThread(
									analyticInstance.getAnalyticInstanceId());
					boost::thread* pResultsRouterThread = new boost::thread(
							*pResultsRouter);
					//if (pResultsRouterThread->joinable()) {

					//	_resultsRouterThreadGroup.add_thread(
						//		pResultsRouterThread);
					//}
				}
			}
			// Start Consumer and Producer threads
			boost::thread* pConsumerThread = new boost::thread(*pConsumer);
			if (pConsumerThread->joinable()) {
				boost::thread* pProducerThread = new boost::thread(*pProducer);
				//if (pProducerThread->joinable()) {
					//_consumerThreadGroup.add_thread(pConsumerThread);
					//_producerThreadGroup.add_thread(pProducerThread);
				//}
			}
		}
	}


	return 0;

}*/
