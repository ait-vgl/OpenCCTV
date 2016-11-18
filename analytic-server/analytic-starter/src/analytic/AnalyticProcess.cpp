
#include "AnalyticProcess.hpp"

namespace analytic
{

AnalyticProcess::AnalyticProcess() : opencctv::Process()
{
}

bool AnalyticProcess::startAnalytic(const std::string sPathToAnalyticRunnerExecutable, unsigned int iAnalyticInstanceId, const std::string &sAnalyticPluginDirLocation, const std::string &sAnalyticPluginFilename, const std::string &sInputAnalyticQueueAddress, const std::string &sOutputAnalyticQueueAddress)
{
	//bool bRet = false;
	std::stringstream ssArgs;
	ssArgs << iAnalyticInstanceId << " ";
	ssArgs << sAnalyticPluginDirLocation << " ";
	ssArgs << sAnalyticPluginFilename << " ";
	ssArgs << sInputAnalyticQueueAddress << " ";
	ssArgs << sOutputAnalyticQueueAddress;
	//std::cout << "From Starter startAnalyticProcess(): " + sPathToAnalyticRunnerExecutable + " " << ssArgs.str() << std::endl;
	return start(sPathToAnalyticRunnerExecutable, ssArgs.str());
}

AnalyticProcess::~AnalyticProcess()
{
}

} /* namespace analytic */
