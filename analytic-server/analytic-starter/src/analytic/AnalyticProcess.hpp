
#ifndef ANALYTIC_ANALYTICPROCESS_HPP_
#define ANALYTIC_ANALYTICPROCESS_HPP_

#include "Process.hpp"
//#include "../result/db/AnalyticInstanceGateway.hpp"

namespace analytic {

class AnalyticProcess : public analytic::Process
{
private:
	bool _bIsActive;					//Indicate whether the analytic instance is running or stopped
	unsigned int _iAnalyticInstanceId;
	//bool isAnalyticInstInfoSent; 	//Indicate whether the details about the analytic instance is sent to the remote

public:
	AnalyticProcess(unsigned int iAnalyticInstanceId);
	//bool startAnalytic(const std::string sPathToAnalyticRunnerExecutable, unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename, const std::string& sInputAnalyticQueueAddress, const std::string& sOutputAnalyticQueueAddress);
	//bool startAnalytic(const std::string sPathToAnalyticRunnerExecutable, unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginFilename, const std::string& sAnalyticResultsDir);
	//bool startAnalytic(std::string sPathToAnalyticRunnerExecutable, unsigned int iAnalyticInstanceId, std::string& sAnalyticPluginFilename, std::string& sAnalyticResultsDir);
	bool startAnalytic(std::string sPathToAnalyticRunnerExecutable, std::string& sAnalyticPluginFilename, std::string& sAnalyticResultsDir);
	bool stopAnalytic();
	virtual ~AnalyticProcess();
	bool isIsActive() const;
	void setIsActive(bool isActive);
	unsigned int getAnalyticStatus();
	unsigned int getAnalyticInstanceId() const;
	//bool updateStatus();
	//bool isIsAnalyticInstInfoSent() const;
	//void setIsAnalyticInstInfoSent(bool isAnalyticInstInfoSent);
};

} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICPROCESS_HPP_ */
