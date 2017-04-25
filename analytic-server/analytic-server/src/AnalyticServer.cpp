#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <signal.h> // to handle terminate signal

/* boost includes (/user/local/include) */
/* boost libraries (/usr/local/lib) -lboost_system */
#include <boost/thread/thread.hpp> // -lboost_thread -pthread
//#include <boost/lockfree/queue.hpp>

#include "opencctv/mq/TcpMqReceiver.hpp"
#include "opencctv/Exception.hpp"
#include "opencctv/mq/TcpMqSender.hpp"
#include "opencctv/PluginLoader.hpp"
#include "opencctv/util/serialization/gpb/ProtoBuf.hpp"
#include "analytic/util/log/Loggers.hpp"

#include "analytic/ConcurrentQueue.hpp"
#include "analytic/api/Analytic.hpp"
#include "analytic/api/IFrameGrabberWrapper.hpp"

#include "FrameGrabberWrapper.h"

#include "analytic/xml/AnalyticMessage.hpp"
//#include "analytic/ProducerThread.hpp"
#include "analytic/ConsumerThread.hpp"
#include "analytic/util/Config.hpp"

#include "analytic/db/AnalyticInstanceStreamGateway.hpp"
#include "analytic/db/AnalyticInstanceConfigGateway.hpp"
//#include "analytic/ImageQueue.hpp"

#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

using namespace opencctv;
using namespace analytic;

void exitHandler(int iSignum);
std::map<string, api::IFrameGrabber *> mFrameGrabbers;

int main(int argc, char *argv[])
{
	// Registering signal SIGINT and signal handler
	signal(SIGINT, exitHandler); // for Ctrl + C keyboard interrupt
	signal(SIGTERM, exitHandler); // for Terminate signal
	signal(SIGKILL, exitHandler); // for SIGKILL

	if (argc < 6)
	{
		analytic::util::log::Loggers::getDefaultLogger()->error("Invalid number of arguments.");
		return -1;
	}

	// Sending PID of Analytic process to Analytic Starter process through stdout
	fprintf(stdout, "%s", opencctv::util::Util::getPidMessage(getpid()).c_str());
	fflush(stdout);

	// Saving input arguments
	std::string sAnalyticInstanceId = argv[1];
	std::string sAnalyticPluginDirLocation = argv[2];
	std::string sAnalyticPluginFilename = argv[3];
	std::string sInputAnalyticQueueAddress = argv[4];
	std::string sOutputAnalyticQueueAddress = argv[5];

	analytic::util::log::Loggers::getDefaultLogger()->debug("From Analytic Server Instance id: " + sAnalyticInstanceId + " plugin location: " + sAnalyticPluginDirLocation + " plugin file anem: " + sAnalyticPluginFilename + " Input Queue port: " + sInputAnalyticQueueAddress + " Output queue port: " + sOutputAnalyticQueueAddress);

	//std::cout << "Test log console with cout xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;

	/*
		// Creating Image input queue
		opencctv::mq::TcpMqReceiver *pReceiver = NULL;

		try
		{
			pReceiver = new opencctv::mq::TcpMqReceiver();
			pReceiver->createMq(sInputAnalyticQueueAddress);
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to create Analytic input image queue. ";
			sErrMsg.append(e.what());
			analytic::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			return -1;
		}

		analytic::util::log::Loggers::getDefaultLogger()->info("Creating Image input queue done.");
	*/
	// Creating Results output queue
	opencctv::mq::TcpMqSender *pSender = NULL;

	try
	{
		pSender = new opencctv::mq::TcpMqSender();
		if(pSender->createMq(sOutputAnalyticQueueAddress))
        {
            analytic::util::log::Loggers::getDefaultLogger()->info("Analytic instance ResultOutput started with port: " + sOutputAnalyticQueueAddress);
        }else
        {
            analytic::util::log::Loggers::getDefaultLogger()->error("Cannot start ResultOutput  port: " + sOutputAnalyticQueueAddress);
            return -1;
        }
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create Analytic Results output queue. ";
		sErrMsg.append(e.what());
		analytic::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	

	// Creating internal input, output queue

	analytic::ConcurrentQueue<analytic::api::Image_t> *pOutputResultQueue = new analytic::ConcurrentQueue<analytic::api::Image_t> (5);

	analytic::util::log::Loggers::getDefaultLogger()->info("Creating output queue done.");

	// Loading Analytic plugin
	opencctv::PluginLoader<analytic::api::Analytic> analyticLoader;
	analytic::api::Analytic *pAnalytic = NULL;

	try
	{
		//std::string sAnalyticPluginPath = sAnalyticPluginDirLocation;
		//sAnalyticPluginPath.append("/").append(sAnalyticPluginFilename);
		sAnalyticPluginDirLocation = analytic::util::Config::getInstance()->get(analytic::util::PROPERTY_ANALYTIC_PLUGIN_DIR);

		if (*sAnalyticPluginDirLocation.rbegin() != '/')     // check last char
		{
			sAnalyticPluginDirLocation.append("/");
		}

		sAnalyticPluginDirLocation.append(sAnalyticPluginFilename);
		std::string sAnalyticPluginPath;
		opencctv::util::Util::findSharedLibOfPlugin(sAnalyticPluginDirLocation, sAnalyticPluginPath);
		//std::cerr << "Analytic plugin path = " << sAnalyticPluginDirLocation << std::endl;
		analyticLoader.loadPlugin(sAnalyticPluginPath);
		pAnalytic = analyticLoader.createPluginInstance();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to load Analytic plugin. ";
		sErrMsg.append(e.what());
		analytic::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	if (!pAnalytic)
	{
		analytic::util::log::Loggers::getDefaultLogger()->error("Loaded Analytic is NULL.");
		return -1;
	}

	analytic::util::log::Loggers::getDefaultLogger()->info("Loading Analytic plugin done.");

// Preparing analytic instance input stream
	analytic::db::AnalyticInstanceStreamGateway *pAnalyticInstanceStreamGateway = NULL;

	try
	{
		pAnalyticInstanceStreamGateway = new analytic::db::AnalyticInstanceStreamGateway();
		analytic::util::log::Loggers::getDefaultLogger()->info("Analytic instance stream variable created.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		analytic::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	std::vector<analytic::dto::AnalyticInstanceStream> vAnalyticInstanceStream;

	try
	{
		pAnalyticInstanceStreamGateway->getAnalyticInstanceStreams(boost::lexical_cast<unsigned int> (sAnalyticInstanceId), vAnalyticInstanceStream);
		analytic::util::log::Loggers::getDefaultLogger()->info("Analytic instance stream loaded.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to get all stream by analytic instance id. ";
		sErrMsg.append(e.what());
		analytic::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

// End preparing analytic instance input stream 	//

// Preparing analytic instance config
	analytic::db::AnalyticInstanceConfigGateway *pAnalyticInstanceConfigGateway = NULL;

	try
	{
		pAnalyticInstanceConfigGateway = new analytic::db::AnalyticInstanceConfigGateway();
		analytic::util::log::Loggers::getDefaultLogger()->info("Analytic instance config variable created.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create AnalyticInstanceConfigGateway -  ";
		sErrMsg.append(e.what());
		analytic::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	std::vector<analytic::dto::AnalyticInstanceConfig> vAnalyticInstaceConfig;

	try
	{

		pAnalyticInstanceConfigGateway->getAnalyticInstanceConfig(boost::lexical_cast<unsigned int> (sAnalyticInstanceId), vAnalyticInstaceConfig);
		analytic::util::log::Loggers::getDefaultLogger()->info("Analytic instance config loaded, with " + boost::lexical_cast<std::string>(vAnalyticInstaceConfig.size()) + " config files.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to get all analytic config by analytic instance id. ";
		sErrMsg.append(e.what());
		analytic::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

// End preparing analytic instance config
	analytic::util::log::Loggers::getDefaultLogger()->info("Starting config FrameGrabber, with " + boost::lexical_cast<string>(vAnalyticInstanceStream.size()) + " channels");

// Creating framgraber object


	if (!vAnalyticInstanceStream.empty())
	{
		for (int i = 0; i < vAnalyticInstanceStream.size(); i++)
		{

			analytic::dto::AnalyticInstanceStream AIS = vAnalyticInstanceStream[i];

			FrameGrabberWrapper *pFGW =  new FrameGrabberWrapper();
			analytic::api::IFrameGrabberWrapper *pIFGW = pFGW;
			pIFGW->init(analytic::util::log::Loggers::getDefaultLogger());

			FileStorage config(AIS.getConfig(), FileStorage::READ + FileStorage::MEMORY);
			pIFGW->setDefaultConfig(config);

			vector<string> strs;
			boost::split(strs, AIS.getURL(), boost::is_any_of("/"), boost::token_compress_on);
			//std::cout << "getURL() " << AIS.getURL() << std::endl;
			//std::cout << "size: "<< strs.size() << std::endl;
			string sUrl;

			for (int i = 0 ; i < strs.size() ; i++)
			{
				std::cout << i << " " <<  strs[i] << std::endl;

				if (i == 0)
					sUrl = strs[i] + "//" + AIS.getUsername() + ":" + AIS.getPassword() + "@";
				else if (i == 1)
					sUrl +=  strs[i];
				else
					sUrl += "/" + strs[i];

				//std::cout << "sURL: " << sUrl << std::endl;
			}

			//string sUrl = strs[0] + "://" + AIS.getUsername() + ":" + AIS.getPassword() + "@" + strs[1];

			int iWidth = AIS.getWidth();
			int iHeight = AIS.getHeight();
			int iFps = 30; //AIS.getFps();

			analytic::util::log::Loggers::getDefaultLogger()->info("Set streamConfig " + sUrl + " " + boost::lexical_cast<std::string>(iWidth) + " " + boost::lexical_cast<std::string>(iHeight) + " " + boost::lexical_cast<std::string>(iFps));

			sUrl = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";

			//sUrl = "rtsp://192.41.170.200:554/now.mp4";

			pIFGW->setStreamConfig(sUrl, iWidth, iHeight, iFps);

			mFrameGrabbers[AIS.getInputName()] = pIFGW->getFrameGrabberObj();
		}

		analytic::util::log::Loggers::getDefaultLogger()->info("FrameGrabber config setted  " + boost::lexical_cast<std::string>(mFrameGrabbers.size()) + " items.");
	}


// End creating framgraber object

// Creating anlaytic config object (File storage)

	std::map<string, FileStorage> mConfigFiles;

	if (!vAnalyticInstaceConfig.empty())
	{
		for (int i = 0; i < vAnalyticInstaceConfig.size(); i++)
		{
			analytic::dto::AnalyticInstanceConfig AIC = vAnalyticInstaceConfig[i];
			FileStorage fs(AIC.getData(), FileStorage::READ + FileStorage::MEMORY);
			mConfigFiles[AIC.getFileName()] = fs;
		}

		analytic::util::log::Loggers::getDefaultLogger()->info("Analytic config setted " + boost::lexical_cast<std::string>(mConfigFiles.size()) + " items.");
	}

// End creating analytic config object

	// Init Analytic plugin
	if (!pAnalytic->init(sAnalyticPluginDirLocation, mConfigFiles, mFrameGrabbers, analytic::util::log::Loggers::getDefaultLogger()))
	{
		analytic::util::log::Loggers::getDefaultLogger()->error("Analytic plugin init failed.");
		return -1;
	}

	analytic::util::log::Loggers::getDefaultLogger()->info("Init Analytic plugin done.");

	// Creating threads
	boost::thread_group threadGroup;
	analytic::ConsumerThread resultsConsumer(pOutputResultQueue, pSender, new opencctv::util::serialization::ProtoBuf());
	boost::thread *pConsumerThread = new boost::thread(resultsConsumer);

	if (pConsumerThread->joinable())
	{
		//threadGroup.add_thread(pProducerThread);
		threadGroup.add_thread(pConsumerThread);
	}

	//}
	analytic::util::log::Loggers::getDefaultLogger()->info("Creating threads done.");

	// Starting Analytic plugin
	analytic::util::log::Loggers::getDefaultLogger()->info("Starting Analytic plugin...");

	pAnalytic->process(pOutputResultQueue);

	return 0;
}

void exitHandler(int iSignum)
{
    // clear
        //  pSender
        // _pSerializer
        // ConsumerThread
    
	std::map<std::string , api::IFrameGrabber *>::iterator it;

	for (it = mFrameGrabbers.begin(); it != mFrameGrabbers.end(); ++it)
	{
		if (it->second)
		{
			delete it->second;
			mFrameGrabbers.erase(it);
		}

	}

	analytic::util::log::Loggers::getDefaultLogger()->info("This analytic instance resources were release from exitHandler before the analytic instance was terminated.");
	exit(iSignum);
}
