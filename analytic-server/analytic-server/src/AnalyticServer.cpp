#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <signal.h> // to handle terminate signal
#include <boost/thread/thread.hpp> // -lboost_thread -pthread


#include "opencctv/mq/TcpMqReceiver.hpp"
#include "opencctv/Exception.hpp"
#include "opencctv/mq/TcpMqSender.hpp"
#include "opencctv/PluginLoader.hpp"
//#include "opencctv/util/serialization/gpb/ProtoBuf.hpp"
#include "opencctv/util/log/Loggers.hpp"
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

//#include "FrameGrabberWrapper.h"

#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

using namespace opencctv;
using namespace analytic;

void exitHandler(int iSignum);
std::map<string, FrameGrabber *> mFrameGrabbers;

int main(int argc, char *argv[])
{
    // Registering signal SIGINT and signal handler
	signal(SIGINT, exitHandler); // for Ctrl + C keyboard interrupt
	signal(SIGTERM, exitHandler); // for Terminate signal
    signal(SIGKILL, exitHandler); // for SIGKILL
    
	if (argc < 4)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Invalid number of arguments.");
		return -1;
	}

	// Sending PID of Analytic process to Analytic Starter process through stdout
	//fprintf(stdout, "%s", opencctv::util::Util::getPidMessage(getpid()).c_str());
	fprintf(stdout, "%s", opencctv::util::Util::getPidMessage(getpid()).c_str());
    fflush(stdout);
   
	// Saving input arguments
    std::string sAnalyticInstanceId = argv[1];
    std::string sAnalyticPluginFilename = argv[2];
    std::string sAnalyticsResultsDir = argv[3];
    std::string sAnalyticPluginDirLocation = "";

	//Check if analytic results directory is writable
	if(access(sAnalyticsResultsDir.c_str(), W_OK)!= 0)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Analytic instance - "+ sAnalyticInstanceId +" : Analytic results directory is not writable.");
		return -1;
	}

	// Loading analytic plugin
	opencctv::PluginLoader<analytic::api::Analytic> analyticLoader;
	analytic::api::Analytic *pAnalytic = NULL;

	try
	{
		sAnalyticPluginDirLocation = analytic::util::Config::getInstance()->get(analytic::util::PROPERTY_ANALYTIC_PLUGIN_DIR);

		if (*sAnalyticPluginDirLocation.rbegin() != '/')     // check last char
		{
			sAnalyticPluginDirLocation.append("/");
		}
		sAnalyticPluginDirLocation.append(sAnalyticPluginFilename);
		std::string sAnalyticPluginPath;
		opencctv::util::Util::findSharedLibOfPlugin(sAnalyticPluginDirLocation, sAnalyticPluginPath);
		//std::cout << "Analytic plugin path = " << sAnalyticPluginPath << std::endl;
		analyticLoader.loadPlugin(sAnalyticPluginPath);
		pAnalytic = analyticLoader.createPluginInstance();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Analytic instance - "+ sAnalyticInstanceId + " : Failed to load analytic plugin. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	if (!pAnalytic)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Analytic instance - "+ sAnalyticInstanceId +" : Loaded analytic is NULL.");
		return -1;
	}

	opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance - "+ sAnalyticInstanceId +" : Loading Analytic plugin done.");

	// Preparing analytic instance input streams
	analytic::db::AnalyticInstanceStreamGateway *pAnalyticInstanceStreamGateway = NULL;

	try
	{
		pAnalyticInstanceStreamGateway = new analytic::db::AnalyticInstanceStreamGateway();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Analytic instance - "+ sAnalyticInstanceId +" : Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	std::vector<analytic::dto::AnalyticInstanceStream> vAnalyticInstanceStream;

	try
	{
		pAnalyticInstanceStreamGateway->getAnalyticInstanceStreams(boost::lexical_cast<unsigned int> (sAnalyticInstanceId), vAnalyticInstanceStream);
		opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance - "+ sAnalyticInstanceId +" : Analytic instance stream loaded.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Analytic instance - "+ sAnalyticInstanceId +" : Failed to retrieve video stream details of analytic instance " + sAnalyticInstanceId + ". ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}
	// End preparing analytic instance input stream 	//


	// Preparing analytic instance configuration details
	analytic::db::AnalyticInstanceConfigGateway *pAnalyticInstanceConfigGateway = NULL;

	try
	{
		pAnalyticInstanceConfigGateway = new analytic::db::AnalyticInstanceConfigGateway();
        //opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance configuration details retrieved.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Analytic instance - "+ sAnalyticInstanceId +" : Failed to create AnalyticInstanceConfigGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	std::vector<analytic::dto::AnalyticInstanceConfig> vAnalyticInstaceConfig;

	try
	{
       
		pAnalyticInstanceConfigGateway->getAnalyticInstanceConfig(boost::lexical_cast<unsigned int> (sAnalyticInstanceId), vAnalyticInstaceConfig);
		//opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance config loaded, with " + boost::lexical_cast<std::string>(vAnalyticInstaceConfig.size()) + "config files." );
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Analytic instance - "+ sAnalyticInstanceId +" : Failed to retrieve analytic configuration details.";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}
	// End preparing analytic instance config

	//opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance - "+ sAnalyticInstanceId +" : Starting configuring FrameGrabber, with " + vAnalyticInstanceStream.size() + " channels.");

	// Creating FrameGrabber object
	if (!vAnalyticInstanceStream.empty())
	{
		for (int i = 0; i < vAnalyticInstanceStream.size(); i++)
		{
			analytic::dto::AnalyticInstanceStream AIS = vAnalyticInstanceStream[i];

			FrameGrabberWrapper *pFGW =  new FrameGrabberWrapper();
			analytic::api::IFrameGrabberWrapper *pIFGW = pFGW;
			pIFGW->init();

			FileStorage config(AIS.getConfig(), FileStorage::READ + FileStorage::MEMORY);
			pIFGW->setDefaultConfig(config);

			//TODO Work with LiLin VMS and LiLin cameras. Will this URL format work for other camera types???
			vector<string> strs;
			boost::split(strs, AIS.getURL(), boost::is_any_of("/"), boost::token_compress_on);
             string sUrl;
             for (int i =0 ; i < strs.size() ; i++){
                std::cout << i << " " <<  strs[i] <<std::endl;
                if (i==0) 
                    sUrl = strs[i] + "//" + AIS.getUsername() + ":" + AIS.getPassword() + "@";
                else if (i==1)
                    sUrl +=  strs[i];
                else 
                    sUrl += "/" + strs[i];
             }

			int iWidth = AIS.getWidth();
			int iHeight = AIS.getHeight();
			int iFps = AIS.getFps();
			if(iFps <= 0)
			{
				iFps = 15; //Default FPS = 15
			}
			
			opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance - "+ sAnalyticInstanceId +" : Set streamConfig " + sUrl + " " + boost::lexical_cast<std::string>(iWidth) + " " + boost::lexical_cast<std::string>(iHeight) + " " + boost::lexical_cast<std::string>(iFps));
			pIFGW->setStreamConfig(sUrl, iWidth, iHeight, iFps);

			mFrameGrabbers[AIS.getInputName()] = pIFGW->getFrameGrabberObj();
		}

		opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance - "+ sAnalyticInstanceId +" : Configuring FrameGrabber objects done.");
	}
	// End creating FrameGrabber object

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

		opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance - "+ sAnalyticInstanceId +" :  Setting analytic instance configurations done.");
	}
	// End creating analytic config object

	// Init Analytic plugin
	if (!pAnalytic->init(mConfigFiles, mFrameGrabbers))
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Analytic instance - "+ sAnalyticInstanceId +" : Analytic plugin init failed.");
		return -1;
	}

	opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance - "+ sAnalyticInstanceId +" : Init Analytic plugin done.");

	// Creating threads
	analytic::ConcurrentQueue<analytic::api::Results_t> *pOutputResultQueue = new analytic::ConcurrentQueue<analytic::api::Results_t> (5);
	/*analytic::ImageQueue<analytic::api::Image_t>* pInputImageQueue = new analytic::ImageQueue<analytic::api::Image_t>(5);
	analytic::ImageQueue<analytic::api::Image_t>* pOutputResultQueue = new analytic::ImageQueue<analytic::api::Image_t>(5);*/
	//opencctv::util::log::Loggers::getDefaultLogger()->info("Creating output queue done.");

	boost::thread_group threadGroup;
	//analytic::ConsumerThread resultsConsumer(pOutputResultQueue, pSender, new opencctv::util::serialization::ProtoBuf());
	analytic::ConsumerThread resultsConsumer(pOutputResultQueue);
	//boost::thread *pConsumerThread = new boost::thread(resultsConsumer);

	/*if (pConsumerThread->joinable())
	{
		//threadGroup.add_thread(pProducerThread);
		threadGroup.add_thread(pConsumerThread);
	}*/

	//}
	//opencctv::util::log::Loggers::getDefaultLogger()->info("Creating threads done.");


	// Starting Analytic plugin
	opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance - "+ sAnalyticInstanceId +" : Starting Analytic plugin...");

	pAnalytic->process(sAnalyticsResultsDir, pOutputResultQueue);

	return 0;
}

void exitHandler(int iSignum)
{
	std::map<std::string , FrameGrabber *>::iterator it;

	for (it = mFrameGrabbers.begin(); it != mFrameGrabbers.end(); /*++it*/)//Iterator increased below
	{
		std::map<std::string , FrameGrabber *>::iterator itErase = it;
		++it;
		if(itErase->second)
        {
            delete itErase->second;  itErase->second = NULL;
            mFrameGrabbers.erase(itErase);
        }
    }
    //opencctv::util::log::Loggers::getDefaultLogger()->info("This analytic instance resources were release from exitHandler before the analytic instance was terminated.");
	opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic instance terminated.");
    exit(iSignum);
}
