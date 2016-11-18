#include "ApplicationModel.hpp"

namespace analytic {

ApplicationModel* ApplicationModel::_pModel = NULL;

ApplicationModel* ApplicationModel::getInstance() {
	if (!_pModel) {
		_pModel = new ApplicationModel();
	}
	return _pModel;
}

bool ApplicationModel::containsAnalyticProcess(unsigned int iAnalyticInstanceId) {
	std::map<unsigned int, analytic::AnalyticProcess*>::iterator it = _mAnalyticProcesses.find(iAnalyticInstanceId);
	if (it != _mAnalyticProcesses.end()) {
		return true;
	}
	return false;
}

ApplicationModel::ApplicationModel() {
}

std::map<unsigned int, analytic::AnalyticProcess*>& ApplicationModel::getAnalyticProcesses() {
	return _mAnalyticProcesses;
}

ApplicationModel::~ApplicationModel() {
}

} /* namespace analytic */
