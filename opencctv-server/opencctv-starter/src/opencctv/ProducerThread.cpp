
#include "ProducerThread.hpp"

namespace opencctv {

ProducerThread::ProducerThread(unsigned int iStreamId, opencctv::api::VmsConnector* pVmsConn) {
	ApplicationModel* pModel = ApplicationModel::getInstance();
	_pQueue = NULL;
	if(pModel->containsInternalQueue(iStreamId))
	{
		_pQueue = pModel->getInternalQueues()[iStreamId];
	}
	_pPluginLoader = NULL;
	if(pModel->containsVmsPluginLoader(iStreamId))
	{
		_pPluginLoader = pModel->getVmsPluginLoaders()[iStreamId];
	}
	_pVmsConn = pVmsConn;
	_bActive = false;
}

void ProducerThread::operator ()()
{
	if(_pVmsConn && _pQueue)
	{
		_bActive = true;
		opencctv::util::log::Loggers::getDefaultLogger()->info("Producer Thread started.");
		try
		{
				_pVmsConn->produceImageObjects(_pQueue);

		}
		catch(std::exception &e)
		{
			_bActive = false;
			std::string sErrMsg = "VMS Connector Plugin's produceImageObjects() function failed. ";
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Producer Thread stopped.");
}

bool ProducerThread::isStillRunning()
{
	bool bVmsConnectorIsActive = false;
	try
	{
		bVmsConnectorIsActive = _pVmsConn->isStillProducingImages();
	}
	catch(std::exception &e)
	{
		throw Exception(e.what());
	}
	return (bVmsConnectorIsActive && _bActive);
}

ProducerThread::~ProducerThread(){

	// Delete ?
	// _pVmsConn (Interface Vms Connectore)
	// _pQueue => in pModel
	// _pPluginLoader


	std::cout << "2. Producer Thread : destructor called" << std::endl;

	if(isStillRunning()){

		if(_pVmsConn){

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
