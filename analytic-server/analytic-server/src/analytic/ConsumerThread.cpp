
#include "ConsumerThread.hpp"

namespace analytic {

ConsumerThread::ConsumerThread(ConcurrentQueue<api::Image_t>* pResultsOutputQueue, opencctv::mq::Sender* pSender, opencctv::util::serialization::Serializable* pSerializer) {
	_pResultsOutputQueue = pResultsOutputQueue;
	_pSender = pSender;
	_pSerializer = pSerializer;
}

void ConsumerThread::operator()()
{
	util::log::Loggers::getDefaultLogger()->info("Analytic consumer thread started.");
	while(_pResultsOutputQueue && _pSender && _pSerializer)
	{
       
        util::log::Loggers::getDefaultLogger()->debug("Start sending result");
		api::Image_t outputImage = _pResultsOutputQueue->pop();
        
        util::log::Loggers::getDefaultLogger()->debug("Result Timestamp: " + outputImage.sInputName + outputImage.sTimestamp + outputImage.sCustomTextResult);
		AnalyticResult result(outputImage.iStreamId, outputImage.sInputName, outputImage.sTimestamp, outputImage.sCustomTextResult, outputImage.bGenerateAnalyticEvent);
		std::stringstream ssMsg;
		
		
		//Writing the result to the analytic output queue
		std::string sSerializedResult = _pSerializer->serialize(result);

         util::log::Loggers::getDefaultLogger()->debug("Serialized data: " + sSerializedResult);
         
         
        // analytic::AnalyticResult resultT = _pSerializer->deserializeAnalyticResult(sSerializedResult);
         
        // util::log::Loggers::getDefaultLogger()->debug("After deserializeAnalyticResult");
			std::stringstream sMsg;
			
		//	sMsg << "\t\tReceived Result of ";
          //  sMsg << resultT.getStreamId();
		//	sMsg << resultT.getTimestamp();
		//	util::log::Loggers::getDefaultLogger()->debug(sMsg.str());
			
         
         
		if(_pSender->send(&sSerializedResult))
        {
            ssMsg << "Got violation " << result.getTimestamp() << ". Sent data size = " << opencctv::util::Util::getCurrentVmUsageKb() << " Kb";
            util::log::Loggers::getDefaultLogger()->debug(ssMsg.str());
            
        }else
        {
            ssMsg << "Cannot send result data with port: " << _pResultsOutputQueue;
            util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
        }
    
        
        //if(outputImage.matImage)
        //outputImage.matImage.release();
	}
}

} /* namespace analytic */
