#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
/* boost includes (/user/local/include) */
/* boost libraries (/usr/local/lib) -lboost_system */
#include <boost/thread/thread.hpp> // -lboost_thread -pthread
#include <boost/lockfree/queue.hpp>
#include "opencctv/util/log/Loggers.hpp"
#include "analytic/xml/AnalyticMessage.hpp"
#include "opencctv/mq/TcpMqReceiver.hpp"
#include "opencctv/Exception.hpp"
#include "opencctv/mq/TcpMqSender.hpp"
#include "analytic/ConcurrentQueue.hpp"
#include "analytic/api/Analytic.hpp"
#include "opencctv/PluginLoader.hpp"
#include "analytic/ProducerThread.hpp"
#include "analytic/ConsumerThread.hpp"
#include "opencctv/util/serialization/gpb/ProtoBuf.hpp"
#include "analytic/util/Config.hpp"
//#include "analytic/ImageQueue.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
	if(argc < 6)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Invalid number of arguments.");
		return -1;
	}
	// Sending PID of Analytic process to Analytic Starter process through stdout
	fprintf(stdout, opencctv::util::Util::getPidMessage(getpid()).c_str());
	fflush (stdout);
	// Saving input arguments
	std::string sAnalyticInstanceId = argv[1];
	std::string sAnalyticPluginDirLocation = argv[2];
	std::string sAnalyticPluginFilename = argv[3];
	std::string sInputAnalyticQueueAddress = argv[4];
	std::string sOutputAnalyticQueueAddress = argv[5];

	// Creating Image input queue
	opencctv::mq::TcpMqReceiver* pReceiver = NULL;
	try {
		pReceiver = new opencctv::mq::TcpMqReceiver();
		pReceiver->createMq(sInputAnalyticQueueAddress);
	} catch (opencctv::Exception &e) {
		std::string sErrMsg = "Failed to create Analytic input image queue. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Creating Image input queue done.");

	// Creating Results output queue
	opencctv::mq::TcpMqSender* pSender = NULL;
	try {
		pSender = new opencctv::mq::TcpMqSender();
		pSender->createMq(sOutputAnalyticQueueAddress);
	} catch (opencctv::Exception &e) {
		std::string sErrMsg = "Failed to create Analytic Results output queue. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Creating Results output queue done.");

	// Creating internal input, output queue
	analytic::ConcurrentQueue<analytic::api::Image_t>* pInputImageQueue = new analytic::ConcurrentQueue<analytic::api::Image_t>(5);
	analytic::ConcurrentQueue<analytic::api::Image_t>* pOutputResultQueue = new analytic::ConcurrentQueue<analytic::api::Image_t>(5);
	/*analytic::ImageQueue<analytic::api::Image_t>* pInputImageQueue = new analytic::ImageQueue<analytic::api::Image_t>(5);
	analytic::ImageQueue<analytic::api::Image_t>* pOutputResultQueue = new analytic::ImageQueue<analytic::api::Image_t>(5);*/

	opencctv::util::log::Loggers::getDefaultLogger()->info("Creating internal input, output queue done.");

	// Loading Analytic plugin
	opencctv::PluginLoader<analytic::api::Analytic> analyticLoader;
	analytic::api::Analytic* pAnalytic = NULL;
	try {
		//std::string sAnalyticPluginPath = sAnalyticPluginDirLocation;
		//sAnalyticPluginPath.append("/").append(sAnalyticPluginFilename);
		sAnalyticPluginDirLocation = analytic::util::Config::getInstance()->get(analytic::util::PROPERTY_ANALYTIC_PLUGIN_DIR);
		if(*sAnalyticPluginDirLocation.rbegin() != '/') // check last char
		{
			sAnalyticPluginDirLocation.append("/");
		}
		sAnalyticPluginDirLocation.append(sAnalyticPluginFilename);
		std::string sAnalyticPluginPath;
		opencctv::util::Util::findSharedLibOfPlugin(sAnalyticPluginDirLocation, sAnalyticPluginPath);
		//std::cerr << "Analytic plugin path = " << sAnalyticPluginDirLocation << std::endl;
		analyticLoader.loadPlugin(sAnalyticPluginPath);
		pAnalytic = analyticLoader.createPluginInstance();
	} catch (opencctv::Exception &e) {
		std::string sErrMsg = "Failed to load Analytic plugin. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}
	if (!pAnalytic) {
		opencctv::util::log::Loggers::getDefaultLogger()->error("Loaded Analytic is NULL.");
		return -1;
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Loading Analytic plugin done.");

	// Init Analytic plugin
	if(!pAnalytic->init(sAnalyticPluginDirLocation))
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Analytic plugin init failed.");
		return -1;
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Init Analytic plugin done.");

	// Creating threads
	boost::thread_group threadGroup;
	analytic::ProducerThread imageProducer(pInputImageQueue, pReceiver, new opencctv::util::serialization::ProtoBuf());
	boost::thread* pProducerThread = new boost::thread(imageProducer);
	if (pProducerThread->joinable()) {
		analytic::ConsumerThread resultsConsumer(pOutputResultQueue, pSender, new opencctv::util::serialization::ProtoBuf());
		boost::thread* pConsumerThread = new boost::thread(resultsConsumer);
		if (pConsumerThread->joinable()) {
			threadGroup.add_thread(pProducerThread);
			threadGroup.add_thread(pConsumerThread);
		}
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Creating threads done.");

	// Starting Analytic plugin
	opencctv::util::log::Loggers::getDefaultLogger()->info("Starting Analytic plugin...");
	pAnalytic->process(pInputImageQueue, pOutputResultQueue);
	// threadGroup.join_all(); //wait for threads finish
	return 0;
}
