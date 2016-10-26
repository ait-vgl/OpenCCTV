
#include "ImageMulticaster.hpp"

namespace opencctv {

ImageMulticaster::ImageMulticaster(unsigned int iStreamId) {
	_bEnable = false;
	_iStreamId = iStreamId;
	_pSerializer = util::serialization::Serializers::getInstanceOfDefaultSerializer();
}

void ImageMulticaster::start() {
	ApplicationModel* pModel = ApplicationModel::getInstance();
	ConcurrentQueue<Image>* pQueue;
	if(pModel->containsInternalQueue(_iStreamId))
	{
		pQueue = pModel->getInternalQueues()[_iStreamId];
	}
	if(pQueue && _pSerializer)
	{
		_bEnable = true;
		std::stringstream ssMsg;
		ssMsg << "Image Multicaster " << _iStreamId << " started.";
		opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
	}
	while (_bEnable) {
		Image* pImage = NULL;
		unsigned long long lProducedTime = pQueue->waitAndGetFrontElement(pImage);
		if(pImage) {
			std::map<unsigned int, Element>::iterator it;
			// for each Analytic Input queue/Analytic Instance Stream
			for (it = _mAISInfo.begin(); it != _mAISInfo.end(); ++it) {
				unsigned int iAnalyticInstanceId = it->second.iAnalyticInstanceId;
				// if Flow Controller available
				if (pModel->containsFlowController(iAnalyticInstanceId)) {
					util::flow::FlowController* pFlowController = pModel->getFlowControllers()[iAnalyticInstanceId];
					// if Flow Controller allows to send
					if (pFlowController->canSendImageGeneratedAt(lProducedTime)) {
						unsigned int iId = it->first; // AnalyticInstanceStream ID
						// check if MQ Sender available
						if (pModel->containsMulticastDestination(iId)) {
							mq::Sender* pSender = pModel->getMulticastDestinations()[iId];
							if (pSender) {
								pImage->setStreamId(_iStreamId);
								pImage->setInputName(it->second.sInputName);
								// send to Analytic Input queue
								if (send(pSender, pImage)) {
									pFlowController->sent(pImage, lProducedTime);
								}
							}
						}
					}
				}
			}
		}
		pQueue->tryRemoveFrontElement();
	}
}

bool ImageMulticaster::send(mq::Sender* pMqSender, Image* pImage) {
	bool bSent = false;
	if(_pSerializer)
	{
		std::string* pSSerializedImage = _pSerializer->serialize(pImage);
		try {
			if(pMqSender->send(pSSerializedImage))
			{
				bSent = true;
				std::stringstream ssMsg;
				ssMsg << "Image " << pImage->getTimestamp() << " sent. VM used = "
						<< util::Util::getCurrentVmUsageKb() << " Kb";
				opencctv::util::log::Loggers::getDefaultLogger()->debug(ssMsg.str());
			}
			else
			{
				std::string sErrMsg = "Image: ";
				sErrMsg.append(pImage->getTimestamp());
				sErrMsg.append(" sending failed.");
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}
		} catch (std::exception &e) {
			std::string sErrMsg = "Failed to send serialized Image. ";
			sErrMsg.append(e.what());
			util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			return false;
		}
		if(pSSerializedImage) delete pSSerializedImage;
	}
	return bSent;
}

void ImageMulticaster::addDestination(const dto::AnalyticInstanceStream& analyticInstance)
{
	ApplicationModel* pModel = ApplicationModel::getInstance();
	if(pModel->containsImageInputQueueAddress(analyticInstance.getAnalyticInstanceId()))
	{
		mq::TcpMqSender* pSender = new mq::TcpMqSender();
		bool bConnectedToMq = false;
		try {
			bConnectedToMq = pSender->connectToMq(util::Config::getInstance()->get(util::PROPERTY_ANALYTIC_SERVER_IP),
					pModel->getImageInputQueueAddresses()[analyticInstance.getAnalyticInstanceId()]);
			pModel->getMulticastDestinations()[analyticInstance.getId()] = pSender;
		} catch (Exception &e) {
			std::stringstream ssErrMsg;
			ssErrMsg << "Failed to connect to Input Image Queue of Analytic Instance ";
			ssErrMsg << analyticInstance.getAnalyticInstanceId() << ". ";
			ssErrMsg << e.what();
			throw Exception(ssErrMsg.str());
		}
		if(bConnectedToMq)
		{
			Element e = {analyticInstance.getAnalyticInstanceId(), analyticInstance.getInputName()};
			_mAISInfo[analyticInstance.getId()] = e;
			std::stringstream ssMsg;
			ssMsg << "Connection established to Input Image Queue of Analytic Instance ";
			ssMsg << analyticInstance.getAnalyticInstanceId() << ". ";
			util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
		}
		else
		{
			if(pSender) delete pSender;
			std::stringstream ssErrMsg;
			ssErrMsg << "Failed to connect to Input Image Queue of Analytic Instance ";
			ssErrMsg << analyticInstance.getAnalyticInstanceId() << ". ";
			util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
		}
	}
}

size_t ImageMulticaster::getNumberOfDestinations()
{
	return _mAISInfo.size();
}

void ImageMulticaster::stop() {
	_bEnable = false;
}

ImageMulticaster::~ImageMulticaster() {
}

} /* namespace opencctv */
