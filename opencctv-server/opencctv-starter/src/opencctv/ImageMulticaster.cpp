#include "ImageMulticaster.hpp"

namespace opencctv {

ImageMulticaster::ImageMulticaster(unsigned int iStreamId) {
	_bEnable = false;
	_iStreamId = iStreamId;
	_pSerializer = util::serialization::Serializers::getInstanceOfDefaultSerializer();

	int iAnalyticServerId = 1;
	//analytic::AnalyticServerManager* pASM = ;
	_pAS = analytic::AnalyticServerManager::getInstance()->getAnalyticServer(iAnalyticServerId);

	int iOpenCCTVServerId = 1;
	//OpenCCTVServerManager* pOSM = OpenCCTVServerManager::getInstance();
	_pOS = OpenCCTVServerManager::getInstance()->getOpenCCTVServer(iOpenCCTVServerId);

}

void ImageMulticaster::start() {

	// set up OpenCCTV and Analytic server
	int iAnalyticServerId = 1;
	//analytic::AnalyticServerManager* pASM = ;
	_pAS = analytic::AnalyticServerManager::getInstance()->getAnalyticServer(iAnalyticServerId);

	if (!_pAS) {
		opencctv::util::log::Loggers::getDefaultLogger()->error("Multicast: Cannot get _pAS");
	}

	int iOpenCCTVServerId = 1;
	//OpenCCTVServerManager* pOSM = OpenCCTVServerManager::getInstance();
	_pOS = OpenCCTVServerManager::getInstance()->getOpenCCTVServer(iOpenCCTVServerId);

	if (!_pOS) {
		opencctv::util::log::Loggers::getDefaultLogger()->error("Multicast: Cannot get _pOS");
	}

	ConcurrentQueue<Image>* pQueue = NULL;

	StreamData* pSD = _pOS->getStreamData(_iStreamId);

	if (pSD && pSD->isInternalQueue()) {
		pQueue = pSD->getInternalQueue();
	}

	if (pQueue && _pSerializer) {
		_bEnable = true;
		std::stringstream ssMsg;
		ssMsg << "Image Multicaster " << _iStreamId << " started.";
		opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
	}
	while (_bEnable) {

		//opencctv::util::log::Loggers::getDefaultLogger()->debug(boost::lexical_cast<std::string>(_bEnable));

		Image* pImage = NULL;
		unsigned long long lProducedTime = pQueue->waitAndGetFrontElement(pImage);
		if (pImage) {
			std::map<unsigned int, MulticastData>::iterator it;
			// for each Analytic Input queue/Analytic Instance Stream
			for (it = _mMulticastDests.begin(); it != _mMulticastDests.end(); ++it) {
				unsigned int iAnalyticInstanceId = it->first;

				opencctv::util::log::Loggers::getDefaultLogger()->debug("Multicaster: analytic id: " + boost::lexical_cast<std::string>(it->first));

				// if Flow Controller available
				analytic::AnalyticData* pAD = _pAS->getAnalyticData(iAnalyticInstanceId);
				if (pAD and pAD->isFlowController()) {
					util::flow::FlowController* pFlowController = pAD->getFlowController();
					// if Flow Controller allows to send
					if (pFlowController->canSendImageGeneratedAt(lProducedTime)) {
						//unsigned int iId = it->second.pSender; // Analytic Instance Stream ID
						// check if MQ Sender available
						if (it->second.pSender) {
							mq::Sender* pSender = it->second.pSender;
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

		//}catch (boost::thread_interrupted&) {
		//	opencctv::util::log::Loggers::getDefaultLogger()->error("Consumer Thread interrupted.");
		//}
	}
}

bool ImageMulticaster::send(mq::Sender* pMqSender, Image* pImage) {
	bool bSent = false;
	if (_pSerializer) {
		std::string* pSSerializedImage = _pSerializer->serialize(pImage);
		try {
			if (pMqSender->send(pSSerializedImage)) {
				bSent = true;
				std::stringstream ssMsg;
				ssMsg << "Image " << pImage->getTimestamp() << " sent. VM used = " << util::Util::getCurrentVmUsageKb() << " Kb";
				opencctv::util::log::Loggers::getDefaultLogger()->debug(ssMsg.str());
			} else {
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
		if (pSSerializedImage)
			delete pSSerializedImage;
	}
	return bSent;
}

void ImageMulticaster::addDestination(dto::AnalyticInstanceStream& analyticInstanceStream) {
	//ApplicationModel* pModel = ApplicationModel::getInstance();

	analytic::AnalyticData* pAD = _pAS->getAnalyticData(analyticInstanceStream.getAnalyticInstanceId());

	if (pAD and pAD->isAnalyticQueueInAddress()) {
		mq::TcpMqSender* pSender = new mq::TcpMqSender();
		bool bConnectedToMq = false;
		try {
			bConnectedToMq = pSender->connectToMq(util::Config::getInstance()->get(util::PROPERTY_ANALYTIC_SERVER_IP), pAD->getAnalyticQueueInAddress());
			//pModel->getMulticastDestinations()[analyticInstanceStream.getId()] = pSender;
		} catch (Exception &e) {
			std::stringstream ssErrMsg;
			ssErrMsg << "Failed to connect to Input Image Queue of Analytic Instance ";
			ssErrMsg << analyticInstanceStream.getAnalyticInstanceId() << ". ";
			ssErrMsg << e.what();
			throw Exception(ssErrMsg.str());
		}
		if (bConnectedToMq) {
			//std::string inputAname = analyticInstanceStream.getInputName();
			MulticastData multicastData;
			multicastData.pSender = pSender;
			multicastData.sInputName = analyticInstanceStream.getInputName();

			_mMulticastDests[analyticInstanceStream.getAnalyticInstanceId()] = multicastData;
			std::stringstream ssMsg;
			ssMsg << "Connection established to Input Image Queue of Analytic Instance ";
			ssMsg << analyticInstanceStream.getAnalyticInstanceId() << ". ";
			util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
		} else {
			if (pSender)
				delete pSender;
			std::stringstream ssErrMsg;
			ssErrMsg << "Failed to connect to Input Image Queue of Analytic Instance ";
			ssErrMsg << analyticInstanceStream.getAnalyticInstanceId() << ". ";
			util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
		}
	}
}

size_t ImageMulticaster::getNumberOfDestinations() {
	return _mMulticastDests.size();
}

void ImageMulticaster::stop() {
	_bEnable = false;
}

const bool& ImageMulticaster::isStart() {
	return _bEnable;
}

void ImageMulticaster::removeElement(unsigned int iAnalyticInstanceStreamId) {
	// TODO:: Remove analytic instance stream id when stopping an analytic
}

ImageMulticaster::~ImageMulticaster() {
	// Delete pSender
	// Delete _pSerializer
	// pImage

	std::cout << "6. ImageMulticaster destructor is called" << std::endl;
}

} /* namespace opencctv */
