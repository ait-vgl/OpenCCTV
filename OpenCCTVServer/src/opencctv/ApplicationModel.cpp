#include "ApplicationModel.hpp"

namespace opencctv {

ApplicationModel* ApplicationModel::_pModel = NULL;

ApplicationModel* ApplicationModel::getInstance() {
	if (!_pModel) {
		_pModel = new ApplicationModel();
	}
	return _pModel;
}

ApplicationModel::ApplicationModel() {
}

bool ApplicationModel::containsImageInputQueueAddress(unsigned int iAnalyticInstanceId)
{
	std::map<unsigned int, std::string>::iterator it = _mImageInputQueueAddresses.find(iAnalyticInstanceId);
	if (it != _mImageInputQueueAddresses.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsResultsOutputQueueAddress(unsigned int iAnalyticInstanceId)
{
	std::map<unsigned int, std::string>::iterator it = _mResultsOutputQueueAddresses.find(iAnalyticInstanceId);
	if (it != _mResultsOutputQueueAddresses.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsFlowController(unsigned int iAnalyticInstanceId)
{
	std::map<unsigned int, util::flow::FlowController*>::iterator it = _mFlowControllers.find(iAnalyticInstanceId);
	if(it != _mFlowControllers.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsMulticastDestination(unsigned int iId)
{
	std::map<unsigned int, mq::Sender*>::iterator it = _mMulticastDestinations.find(iId);
	if(it != _mMulticastDestinations.end())
	{
		return true;
	}
	return false;
}

bool ApplicationModel::containsVmsPluginLoader(unsigned int iVmsTypeId)
{
	std::map<unsigned int, PluginLoader<api::VmsConnector>*>::iterator it = _mVmsPluginLoaders.find(iVmsTypeId);
	if(it != _mVmsPluginLoaders.end())
	{
		return true;
	}
	return false;
}

bool ApplicationModel::containsInternalQueue(unsigned int iStreamId)
{
	std::map<unsigned int, ConcurrentQueue<Image>*>::iterator it = _mInternalQueues.find(iStreamId);
	if(it != _mInternalQueues.end())
	{
		return true;
	}
	return false;
}

bool ApplicationModel::containsAnalyticInstanceManager(unsigned int iAnalyticServerId)
{
	std::map<unsigned int, analytic::AnalyticInstanceManager*>::iterator it = _mAnalyticInstanceManagers.find(iAnalyticServerId);
	if (it != _mAnalyticInstanceManagers.end()) {
		return true;
	}
	return false;
}

std::map<unsigned int, std::string>& ApplicationModel::getImageInputQueueAddresses()
{
	return _mImageInputQueueAddresses;
}

std::map<unsigned int, std::string>& ApplicationModel::getResultsOutputQueueAddresses()
{
	return _mResultsOutputQueueAddresses;
}

std::map<unsigned int, util::flow::FlowController*>& ApplicationModel::getFlowControllers()
{
	return _mFlowControllers;
}

std::map<unsigned int, mq::Sender*>& ApplicationModel::getMulticastDestinations()
{
	return _mMulticastDestinations;
}

std::map<unsigned int, PluginLoader<api::VmsConnector>*>& ApplicationModel::getVmsPluginLoaders()
{
	return _mVmsPluginLoaders;
}

std::map<unsigned int, ConcurrentQueue<Image>*>& ApplicationModel::getInternalQueues()
{
	return _mInternalQueues;
}

std::map<unsigned int, analytic::AnalyticInstanceManager*>& ApplicationModel::getAnalyticInstanceManagers()
{
	return _mAnalyticInstanceManagers;
}

ApplicationModel::~ApplicationModel() {
	if (_pModel) {
		delete _pModel;
	}
}

} /* namespace opencctv */
