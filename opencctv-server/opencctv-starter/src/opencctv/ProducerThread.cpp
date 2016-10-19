#include "ProducerThread.hpp"

namespace opencctv {

ProducerThread::ProducerThread(unsigned int iStreamId, opencctv::api::VmsConnector* pVmsConn) {

	_pQueue = NULL;
	_pImageMulticaster = NULL;

	_iOpenCCTVServerId = 1;
	OpenCCTVServer* _pOS = OpenCCTVServerManager::getInstance()->getOpenCCTVServer(_iOpenCCTVServerId);

	if (_pOS) {
		StreamData* pSD = _pOS->getStreamData(iStreamId);

		if (pSD) {
			_pQueue = pSD->getInternalQueue();
		}else{
			opencctv::util::log::Loggers::getDefaultLogger()->debug("Producer: _pSD is empty.");
		}
	}else{
		opencctv::util::log::Loggers::getDefaultLogger()->debug("Producer: _pOS is empty.");
	}

	//_pPluginLoader = NULL;
	//if(pModel->containsVmsPluginLoader(iStreamId))
	//{
	//_pPluginLoader = pModel->getVmsPluginLoaders()[iStreamId];
	//}
	_pVmsConn = pVmsConn;
	_bActive = false;
}

void ProducerThread::operator ()() {
	if (_pVmsConn && _pQueue) {
		_bActive = true;
		opencctv::util::log::Loggers::getDefaultLogger()->info("Producer Thread started.");
		try {
			_pVmsConn->produceImageObjects(_pQueue);

		} catch (std::exception &e) {
			_bActive = false;
			std::string sErrMsg = "VMS Connector Plugin's produceImageObjects() function failed. ";
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}
	} else {
		opencctv::util::log::Loggers::getDefaultLogger()->error("Producer: pVmsConn or pQueue is empty");
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Producer Thread stopped.");
}

bool ProducerThread::isStillRunning() {
	bool bVmsConnectorIsActive = false;
	try {
		bVmsConnectorIsActive = _pVmsConn->isStillProducingImages();
	} catch (std::exception &e) {
		throw Exception(e.what());
	}
	return (bVmsConnectorIsActive && _bActive);
}

ProducerThread::~ProducerThread() {

	// Delete ?
	// _pVmsConn (Interface Vms Connectore)
	// _pQueue => in pModel
	// _pPluginLoader

	std::cout << "2. Producer Thread : destructor called" << std::endl;

	if (isStillRunning()) {

		if (_pVmsConn) {

			delete _pVmsConn;
			_pVmsConn = NULL;

			_bActive = false;

			//if(_pQueue)
			//_pQueue->empty();

			std::cout << "2.1 Start delete _pVmsConn" << std::endl;
		}
	}
}

} /* namespace opencctv */
