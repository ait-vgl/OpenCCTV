#include "ApplicationModel.hpp"

namespace analytic
{

ApplicationModel* ApplicationModel::_pModel = NULL;

ApplicationModel* ApplicationModel::getInstance()
{
	if (!_pModel)
	{
		_pModel = new ApplicationModel();
	}
	return _pModel;
}

bool ApplicationModel::containsAnalyticProcess(unsigned int iAnalyticInstanceId)
{
	std::map<unsigned int, analytic::AnalyticProcess*>::iterator it = _mAnalyticProcesses.find(iAnalyticInstanceId);
	if (it != _mAnalyticProcesses.end()) {
		return true;
	}
	return false;
}

ApplicationModel::ApplicationModel()
{
	_pResultsTxThreadGroup = new boost::thread_group();
	boost::thread_group* _pResultsTxThreadGroup = new boost::thread_group();
	//_pResultsTxThread = NULL;
}

bool ApplicationModel::containsRAppPluginLoader(unsigned int iRAppConnectorId)
{
	std::map<unsigned int, analytic::util::PluginLoader<result::api::ResultsAppConnector>*>::iterator it = _mRAppPluginLoaders.find(iRAppConnectorId);
	if(it != _mRAppPluginLoaders.end())
	{
		return true;
	}
	return false;
}

bool ApplicationModel::containsRAppConnector(unsigned int iRAppInstanceId)
{
	std::map<unsigned int, std::pair <unsigned int,result::api::ResultsAppConnector*> >::iterator it = _mRAppConnectors.find(iRAppInstanceId);
	if (it != _mRAppConnectors.end()) {
		return true;
	}
	return false;

}

std::map<unsigned int, analytic::AnalyticProcess*>& ApplicationModel::getAnalyticProcesses()
{
	return _mAnalyticProcesses;
}

std::map<unsigned int, analytic::util::PluginLoader<result::api::ResultsAppConnector>*>& ApplicationModel::getRAppPluginLoaders()
{
	return _mRAppPluginLoaders;
}

std::map<unsigned int,std::pair<unsigned int, result::api::ResultsAppConnector*> >& ApplicationModel::getRAppConnectors()
{
	return _mRAppConnectors;
}

boost::thread_group*& ApplicationModel::getResultsTxThreadGroup()
{
	return _pResultsTxThreadGroup;
}

bool ApplicationModel::containsResultsTxThread(unsigned int iResultsAppInstId)
{
	std::map<unsigned int, boost::thread*>::iterator it = _mResultsTxThreads.find(iResultsAppInstId);
	if (it != _mResultsTxThreads.end()) {
		return true;
	}
	return false;
}

std::map<unsigned int, boost::thread*>& ApplicationModel::getResultsTxThreads()
{
	return _mResultsTxThreads;
}

bool ApplicationModel::containsResultAppInstController(unsigned int iResultsAppInstId)
{
	std::map<unsigned int, result::ResultAppInstController*>::iterator it = _mResultAppInstControllers.find(iResultsAppInstId);
	if (it != _mResultAppInstControllers.end()) {
		return true;
	}
	return false;
}

std::map<unsigned int, result::ResultAppInstController*>& ApplicationModel::getResultAppInstControllers()
{
	return _mResultAppInstControllers;
}

bool ApplicationModel::containsResultsAppInstAnalyticInstances(unsigned int iResultsAppInstId)
{
	std::map<unsigned int, std::vector<unsigned int> >::iterator it = _mRAppInstAnalyticInstances.find(iResultsAppInstId);
	if (it != _mRAppInstAnalyticInstances.end()) {
		return true;
	}
	return false;
}
std::map<unsigned int, std::vector<unsigned int> >& ApplicationModel::ResultsAppInstAnalyticInstances()
{
	return _mRAppInstAnalyticInstances;
}

/*void ApplicationModel::setResultsTxThreadGroup(boost::thread_group*& resultsTxThreadGroup)
{
	_pResultsTxThreadGroup = resultsTxThreadGroup;
}*/

/*boost::thread*& ApplicationModel::getResultsTxThread()
{
	return _pResultsTxThread;
}

void ApplicationModel::setResultsTxThread(boost::thread*& resultsTxThread)
{
	_pResultsTxThread = resultsTxThread;
}*/

ApplicationModel::~ApplicationModel()
{
}

} /* namespace analytic */

