
#ifndef ANALYTIC_APPLICATIONMODEL_HPP_
#define ANALYTIC_APPLICATIONMODEL_HPP_

#include <map>
#include <boost/thread/thread.hpp>
#include "AnalyticProcess.hpp"
#include "util/PluginLoader.hpp"
#include "../result/api/ResultsAppConnector.hpp"
#include "../result/ResultAppInstController.hpp"
//#include "../result/ResultsTxThread.hpp"

//class result::ResultAppInstController;

namespace analytic {

class ApplicationModel {
private:
	static ApplicationModel* _pModel;
	std::map<unsigned int, analytic::AnalyticProcess*> _mAnalyticProcesses; // Analytic Instance ID as key
	boost::thread_group* _pResultsTxThreadGroup;
	std::map<unsigned int, boost::thread*> _mResultsTxThreads; // Results App Instance ID as key
	std::map<unsigned int, result::ResultAppInstController*> _mResultAppInstControllers; // Results App Instance ID as key
	std::map<unsigned int, analytic::util::PluginLoader<result::api::ResultsAppConnector>*> _mRAppPluginLoaders; //Results App connector ID as key
	std::map<unsigned int, std::pair <unsigned int,result::api::ResultsAppConnector*> > _mRAppConnectors; // Results App instance ID as key and Results App connector ID in the pair
	std::map<unsigned int, std::vector<unsigned int> > _mRAppInstAnalyticInstances; // Results App Instance ID as key


	//std::map<unsigned int, std::vector<result::api::ResultsAppConnector*> > _mRegistrations; //Analytic instance ID as Key

	//boost::thread* _pResultsTxThread;

	//std::map<unsigned int, boost::thread*> _mProducerThreads; // Stream ID as key

	ApplicationModel();

public:
	static ApplicationModel* getInstance();

	//Functions for std::map<unsigned int, analytic::AnalyticProcess*> _mAnalyticProcesses;
	bool containsAnalyticProcess(unsigned int iAnalyticInstanceId);
	std::map<unsigned int, analytic::AnalyticProcess*>& getAnalyticProcesses();

	//Functions for boost::thread_group* _pResultsTxThreadGroup;
	boost::thread_group*& getResultsTxThreadGroup();

	//Functions for std::map<unsigned int, boost::thread*> _mResultsTxThreads;
	bool containsResultsTxThread(unsigned int iResultsAppInstId);
	std::map<unsigned int, boost::thread*>& getResultsTxThreads();

	//Functions for std::map<unsigned int, result::ResultAppInstController*> _mResultAppInstControllers;
	bool containsResultAppInstController(unsigned int iResultsAppInstId);
	std::map<unsigned int, result::ResultAppInstController*>& getResultAppInstControllers();

	//Functions for std::map<unsigned int, analytic::util::PluginLoader<result::api::ResultsAppConnector>*> _mRAppPluginLoaders;
	bool containsRAppPluginLoader(unsigned int iRAppConnectorId);
	std::map<unsigned int, analytic::util::PluginLoader<result::api::ResultsAppConnector>*>& getRAppPluginLoaders();

	//Functions for std::map<unsigned int, std::pair <unsigned int,result::api::ResultsAppConnector*> > _mRAppConnectors;
	bool containsRAppConnector(unsigned int iRAppInstanceId);
	std::map<unsigned int, std::pair <unsigned int,result::api::ResultsAppConnector*> >& getRAppConnectors();

	//Functions for std::map<unsigned int, std::vector<unsigned int> > _mRAppInstAnalyticInstances;
	bool containsResultsAppInstAnalyticInstances(unsigned int iResultsAppInstId);
	std::map<unsigned int, std::vector<unsigned int> >& ResultsAppInstAnalyticInstances();

	//void setResultsTxThreadGroup(boost::thread_group*& resultsTxThreadGroup);
	//boost::thread*& getResultsTxThread();
	//void setResultsTxThread(boost::thread*& resultsTxThread);
	virtual ~ApplicationModel();
};

} /* namespace analytic */

#endif /* ANALYTIC_APPLICATIONMODEL_HPP_ */
