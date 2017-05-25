
#ifndef ANALYTIC_ANALYTICPROCESS_HPP_
#define ANALYTIC_ANALYTICPROCESS_HPP_

#include "../opencctv/Process.hpp"

namespace analytic {

class AnalyticProcess : public opencctv::Process
{
private:
	bool isActive;					//Indicate whether the analytic instance is running or stopped
	//bool isAnalyticInstInfoSent; 	//Indicate whether the details about the analytic instance is sent to the remote

public:
	AnalyticProcess();
	bool startAnalytic(const std::string sPathToAnalyticRunnerExecutable, unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename, const std::string& sInputAnalyticQueueAddress, const std::string& sOutputAnalyticQueueAddress);
	bool stopAnalytic();
	virtual ~AnalyticProcess();
	bool isIsActive() const;
	void setIsActive(bool isActive);
	//bool isIsAnalyticInstInfoSent() const;
	//void setIsAnalyticInstInfoSent(bool isAnalyticInstInfoSent);
};

} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICPROCESS_HPP_ */
