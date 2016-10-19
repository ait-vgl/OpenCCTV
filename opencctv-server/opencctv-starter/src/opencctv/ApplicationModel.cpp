#include "ApplicationModel.hpp"

namespace opencctv {

ApplicationModel* ApplicationModel::_pModel = NULL;

/*ApplicationModel* ApplicationModel::getInstance() {
	if (!_pModel) {
		_pModel = new ApplicationModel();
	}
	return _pModel;
}

ApplicationModel::ApplicationModel() {
}

bool ApplicationModel::containsImageInputQueueAddress(unsigned int iAnalyticInstanceId) {
	std::map<unsigned int, std::string>::iterator it = _mImageInputQueueAddresses.find(iAnalyticInstanceId);
	if (it != _mImageInputQueueAddresses.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsResultsOutputQueueAddress(unsigned int iAnalyticInstanceId) {
	std::map<unsigned int, std::string>::iterator it = _mResultsOutputQueueAddresses.find(iAnalyticInstanceId);
	if (it != _mResultsOutputQueueAddresses.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsFlowController(unsigned int iAnalyticInstanceId) {
	std::map<unsigned int, util::flow::FlowController*>::iterator it = _mFlowControllers.find(iAnalyticInstanceId);
	if (it != _mFlowControllers.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsMulticastDestination(unsigned int iId) {
	std::map<unsigned int, mq::Sender*>::iterator it = _mMulticastDestinations.find(iId);
	if (it != _mMulticastDestinations.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsVmsPluginLoader(unsigned int iVmsTypeId) {
	std::map<unsigned int, PluginLoader<api::VmsConnector>*>::iterator it = _mVmsPluginLoaders.find(iVmsTypeId);
	if (it != _mVmsPluginLoaders.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsInternalQueue(unsigned int iStreamId) {
	std::map<unsigned int, ConcurrentQueue<Image>*>::iterator it = _mInternalQueues.find(iStreamId);
	if (it != _mInternalQueues.end()) {
		return true;
	}
	return false;
}

bool ApplicationModel::containsAnalyticInstanceManager(unsigned int iAnalyticServerId) {
	std::map<unsigned int, analytic::AnalyticInstanceManager*>::iterator it = _mAnalyticInstanceManagers.find(iAnalyticServerId);
	if (it != _mAnalyticInstanceManagers.end()) {
		return true;
	}
	return false;
}

std::map<unsigned int, std::string>& ApplicationModel::getImageInputQueueAddresses() {
	return _mImageInputQueueAddresses;
}

std::map<unsigned int, std::string>& ApplicationModel::getResultsOutputQueueAddresses() {
	return _mResultsOutputQueueAddresses;
}

std::map<unsigned int, util::flow::FlowController*>& ApplicationModel::getFlowControllers() {
	return _mFlowControllers;
}

std::map<unsigned int, mq::Sender*>& ApplicationModel::getMulticastDestinations() {
	return _mMulticastDestinations;
}

std::map<unsigned int, PluginLoader<api::VmsConnector>*>& ApplicationModel::getVmsPluginLoaders() {
	return _mVmsPluginLoaders;
}

std::map<unsigned int, ConcurrentQueue<Image>*>& ApplicationModel::getInternalQueues() {
	return _mInternalQueues;
}

std::map<unsigned int, analytic::AnalyticInstanceManager*>& ApplicationModel::getAnalyticInstanceManagers() {
	return _mAnalyticInstanceManagers;
}

std::map<unsigned int, analytic::AnalyticInstanceManager*>* a = getAnalytic()
std::map<unsigned int, analytic::AnalyticInstanceManager*> a = getAnalytic()
a.


std::map<unsigned int, analytic::AnalyticInstanceManager*>& ApplicationModel::getAnalyticInstanceManagers() {
	return &_mAnalyticInstanceManagers;
}

std::map<unsigned int, std::vector<unsigned int> >& ApplicationModel::getStreamManages() {

	return _mStreamManagers;

}

void ApplicationModel::setStreamManages(unsigned int iStreamId, unsigned int analyticInstaceId) {

	_mStreamManagers[iStreamId].push_back(analyticInstaceId);

	std::cout << "Aanalytic running per stream " << iStreamId << " : " << _mStreamManagers[iStreamId].size() << std::endl;
}

void ApplicationModel::removeDataByAIId(unsigned int& iAnalyticInstanceId) { // AI = Analytic Instance

	//TODO: delete pointer before erase
	if (_mImageInputQueueAddresses.count(iAnalyticInstanceId) > 0) _mImageInputQueueAddresses.erase(iAnalyticInstanceId);

	if (_mResultsOutputQueueAddresses.count(iAnalyticInstanceId) > 0) _mResultsOutputQueueAddresses.erase(iAnalyticInstanceId);

	if (_mFlowControllers.count(iAnalyticInstanceId) > 0) _mFlowControllers.erase(iAnalyticInstanceId);

	if (_mMulticastDestinations.count(iAnalyticInstanceId) > 0) _mMulticastDestinations.erase(iAnalyticInstanceId);

}

void ApplicationModel::removeDataByAISId(unsigned int& iAnalyticStreamId){
//TODO: delete pointer before erase
	if(_mMulticastDestinations.count(iAnalyticStreamId) > 0 ) _mMulticastDestinations.erase(iAnalyticStreamId);

}

void ApplicationModel::removeDataByStreamId(unsigned int iStreamId) {
	//TODO: delete pointer before erase
	if (_mStreamManagers.count(iStreamId) > 0) _mStreamManagers.erase(iStreamId);

	if (_mInternalQueues.count(iStreamId) > 0) _mInternalQueues.erase(iStreamId);

}

bool ApplicationModel::isAnalyticRunningByStreamId(unsigned int iStreamId, unsigned int analyticInstanceId) {

	if (_mStreamManagers.count(iStreamId) > 0) {

		for (unsigned int i = 0; i <= _mStreamManagers[iStreamId].size(); i++) {
			if (_mStreamManagers[iStreamId][i] == analyticInstanceId) {
				return true;
			}
		}
	}

	return false;

}

bool ApplicationModel::isStreamRunning(unsigned int iStreamId) {
	if (_mStreamManagers.count(iStreamId) > 0) {
		return true;
	}

	return false;
}*/

ApplicationModel::~ApplicationModel() {
	if (_pModel) {
		delete _pModel;
	}
}

} /* namespace opencctv */
