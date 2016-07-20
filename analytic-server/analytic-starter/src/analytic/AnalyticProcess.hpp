
#ifndef ANALYTIC_ANALYTICPROCESS_HPP_
#define ANALYTIC_ANALYTICPROCESS_HPP_

#include "../opencctv/Process.hpp"

namespace analytic {

class AnalyticProcess : public opencctv::Process {
public:
	AnalyticProcess();
	bool startAnalytic(const std::string sPathToAnalyticRunnerExecutable, unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename, const std::string& sInputAnalyticQueueAddress, const std::string& sOutputAnalyticQueueAddress);
	virtual ~AnalyticProcess();
};

} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICPROCESS_HPP_ */
