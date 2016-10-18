/*
 * MainController.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: nhutumdai
 */

#include "MainController.hpp"


namespace opencctv {
namespace controller {

MainController::MainController(AnalyticServer* pAS, OpenCCTVServer* pOS) {
	this->_pAS = pAS;
	this->_pOS = pOS;
}

int MainController::startAllAnalytic() {

	// TODO: Handle error to main loop

	// check OpenCCTVServer && AnalyticSever

	if (!_pOS) {
		opencctv::util::log::Loggers::getDefaultLogger()->error("MainController: Cannot start all analytic, _pOS is empty");
		return -1;
	}

	if (!_pAS) {
		opencctv::util::log::Loggers::getDefaultLogger()->error("MainController: Cannot start all analytic, _pAS is empty");
		return -1;
	}

	util::Config* pConfig;
	//ApplicationModel* pModel;

	db::StreamGateway* pStreamGateway = NULL;

	try {
		pStreamGateway = new db::StreamGateway();
	} catch (Exception &e) {
		std::string sErrMsg = "Failed to create StreamGateway -  ";
		sErrMsg.append(e.what());
		util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		//return -1;
	}

	//test::gateway::TestAnalyticInstanceStreamGateway analyticInstanceGateway;
	db::AnalyticInstanceStreamGateway* pAnalyticInstanceGateway = NULL;
	try {
		pAnalyticInstanceGateway = new db::AnalyticInstanceStreamGateway();
	} catch (Exception &e) {
		std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		//return -1;
	}

	pConfig = util::Config::getInstance();
	//pModel = ApplicationModel::getInstance();

	size_t internalQueueSize = boost::lexical_cast<size_t>(pConfig->get(util::PROPERTY_INTERNAL_QUEUE_SIZE));
	size_t remoteQueueSize = boost::lexical_cast<size_t>(pConfig->get(util::PROPERTY_REMOTE_QUEUE_SIZE));

	util::log::Loggers::getDefaultLogger()->info("Initializing variables done.");

	std::vector<dto::Stream> vStreams;
	try {
		pStreamGateway->findAll(vStreams);
		util::log::Loggers::getDefaultLogger()->info("Streams loaded.");
	} catch (Exception &e) {
		std::string sErrMsg = "Failed to find all Streams. ";
		sErrMsg.append(e.what());
		util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		//return -1;
	}
	for (size_t i = 0; i < vStreams.size(); ++i) {
		dto::Stream stream = vStreams[i];

		ImageMulticaster* pMulticaster = new ImageMulticaster(stream.getId());
		std::vector<dto::AnalyticInstanceStream> vAnalyticInstances;
		/* Even use analyticInstanceStream, data inside is work like analytic instance
		 * , because AnalyticInstanceStream is used for multiple relationship Analytic instance and Stream
		 */
		try {
			// Get all analytic instance
			pAnalyticInstanceGateway->findAllForStream(stream.getId(), vAnalyticInstances);
			util::log::Loggers::getDefaultLogger()->info("Analytic Instances Streams loaded.");
		} catch (Exception &e) {
			std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
			sErrMsg.append(e.what());
			util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			//return -1;
		}

		// Remove analytic, if it is running by stream id
		/*	for (size_t j = 0; j < vAnalyticInstances.size(); ++j) {
		 if (pModel->isAnalyticRunningByStreamId(vAnalyticInstances[j].getId(), vAnalyticInstances[j].getAnalyticInstanceId())) {
		 vAnalyticInstances.erase(vAnalyticInstances.begin() + j); // Remove by index
		 std::stringstream sErrMsg;
		 sErrMsg << "Remove duplicate analytic instance running: stream " << vAnalyticInstances[j].getId() << " ,analytic instance"
		 << vAnalyticInstances[j].getAnalyticInstanceId();
		 util::log::Loggers::getDefaultLogger()->debug(sErrMsg.str());
		 }
		 }*/

		// Starting Analytic Instances
		for (size_t j = 0; j < vAnalyticInstances.size(); ++j) {
			dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];

			// Check duplicate analytic running instance
			//	if (!pModel->isAnalyticRunningByStreamId(vAnalyticInstances[j].getId(), vAnalyticInstances[j].getAnalyticInstanceId())) {

			// if the Analytic Instance has not been started yet
			AnalyticData* pAD = _pAS->getAnalyticData(analyticInstance.getAnalyticInstanceId());

			if (!(pAD) or !pAD->isAnalyticQueueInAddress()) {
				bool bAIStarted = false;
				std::string sAnalyticQueueInAddress, sAnalyticQueueOutAddress;
				try {
					// start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
					bAIStarted = _pAS->startAnalyticInstance(analyticInstance.getAnalyticInstanceId(), analyticInstance.getAnalyticDirLocation(), analyticInstance.getAnalyticFilename(),
							sAnalyticQueueInAddress, sAnalyticQueueOutAddress);

				} catch (Exception &e) {
					std::stringstream ssErrMsg;
					ssErrMsg << "Failed to start Analytic Instance ";
					ssErrMsg << analyticInstance.getAnalyticInstanceId() << ". ";
					ssErrMsg << e.what();
					util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}
				if (bAIStarted) {
					// store analytic input queue address and output queue address in model

					AnalyticData* pAnalyticInstance = new AnalyticData(analyticInstance.getAnalyticInstanceId());
					//set AnalyticQueueInAddress
					//pModel->getImageInputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] = sAnalyticQueueInAddress;
					pAnalyticInstance->setAnalyticQueueInAddress(sAnalyticQueueInAddress);
					//map<analytic instance id,?>
					//pModel->getResultsOutputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] = sAnalyticQueueOutAddress;
					pAnalyticInstance->setAnalyticQueueOutAddress(sAnalyticQueueOutAddress);

					// if Analytic Instance started, construct Flow Controller for Input Analytic queue and store it on Application Model.
					util::flow::FlowController* pFlowController = new util::flow::SimpleFlowController(remoteQueueSize);

					//util::flow::FlowController* pFlowController = new util::flow::BasicFlowController();
					pAnalyticInstance->setFlowController(pFlowController);


					// Place to set up analytic id
					_pAS->setAnalyticData(analyticInstance.getAnalyticInstanceId(), pAnalyticInstance);

					//pAD =  pAS->setAnalyticInstance(analyticInstance.getAnalyticInstanceId(), pAnalyticInstance);

					std::stringstream ssMsg;
					ssMsg << "Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssMsg << " started.";
					util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
				} else {
					std::stringstream ssErrMsg;
					ssErrMsg << "Starting Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssErrMsg << " failed.";
					util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}
			}
			// Adding Input Image Queue destinations to Image Multicaster
			try {

				pMulticaster->addDestination(analyticInstance);

			} catch (Exception &e) {
				std::string sErrMsg = "Failed to add destination to Image Multicaster. ";
				sErrMsg.append(e.what());
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}
			//} else {
			//std::cout << "Not run duplicate analytic instance , during add to multicast" << analyticInstance.getAnalyticInstanceId() << std::endl;
			//}
		}

		util::log::Loggers::getDefaultLogger()->info("Starting Analytic Instances done.");

		// Creating threads and internal queues
		ConcurrentQueue<Image>* pQueue = NULL; // const only in pointer
		ConsumerThread* pConsumer = NULL;
		ProducerThread* pProducer = NULL;

		std::cout << "Multicast Destination number " << pMulticaster->getNumberOfDestinations() << std::endl;

		StreamData* pSD = _pOS->getStreamData(stream.getId());

		if (pMulticaster->getNumberOfDestinations() > 0) {

			pQueue = new ConcurrentQueue<Image>(internalQueueSize);

			//check if pQueue is in map
			if (pSD) {
				if (!pSD->isInternalQueue()) {
					StreamData* pStreamData = _pOS->getStreamData(stream.getId());
					pStreamData->setInternalQueue(pQueue);
					//pOS->setStream(stream.getId(), pStreamData);

				} else {
					pQueue = pSD->getInternalQueue();
				}
			} else {
				StreamData* pStreamData = new StreamData(stream.getId());
				pStreamData->setInternalQueue(pQueue);
				_pOS->setStream(stream.getId(), pStreamData);

				util::log::Loggers::getDefaultLogger()->debug("MainController: pQueue: Set up StreamData");
			}

			pConsumer = new ConsumerThread(stream.getId(), pMulticaster);

			std::string sVmsPluginDirPath = pConfig->get(util::PROPERTY_VMS_CONNECTOR_DIR);

			if (*sVmsPluginDirPath.rbegin() != '/') // check last char
					{
				sVmsPluginDirPath.append("/");
			}

			sVmsPluginDirPath.append(stream.getVmsConnectorFilename());

			// Loading VMS Connector Plugin
			api::VmsConnector* pVmsConnector = NULL;
			PluginLoader<api::VmsConnector>* pVmsPluginLoader = NULL;

			// if VMS Plugin Loader has been constructed, use it.
			if (_pOS->isVMSConnector(stream.getVmsTypeId())) {
				pVmsPluginLoader = _pOS->getVMSConnector(stream.getVmsTypeId());
			}
			// if VMS Plugin Loader has not been constructed, construct it.
			else {
				try {
					pVmsPluginLoader = new PluginLoader<api::VmsConnector>();

					std::string sVmsPluginPath;
					util::Util::findSharedLibOfPlugin(sVmsPluginDirPath, sVmsPluginPath);
					pVmsPluginLoader->loadPlugin(sVmsPluginPath);
					//pModel->getVmsPluginLoaders()[stream.getVmsTypeId()] = pVmsPluginLoader;
				} catch (Exception &e) {
					std::string sErrMsg = "Failed to load VMS Connector Plugin using VMS Connector Plugin Loader. ";
					sErrMsg.append(e.what());
					util::log::Loggers::getDefaultLogger()->error(sErrMsg);
				}
			}

			// create VMS Connector using the VMS Plugin Loader
			if (pVmsPluginLoader) {
				try {
					pVmsConnector = pVmsPluginLoader->createPluginInstance();
					std::stringstream ssMsg;
					ssMsg << "VMS Connector plugin for Stream " << stream.getId();
					ssMsg << " constructed.";
					util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
				} catch (Exception &e) {
					std::string sErrMsg = "Failed to create VMS Connector plugin. ";
					sErrMsg.append(e.what());
					util::log::Loggers::getDefaultLogger()->error(sErrMsg);
				}
			}

			// init VMS Connector
			api::VmsConnectInfo connInfo = { stream.getVmsServerIp(), stream.getVmsServerPort(), stream.getVmsUsername(), stream.getVmsPassword(), stream.getCameraId(),
					stream.getWidth(), stream.getHeight(), stream.getKeepAspectRatio(), stream.getAllowUpSizing(), stream.getCompressionRate() };
			bool bVmsConnInitDone = false;
			try {
				//=======For Test Gateways====================================
				//bVmsConnInitDone = pVmsConnector->init(connInfo, stream.getVmsConnectorDirLocation());
				//============================================================

				bVmsConnInitDone = pVmsConnector->init(connInfo, sVmsPluginDirPath);

			} catch (std::exception &e) {
				std::string sErrMsg = "Failed to initialize VMS Connector plugin. ";
				sErrMsg.append(e.what());
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}

			if (pVmsConnector && bVmsConnInitDone) {
				std::stringstream ssMsg;
				ssMsg << "VMS Connector plugin " << stream.getId();
				ssMsg << " init done.";
				util::log::Loggers::getDefaultLogger()->info(ssMsg.str());

				// set pProducer
				pProducer = new ProducerThread(stream.getId(), pVmsConnector);
			}
		} //end if of pMulticast

		if (pQueue && pConsumer && pProducer) {

			// Start Consumer and Producer threads
			// check duplicate stream
			//if (!pOS->isStream(stream.getId())) {
			// Place of pConsumer and pProducer
			//} else {
			//std::cout << "Not run duplicate stream " << stream.getId() << std::endl;
			//}

			// Create and start Results Router threads
			for (size_t j = 0; j < vAnalyticInstances.size(); ++j) { // loop for each analytic per stream

				dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];

				// Check if Analytic instance running per stream, in case running duplicate process
				// We can use stream.getId() instead of vAnalyticInatances[j].getId()
				//if (!pModel->isAnalyticRunningByStreamId(vAnalyticInstances[j].getId(), vAnalyticInstances[j].getAnalyticInstanceId())) {

				AnalyticData* pAD = _pAS->getAnalyticData(analyticInstance.getAnalyticInstanceId());

				if (pAD) {
					if (pAD->isAnalyticQueueOutAddress()) {

						// [1] is analytic  server id
						//analytic::AnalyticInstanceManager* analyticInstanceManager = ApplicationModel::getInstance()->getAnalyticInstanceManagers()[analyticServerId];
						AnalyticData* pAnalyticData = NULL;
						// check [if] analytic running different stream, in case one analytic per multiple stream
						if (pAD->isMultipleStream()) {

							pAnalyticData = _pAS->getAnalyticData(analyticInstance.getAnalyticInstanceId());
							pAnalyticData->setStreamId(stream.getId());
							//pAnalyticData->vAnalyticInstanceStreamId.push_back(analyticInstance.getId());

							Loggers::getDefaultLogger()->debug("Found multiple stream per analytic " + analyticInstance.getAnalyticInstanceId());

						} else {

							ResultRouterThread* pResultRouter = new ResultRouterThread(analyticInstance.getAnalyticInstanceId());

							boost::thread* pResultRouterThread = new boost::thread(*pResultRouter);

							// set Analytic data for management

							AnalyticData* pAnalyticData = _pAS->getAnalyticData(analyticInstance.getAnalyticInstanceId());
							if (pAnalyticData && pResultRouterThread->joinable()) {
								Loggers::getDefaultLogger()->debug("MainController: Starting pResulter");
								Loggers::getDefaultLogger()->debug("MainController: update Analytic Data: " + boost::lexical_cast<std::string>(analyticInstance.getAnalyticInstanceId()));
								pAnalyticData->setResultRouterThread(pResultRouterThread);
								pAnalyticData->setStatus(true);
								pAnalyticData->setStreamId(stream.getId());
							}
						}
					}else {
						Loggers::getDefaultLogger()->debug("MainController: pAD is empty");
					}
				}
			}

			boost::thread* pConsumerThread = new boost::thread(*pConsumer);
			if (pConsumerThread->joinable()) {
				Loggers::getDefaultLogger()->debug("MainController: Starting pConsumer");

				if(pSD){
					pSD->setConsumerThread(pConsumerThread);
				}else{
					Loggers::getDefaultLogger()->debug("MainController: pSD is empty");
					pSD = _pOS->getStreamData(stream.getId());
					pSD->setConsumerThread(pConsumerThread);
				}


				boost::thread* pProducerThread = new boost::thread(*pProducer);
				if (pProducerThread->joinable()) {
					Loggers::getDefaultLogger()->debug("MainController: Starting pProducer");
					pSD->setProducerThread(pProducerThread);
				}
			}
		}
	}

	return 0;
}

/*
 void killAllAnalytic() {

 int analyticServerId = 1;

 // have to delete AnalyticInstanceManage here for supporting changing analytic server

 util::log::Loggers::getDefaultLogger()->info("Kill all analytic process.");

 //ThreadPool* pThreadPool;

 //pThreadPool = ThreadPool::getInstance();

 //pThreadPool->interruptAll();
 std::cout << "from kill all" << std::endl;
 //pThreadPool->stopAll(analyticServerId);

 //analytic::AnalyticInstanceManager analyticInstanceManager = ApplicationModel::getInstance()->getAnalyticInstanceManagers()[analyticServerId];
 //std::map<unsigned int, analytic::AnalyticInstanceManager*>::iterator it;

 //cout << "killAllAnalytic" << endl;
 //for (it = mAnalyticInstanceManagers.begin(); it != mAnalyticInstanceManagers.end(); ++it)
 //{
 AnalyticServer* pAS = AnalyticServerManager::getInstance()->getAnalyticServer(analyticServerId);

 if (pAS) {

 if (pAS->killAllAnalyticInstances()) {
 // Remove data that relate with analytic instance in Application Model
 std::map<unsigned int, analytic::AnalyticData*> mAnalyticInstances = pAS->getAllAnalyticData()();
 std::map<unsigned int, analytic::AnalyticData*>::iterator itAnalytic;

 for (itAnalytic = mAnalyticInstances.begin(); itAnalytic != mAnalyticInstances.end(); ++itAnalytic) {

 unsigned int analyticId = itAnalytic->first;

 Loggers::getDefaultLogger()->debug("Analytic Id removed " << analyticId);

 // Remove data by analytic id
 ApplicationModel::getInstance()->removeDataByAIId(analyticId);

 // Remove data by analytic instance id
 std::vector<unsigned int> vAnalyticInstanceStreamId = itAnalytic->second->vAnalyticInstanceStreamId;

 for (std::size_t i = 0; i != vAnalyticInstanceStreamId.size(); ++i) {
 ApplicationModel::getInstance()->removeDataByAISId(vAnalyticInstanceStreamId[i]);
 }

 // Remvoe analytic data itself
 //if (mAnalyticInstances.count(itAnalytic->first) > 0) mAnalyticInstances.erase(itAnalytic->first);

 // Remove data by stream id, not do in stop all
 }

 if (pAnalyticInstanceManager) {

 //mAnalyticInstanceManagers.erase(it);
 //delete pAnalyticInstanceManager;
 //pAnalyticInstanceManager = NULL;

 } else {
 // When fail
 }
 }
 }
 }
 */
/*
 if (!mAnalyticInstanceManagers.empty()) {
 util::log::Loggers::getDefaultLogger()->error(
 "Failed to reset all the Analytic Servers.");
 } else {
 util::log::Loggers::getDefaultLogger()->info(
 "Reset all the Analytic Servers.");
 }
 */

/*
 int startAanalyticById(int analyticServerId, unsigned int analyticInstanceId) {
 // TODO: Handle error to main loop

 // Initializing variables
 //test::gateway::TestStreamGateway streamGateway;
 db::StreamGateway* pStreamGateway = NULL;
 ThreadPool* pThreadPool;
 try {
 pStreamGateway = new db::StreamGateway();
 } catch (Exception &e) {
 std::string sErrMsg = "Failed to create StreamGateway -  ";
 sErrMsg.append(e.what());
 util::log::Loggers::getDefaultLogger()->error(sErrMsg);
 return -1;
 }

 //test::gateway::TestAnalyticInstanceStreamGateway analyticInstanceGateway;
 db::AnalyticInstanceStreamGateway* pAnalyticInstanceGateway = NULL;
 try {
 pAnalyticInstanceGateway = new db::AnalyticInstanceStreamGateway();
 } catch (Exception &e) {
 std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
 sErrMsg.append(e.what());
 util::log::Loggers::getDefaultLogger()->error(sErrMsg);
 return -1;
 }

 util::Config* pConfig = util::Config::getInstance();
 ApplicationModel* pModel = ApplicationModel::getInstance();

 size_t internalQueueSize = boost::lexical_cast<size_t>(pConfig->get(util::PROPERTY_INTERNAL_QUEUE_SIZE));
 size_t remoteQueueSize = boost::lexical_cast<size_t>(pConfig->get(util::PROPERTY_REMOTE_QUEUE_SIZE));
 analytic::AnalyticInstanceManager* pAnalyticInstanceManager = new analytic::AnalyticInstanceManager(pConfig->get(util::PROPERTY_ANALYTIC_SERVER_IP),
 pConfig->get(util::PROPERTY_ANALYTIC_SERVER_PORT));

 if (!pModel->containsAnalyticInstanceManager(analyticServerId)) {
 pModel->getAnalyticInstanceManagers()[analyticServerId] = pAnalyticInstanceManager; // [x] x is a analytic server ID (key)
 }
 //util::log::Loggers::getDefaultLogger()->info("AnalyticInstanceManager size:" + pModel->getAnalyticInstanceManagers().size());

 util::log::Loggers::getDefaultLogger()->info("Initializing variables done.");

 std::vector<dto::Stream> vStreams;
 try {
 pStreamGateway->findAllByAnalyticInstanceId(vStreams, analyticInstanceId);
 util::log::Loggers::getDefaultLogger()->info("Streams loaded by Analytic instance id: " + analyticInstanceId);
 } catch (Exception &e) {
 std::string sErrMsg = "Failed to find all Streams. ";
 sErrMsg.append(e.what());
 util::log::Loggers::getDefaultLogger()->error(sErrMsg);
 return -1;
 }
 for (size_t i = 0; i < vStreams.size(); ++i) {
 dto::Stream stream = vStreams[i];
 ImageMulticaster* pMulticaster = new ImageMulticaster(stream.getId());
 // If a analytic instance use same stream, it should not create new stream variable.
 std::vector<dto::AnalyticInstanceStream> vAnalyticInstances;
 try {
 pAnalyticInstanceGateway->findAllForStream(stream.getId(), vAnalyticInstances);
 util::log::Loggers::getDefaultLogger()->info("Analytic Instances Streams loaded.");
 } catch (Exception &e) {
 std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
 sErrMsg.append(e.what());
 util::log::Loggers::getDefaultLogger()->error(sErrMsg);
 return -1;
 }

 // Remove analytic, if it is running by stream id
 for (size_t j = 0; j < vAnalyticInstances.size(); ++j) {
 if (pModel->isAnalyticRunningByStreamId(vAnalyticInstances[j].getId(), vAnalyticInstances[j].getAnalyticInstanceId())) {
 vAnalyticInstances.erase(vAnalyticInstances.begin() + j); // Remove by index
 std::stringstream sErrMsg;
 sErrMsg << "Remove duplicate analytic instance running: stream " << vAnalyticInstances[j].getId() << " ,analytic instance"
 << vAnalyticInstances[j].getAnalyticInstanceId();
 util::log::Loggers::getDefaultLogger()->debug(sErrMsg.str());
 }
 }

 // Starting Analytic Instances
 for (size_t j = 0; j < vAnalyticInstances.size(); ++j) {
 // Check duplicate analytic running instance
 if (!pModel->isAnalyticRunningByStreamId(vAnalyticInstances[j].getId(), vAnalyticInstances[j].getAnalyticInstanceId())) {

 dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];
 // if the Analytic Instance has not been started yet
 if (!pModel->containsImageInputQueueAddress(analyticInstance.getAnalyticInstanceId())) {
 bool bAIStarted = false;
 std::string sAnalyticQueueInAddress, sAnalyticQueueOutAddress;
 try {
 // start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
 bAIStarted = pAnalyticInstanceManager->startAnalyticInstance(analyticInstance.getAnalyticInstanceId(), analyticInstance.getAnalyticDirLocation(),
 analyticInstance.getAnalyticFilename(), sAnalyticQueueInAddress, sAnalyticQueueOutAddress);
 } catch (Exception &e) {
 std::stringstream ssErrMsg;
 ssErrMsg << "Failed to start Analytic Instance ";
 ssErrMsg << analyticInstance.getAnalyticInstanceId() << ". ";
 ssErrMsg << e.what();
 util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
 }
 if (bAIStarted) {
 // store analytic input queue address and output queue address in model
 pModel->getImageInputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] = sAnalyticQueueInAddress;
 pModel->getResultsOutputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] = sAnalyticQueueOutAddress;
 // if Analytic Instance started, construct Flow Controller for Input Analytic queue and store it on Application Model.
 util::flow::FlowController* pFlowController = new util::flow::SimpleFlowController(remoteQueueSize);
 //util::flow::FlowController* pFlowController = new util::flow::BasicFlowController();
 pModel->getFlowControllers()[analyticInstance.getAnalyticInstanceId()] = pFlowController;
 std::stringstream ssMsg;
 ssMsg << "Analytic Instance " << analyticInstance.getAnalyticInstanceId();
 ssMsg << " started.";
 util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
 } else {
 std::stringstream ssErrMsg;
 ssErrMsg << "Starting Analytic Instance " << analyticInstance.getAnalyticInstanceId();
 ssErrMsg << " failed.";
 util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
 }
 }
 // Adding Input Image Queue destinations to Image Multicaster
 try {
 pMulticaster->addDestination(analyticInstance);
 } catch (Exception &e) {
 std::string sErrMsg = "Failed to add destination to Image Multicaster. ";
 sErrMsg.append(e.what());
 util::log::Loggers::getDefaultLogger()->error(sErrMsg);
 }
 }
 }

 util::log::Loggers::getDefaultLogger()->info("Starting Analytic Instances done.");

 // Creating threads and internal queues
 ConcurrentQueue<Image>* pQueue = NULL;
 ConsumerThread* pConsumer = NULL;
 ProducerThread* pProducer = NULL;
 if (pMulticaster->getNumberOfDestinations() > 0) {
 pQueue = new ConcurrentQueue<Image>(internalQueueSize);

 //check if pQueue is in map
 if (pModel->containsInternalQueue(stream.getId())) {
 pQueue = pModel->getInternalQueues()[stream.getId()];
 } else {
 pModel->getInternalQueues()[stream.getId()] = pQueue;
 }

 pConsumer = new ConsumerThread(stream.getId(), pMulticaster);

 std::string sVmsPluginDirPath = pConfig->get(util::PROPERTY_VMS_CONNECTOR_DIR);
 if (*sVmsPluginDirPath.rbegin() != '/') // check last char
 {
 sVmsPluginDirPath.append("/");
 }
 sVmsPluginDirPath.append(stream.getVmsConnectorFilename());

 // Loading VMS Connector Plugin
 api::VmsConnector* pVmsConnector = NULL;
 PluginLoader<api::VmsConnector>* pVmsPluginLoader =
 NULL;
 // if VMS Plugin Loader has been constructed, use it.
 if (pModel->containsVmsPluginLoader(stream.getVmsTypeId())) {
 pVmsPluginLoader = pModel->getVmsPluginLoaders()[stream.getVmsTypeId()];
 }
 // if VMS Plugin Loader has not been constructed, construct it.
 else {
 try {
 pVmsPluginLoader = new PluginLoader<api::VmsConnector>();

 std::string sVmsPluginPath;
 util::Util::findSharedLibOfPlugin(sVmsPluginDirPath, sVmsPluginPath);
 pVmsPluginLoader->loadPlugin(sVmsPluginPath);
 pModel->getVmsPluginLoaders()[stream.getVmsTypeId()] = pVmsPluginLoader;
 } catch (Exception &e) {
 std::string sErrMsg = "Failed to load VMS Connector Plugin using VMS Connector Plugin Loader. ";
 sErrMsg.append(e.what());
 util::log::Loggers::getDefaultLogger()->error(sErrMsg);
 }
 }
 // create VMS Connector using the VMS Plugin Loader
 if (pVmsPluginLoader) {
 try {
 pVmsConnector = pVmsPluginLoader->createPluginInstance();
 std::stringstream ssMsg;
 ssMsg << "VMS Connector plugin for Stream " << stream.getId();
 ssMsg << " constructed.";
 util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
 } catch (Exception &e) {
 std::string sErrMsg = "Failed to create VMS Connector plugin. ";
 sErrMsg.append(e.what());
 util::log::Loggers::getDefaultLogger()->error(sErrMsg);
 }
 }
 // init VMS Connector
 api::VmsConnectInfo connInfo = { stream.getVmsServerIp(), stream.getVmsServerPort(), stream.getVmsUsername(), stream.getVmsPassword(), stream.getCameraId(),
 stream.getWidth(), stream.getHeight(), stream.getKeepAspectRatio(), stream.getAllowUpSizing(), stream.getCompressionRate() };
 bool bVmsConnInitDone = false;
 try {
 //=======For Test Gateways====================================
 //bVmsConnInitDone = pVmsConnector->init(connInfo, stream.getVmsConnectorDirLocation());
 //============================================================
 bVmsConnInitDone = pVmsConnector->init(connInfo, sVmsPluginDirPath);
 } catch (std::exception &e) {
 std::string sErrMsg = "Failed to initialize VMS Connector plugin. ";
 sErrMsg.append(e.what());
 util::log::Loggers::getDefaultLogger()->error(sErrMsg);
 }
 if (pVmsConnector && bVmsConnInitDone) {
 std::stringstream ssMsg;
 ssMsg << "VMS Connector plugin " << stream.getId();
 ssMsg << " init done.";
 util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
 pProducer = new ProducerThread(stream.getId(), pVmsConnector);
 }
 }

 pThreadPool = ThreadPool::getInstance();

 if (pQueue && pConsumer && pProducer) {

 // Start Consumer and Producer threads
 if (!pModel->isStreamRunning(stream.getId())) {
 boost::thread* pConsumerThread = new boost::thread(*pConsumer);
 if (pConsumerThread->joinable()) {

 pThreadPool->setConsumerThreadManagers(stream.getId(), pConsumerThread);

 boost::thread* pProducerThread = new boost::thread(*pProducer);
 if (pProducerThread->joinable()) {

 pThreadPool->setProducerThreadManagers(stream.getId(), pProducerThread);
 }
 }
 }

 // Create and start Results Router threads
 for (size_t j = 0; j < vAnalyticInstances.size(); ++j) { // loop for each analytic per stream

 dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];

 // Check if Analytic instance running per stream, in case running duplicate process
 // We can use stream.getId() instead of vAnalyticInatances[j].getId()
 if (!pModel->isAnalyticRunningByStreamId(vAnalyticInstances[j].getId(), vAnalyticInstances[j].getAnalyticInstanceId())) {

 if (pModel->containsResultsOutputQueueAddress(analyticInstance.getAnalyticInstanceId())) {

 // [1] is analytic  server id
 analytic::AnalyticInstanceManager* analyticInstanceManager = ApplicationModel::getInstance()->getAnalyticInstanceManagers()[analyticServerId];

 // check [if] analytic running different stream, in case one analytic per multiple stream
 if (analyticInstanceManager->isMultipleStreamPerAnalytic(analyticInstance.getAnalyticInstanceId())) {
 analytic::AnalyticData* analyticData = analyticInstanceManager->getAanalyticData(analyticInstance.getAnalyticInstanceId());
 analyticData->vStreamId.push_back(stream.getId());

 } else {

 ResultRouterThread* pResultsRouter = new ResultRouterThread(analyticInstance.getAnalyticInstanceId());

 boost::thread* pResultsRouterThread = new boost::thread(*pResultsRouter);

 //map<AIId,Thread>
 pThreadPool->setResultRouterThreadManagers(analyticInstance.getAnalyticInstanceId(), pResultsRouterThread);

 // set Analytic data for management
 analytic::AnalyticData* analyticData = new analytic::AnalyticData();

 analyticData->bStatus = true;
 //analyticData->vAnalyticInstanceStreamId.push_back(analyticInstance.getId()); // this will get analytic instance stream id and set to the vector
 analyticData->vStreamId.push_back(stream.getId());

 analyticInstanceManager->getAnalyticInstances()[analyticInstance.getAnalyticInstanceId()] = analyticData;
 }

 // Set counting analytic instance running to stream
 pModel->setStreamManages(stream.getId(), analyticInstance.getAnalyticInstanceId());
 }
 }
 }

 }
 return 0;
 }
 }*/

void stopAnalytic() {
// Delete consumer when no analytic running
// Delete Producer when no analytic running
// can delete Result router thread by analytic id
// Delete 3 map from App Model
}

MainController::~MainController() {
// TODO Auto-generated destructor stub
}
} /* namesapce controller */
} /* namespace opencctv */
