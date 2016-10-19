
#include "ConsumerThread.hpp"

namespace opencctv {

ConsumerThread::ConsumerThread(unsigned int iStreamId, ImageMulticaster* pImageMulticaster) {

	int _iOpenCCTVServerId = 1;
	_pQueue = NULL;

	OpenCCTVServer* pOS = OpenCCTVServerManager::getInstance()->getOpenCCTVServer(_iOpenCCTVServerId);

	if(pOS){
		StreamData* pSD = pOS->getStreamData(iStreamId);

		if(pSD){
			_pQueue = pSD->getInternalQueue();
		}else{
			opencctv::util::log::Loggers::getDefaultLogger()->error("Consumer: pSD is empty.");
		}
	}else{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Consumer: _pOS is empty.");
	}

	_pImageMulticaster = pImageMulticaster;
}

void ConsumerThread::operator ()()
{
	if(_pImageMulticaster && _pQueue)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->info("Consumer Thread started.");
		_pImageMulticaster->start();
	}else{
		opencctv::util::log::Loggers::getDefaultLogger()->error("ImageMulticaster or Queue is empty.");

	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Consumer Thread stopped.");
}

ConsumerThread::~ConsumerThread(){

	// delete ?
	// _pImageMulticaster
	// _pQueue ==> in pModel

	std::cout << "1. Consumer destructor called." << std::endl;

	// TODO: delete Multicater


	if(_pImageMulticaster && _pImageMulticaster->isStart()){

		//_pImageMulticaster->stop();

		//if(_pImageMulticaster){
			//delete _pImageMulticaster;
			//_pImageMulticaster = NULL;
		//}
		std::cout << "1.1 delete _pImageMulticaster." << std::endl;
	}
}

} /* namespace opencctv */
