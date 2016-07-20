
#include "ProducerThread.hpp"

namespace analytic {

ProducerThread::ProducerThread(ConcurrentQueue<api::Image_t>* pImageInputQueue, opencctv::mq::Receiver* pReceiver, opencctv::util::serialization::Serializable* pSerializer) {
	_pImageInputQueue = pImageInputQueue;
	_pReceiver = pReceiver;
	_pSerializer = pSerializer;
}

void ProducerThread::operator()()
{
	opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic producer thread started.");
	while(true)
	{
		//Read serialized Image object
		std::string* pSSerializedImage = _pReceiver->receive();
		// De-serialize Image
		opencctv::Image* pImage = _pSerializer->deserializeImage(pSSerializedImage);
		if(pSSerializedImage) delete pSSerializedImage;
		//Image object to OpenCV Mat
		cv::Mat matImage;
		util::ImageUtil::toOpenCvMat(pImage, matImage);
		api::Image_t inputImage = {matImage, pImage->getStreamId(), pImage->getInputName(), pImage->getTimestamp(), std::string(), false};
		if(pImage) delete pImage;
		_pImageInputQueue->push(inputImage);
	}
}

} /* namespace analytic */
