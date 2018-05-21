
#include "ConsumerThread.hpp"

namespace analytic
{

ConsumerThread::ConsumerThread(unsigned int iAnalyticInstanceId, analytic::api::ResultsQueue<analytic::api::Result_t*>* pResultsOutputQueue)
{
	_iAnalyticInstanceId = iAnalyticInstanceId;
	_pResultsOutputQueue = pResultsOutputQueue;
}

void ConsumerThread::operator()()
{
	opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic consumer thread started.");
	while(_pResultsOutputQueue)
	{
		//Read from the ResultsOutputQueue
		analytic::api::Result_t* pResult = _pResultsOutputQueue->pop();

		//Save to the results database
		analytic::db::AnalyticResultsGateway analyticResultsGateway;
		int iResultId = 0;
		try
		{
			iResultId = analyticResultsGateway.insertResults(_iAnalyticInstanceId, pResult->getTimestamp(), pResult->getCustomTextResult());
		}catch(opencctv::Exception &e)
		{
			opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		}

		if(iResultId > 0)
		{
			try
			{
				//Insert results files
				const std::vector<std::string>* vResults = pResult->getVResultsFiles();
				if (vResults && !(vResults->empty()))
				{
					analyticResultsGateway.insertFiles(iResultId, vResults, "results_files");
				}

				//Insert image files
				const std::vector<std::string>* vImages = pResult->getVIamgeFiles();
				if (vImages && !(vImages->empty()))
				{
					analyticResultsGateway.insertFiles(iResultId, vImages, "images");
				}

				//Insert video files
				const std::vector<std::string>* vVideos = pResult->getVVideoFiles();
				if (vVideos && !(vVideos->empty()))
				{
					analyticResultsGateway.insertFiles(iResultId, vVideos, "videos");
				}
			} catch (opencctv::Exception &e)
			{
				opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
			}
		}

		delete pResult; pResult = NULL;
	}
}


} /* namespace analytic */
