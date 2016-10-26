
#include "ConsumerThread.hpp"

namespace analytic {

ConsumerThread::ConsumerThread(ConcurrentQueue<api::Image_t>* pResultsOutputQueue, opencctv::mq::Sender* pSender, opencctv::util::serialization::Serializable* pSerializer) {
	_pResultsOutputQueue = pResultsOutputQueue;
	_pSender = pSender;
	_pSerializer = pSerializer;
}

void ConsumerThread::operator()()
{
	opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic consumer thread started.");
	while(_pResultsOutputQueue && _pSender && _pSerializer)
	{
		api::Image_t outputImage = _pResultsOutputQueue->pop();
		AnalyticResult result(outputImage.iStreamId, outputImage.sInputName, outputImage.sTimestamp, outputImage.sCustomTextResult, outputImage.bGenerateAnalyticEvent);
		std::stringstream ssMsg;
		ssMsg << "Image " << result.getTimestamp() << " processed. VM used = "
				<< opencctv::util::Util::getCurrentVmUsageKb() << " Kb";
		opencctv::util::log::Loggers::getDefaultLogger()->debug(ssMsg.str());
		//Writing the result to the analytic output queue
		std::string sSerializedResult = _pSerializer->serialize(result);
		_pSender->send(&sSerializedResult);
		outputImage.matImage.release();
	}
}

} /* namespace analytic */
