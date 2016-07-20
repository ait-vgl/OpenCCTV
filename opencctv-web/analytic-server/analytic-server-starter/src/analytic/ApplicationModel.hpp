
#ifndef ANALYTIC_APPLICATIONMODEL_HPP_
#define ANALYTIC_APPLICATIONMODEL_HPP_

#include "AnalyticProcess.hpp"
#include <map>

namespace analytic {

class ApplicationModel {
private:
	static ApplicationModel* _pModel;
	ApplicationModel();
	std::map<unsigned int, analytic::AnalyticProcess*> _mAnalyticProcesses; // Analytic Instance ID as key
public:
	static ApplicationModel* getInstance();
	bool containsAnalyticProcess(unsigned int iAnalyticInstanceId);
	std::map<unsigned int, analytic::AnalyticProcess*>& getAnalyticProcesses();
	virtual ~ApplicationModel();
};

} /* namespace analytic */

#endif /* ANALYTIC_APPLICATIONMODEL_HPP_ */
