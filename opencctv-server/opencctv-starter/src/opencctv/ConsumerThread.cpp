
#include "ConsumerThread.hpp"

namespace opencctv {

ConsumerThread::ConsumerThread(unsigned int iStreamId, ImageMulticaster* pImageMulticaster) {
	ApplicationModel* pModel = ApplicationModel::getInstance();
	_pQueue = NULL;
	if(pModel->containsInternalQueue(iStreamId))
	{
		_pQueue = pModel->getInternalQueues()[iStreamId];
	}
	_pImageMulticaster = pImageMulticaster;
}

void ConsumerThread::operator ()()
{
	if(_pImageMulticaster)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->info("Consumer Thread started.");
		_pImageMulticaster->start();
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Consumer Thread stopped.");
}

ConsumerThread::~ConsumerThread(){

	// delete ?
	// _pImageMulticaster
	// _pQueue ==> in pModel

	std::cout << "1. Consumer destructure called." << std::endl;

	// TODO: delete Multicater

/*
	if(_pImageMulticaster && _pQueue){
		delete _pImageMulticaster;
		_pImageMulticaster = NULL;

		std::cout << "1.1 delete _pImageMulticaster." << std::endl;
	}*/
}

} /* namespace opencctv */
