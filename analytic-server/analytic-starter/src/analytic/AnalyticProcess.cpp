
#include "AnalyticProcess.hpp"

namespace analytic
{

AnalyticProcess::AnalyticProcess() : opencctv::Process()
{
	isActive = false;
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
	isActive = false;
	try
	{
		isActive = start(sPathToAnalyticRunnerExecutable, ssArgs.str());
		return isActive;
	}catch(opencctv::Exception &e)
	{
		throw e;
	}

}

bool analytic::AnalyticProcess::stopAnalytic()
{
	bool bResult = stop();
	if(bResult)
	{
		isActive = false;
	}

	return bResult;
}

bool AnalyticProcess::isIsActive() const
{
	return isActive;
}

void AnalyticProcess::setIsActive(bool isActive)
{
	this->isActive = isActive;
}

/*bool AnalyticProcess::isIsAnalyticInstInfoSent() const
{
	return isAnalyticInstInfoSent;
}

void AnalyticProcess::setIsAnalyticInstInfoSent(bool isAnalyticInstInfoSent)
{
	this->isAnalyticInstInfoSent = isAnalyticInstInfoSent;
}*/

AnalyticProcess::~AnalyticProcess()
{
}

} /* namespace analytic */


