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


void terminateHandler(int signum); // Terminate signal handler

int main()
{
	/*
	// Sending PID of OpenCCTV Server process to OpenCCTV Starter process through stdout
	fprintf(stdout, opencctv::util::Util::getPidMessage(getpid()).c_str());
	fflush (stdout);
	*/

	// Registering signal handlers
	signal(SIGTERM, terminateHandler); // for Terminate signal
	signal(SIGINT, terminateHandler); // for Ctrl + C keyboard interrupt

	// Initializing variables
	//test::gateway::TestStreamGateway streamGateway;
	opencctv::db::StreamGateway* pStreamGateway = NULL;
	try
	{
		pStreamGateway = new opencctv::db::StreamGateway();
	}
	catch(opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create StreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	//test::gateway::TestAnalyticInstanceStreamGateway analyticInstanceGateway;
	opencctv::db::AnalyticInstanceStreamGateway* pAnalyticInstanceGateway = NULL;
	try
	{
		pAnalyticInstanceGateway = new opencctv::db::AnalyticInstanceStreamGateway();
	}
	catch(opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	opencctv::util::Config* pConfig = opencctv::util::Config::getInstance();
	opencctv::ApplicationModel* pModel = opencctv::ApplicationModel::getInstance();
	boost::thread_group _producerThreadGroup;
	boost::thread_group _consumerThreadGroup;
	boost::thread_group _resultsRouterThreadGroup;
	size_t internalQueueSize = boost::lexical_cast<size_t>(pConfig->get(opencctv::util::PROPERTY_INTERNAL_QUEUE_SIZE));
	size_t remoteQueueSize = boost::lexical_cast<size_t>(pConfig->get(opencctv::util::PROPERTY_REMOTE_QUEUE_SIZE));
	analytic::AnalyticInstanceManager* pAnalyticInstanceManager = new analytic::AnalyticInstanceManager(pConfig->get(opencctv::util::PROPERTY_ANALYTIC_SERVER_IP), pConfig->get(opencctv::util::PROPERTY_ANALYTIC_SERVER_PORT));
	pModel->getAnalyticInstanceManagers()[1] = pAnalyticInstanceManager;
	opencctv::util::log::Loggers::getDefaultLogger()->info("Initializing variables done.");

	std::vector<opencctv::dto::Stream> vStreams;
	try
	{
		pStreamGateway->findAll(vStreams);
		opencctv::util::log::Loggers::getDefaultLogger()->info("Streams loaded.");
	}
	catch(opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to find all Streams. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}
	for(size_t i = 0; i < vStreams.size(); ++i)
	{
		opencctv::dto::Stream stream = vStreams[i];
		opencctv::ImageMulticaster* pMulticaster = new opencctv::ImageMulticaster(stream.getId());
		std::vector<opencctv::dto::AnalyticInstanceStream> vAnalyticInstances;
		try
		{
			pAnalyticInstanceGateway->findAllForStream(stream.getId(), vAnalyticInstances);
			opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic Instances Streams loaded.");
		}
		catch(opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			return -1;
		}
		// Starting Analytic Instances
		for(size_t j = 0; j < vAnalyticInstances.size(); ++j) {
			opencctv::dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];
			// if the Analytic Instance has not been started yet
			if(!pModel->containsImageInputQueueAddress(analyticInstance.getAnalyticInstanceId())) {
				bool bAIStarted = false;
				std::string sAnalyticQueueInAddress, sAnalyticQueueOutAddress;
				try {
					// start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
					bAIStarted = pAnalyticInstanceManager->startAnalyticInstance(
							analyticInstance.getAnalyticInstanceId(),
							analyticInstance.getAnalyticDirLocation(),
							analyticInstance.getAnalyticFilename(), sAnalyticQueueInAddress, sAnalyticQueueOutAddress);
				} catch (opencctv::Exception &e) {
					std::stringstream ssErrMsg;
					ssErrMsg << "Failed to start Analytic Instance ";
					ssErrMsg << analyticInstance.getAnalyticInstanceId() << ". ";
					ssErrMsg << e.what();
					opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}
				if (bAIStarted) {
					// store analytic input queue address and output queue address in model
					pModel->getImageInputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] = sAnalyticQueueInAddress;
					pModel->getResultsOutputQueueAddresses()[analyticInstance.getAnalyticInstanceId()] = sAnalyticQueueOutAddress;
					// if Analytic Instance started, construct Flow Controller for Input Analytic queue and store it on Application Model.
					opencctv::util::flow::FlowController* pFlowController = new opencctv::util::flow::SimpleFlowController(remoteQueueSize);
					//opencctv::util::flow::FlowController* pFlowController = new opencctv::util::flow::BasicFlowController();
					pModel->getFlowControllers()[analyticInstance.getAnalyticInstanceId()] = pFlowController;
					std::stringstream ssMsg;
					ssMsg << "Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssMsg << " started.";
					opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
				}
				else {
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
		if(pMulticaster->getNumberOfDestinations() > 0) {
			pQueue = new opencctv::ConcurrentQueue<opencctv::Image>(internalQueueSize);
			pModel->getInternalQueues()[stream.getId()] = pQueue;
			pConsumer = new opencctv::ConsumerThread(stream.getId(), pMulticaster);

			std::string sVmsPluginDirPath = pConfig->get(opencctv::util::PROPERTY_VMS_CONNECTOR_DIR);
			if(*sVmsPluginDirPath.rbegin() != '/') // check last char
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
					std::string sErrMsg = "Failed to load VMS Connector Plugin using VMS Connector Plugin Loader. ";
					sErrMsg.append(e.what());
					opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
				}
			}
			// create VMS Connector using the VMS Plugin Loader
			if(pVmsPluginLoader)
			{
				try {
					pVmsConnector = pVmsPluginLoader->createPluginInstance();
					std::stringstream ssMsg;
					ssMsg << "VMS Connector plugin for Stream " << stream.getId();
					ssMsg << " constructed.";
					opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
				} catch (opencctv::Exception &e) {
					std::string sErrMsg = "Failed to create VMS Connector plugin. ";
					sErrMsg.append(e.what());
					opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
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
				bVmsConnInitDone = pVmsConnector->init(connInfo, sVmsPluginDirPath);
			} catch (std::exception &e) {
				std::string sErrMsg =
						"Failed to initialize VMS Connector plugin. ";
				sErrMsg.append(e.what());
				opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}
			if(pVmsConnector && bVmsConnInitDone)
			{
				std::stringstream ssMsg;
				ssMsg << "VMS Connector plugin " << stream.getId();
				ssMsg << " init done.";
				opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
				pProducer = new opencctv::ProducerThread(stream.getId(), pVmsConnector);
			}
		}
		if (pQueue && pConsumer && pProducer) {
			// Create and start Results Router threads
			for(size_t j = 0; j < vAnalyticInstances.size(); ++j) {
				opencctv::dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];
				if(pModel->containsResultsOutputQueueAddress(analyticInstance.getAnalyticInstanceId()))
				{
					opencctv::ResultRouterThread* pResultsRouter = new opencctv::ResultRouterThread(analyticInstance.getAnalyticInstanceId());
					boost::thread* pResultsRouterThread = new boost::thread(*pResultsRouter);
					if(pResultsRouterThread->joinable())
					{
						_resultsRouterThreadGroup.add_thread(pResultsRouterThread);
					}
				}
			}
			// Start Consumer and Producer threads
			boost::thread* pConsumerThread = new boost::thread(*pConsumer);
			if (pConsumerThread->joinable()) {
				boost::thread* pProducerThread = new boost::thread(*pProducer);
				if (pProducerThread->joinable()) {
					_consumerThreadGroup.add_thread(pConsumerThread);
					_producerThreadGroup.add_thread(pProducerThread);
				}
			}
		}
	}
	_resultsRouterThreadGroup.join_all();
	// _consumerThreadGroup.join_all();
	// _producerThreadGroup.join_all();
	return 0;
}

// Signal handler for SIGTERM (Terminate signal)
void terminateHandler(int signum) {
	std::map<unsigned int, analytic::AnalyticInstanceManager*> mAnalyticInstanceManagers = opencctv::ApplicationModel::getInstance()->getAnalyticInstanceManagers();
	std::map<unsigned int, analytic::AnalyticInstanceManager*>::iterator it;
	for(it = mAnalyticInstanceManagers.begin(); it != mAnalyticInstanceManagers.end(); /*it increment below*/) {
		analytic::AnalyticInstanceManager* pAnalyticInstanceManager = it->second;
		if(pAnalyticInstanceManager->killAllAnalyticInstances())
		{
			if(pAnalyticInstanceManager)
			{
				delete pAnalyticInstanceManager;
				pAnalyticInstanceManager = NULL;
			}
			mAnalyticInstanceManagers.erase(it++); //remove analytic instance manager from the model
		}
		else
		{
			++it;
		}
	}
	if(!mAnalyticInstanceManagers.empty())
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Failed to reset all the Analytic Servers.");
	}
	else
	{
		opencctv::util::log::Loggers::getDefaultLogger()->info("Reset all the Analytic Servers.");
	}
	exit(signum);
}
