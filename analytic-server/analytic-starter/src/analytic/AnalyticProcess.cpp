
#include "AnalyticProcess.hpp"

namespace analytic
{

AnalyticProcess::AnalyticProcess(unsigned int iAnalyticInstanceId) : analytic::Process()
{
	_iAnalyticInstanceId = iAnalyticInstanceId;
	_bIsActive = false;
}

/*bool AnalyticProcess::startAnalytic(std::string sPathToAnalyticRunnerExecutable, std::string& sAnalyticPluginFilename, std::string& sAnalyticResultsDir)
{
	std::stringstream ssArgs;
	ssArgs << _iAnalyticInstanceId << " ";
	ssArgs << sAnalyticPluginFilename << " ";
	ssArgs << sAnalyticResultsDir;
	_bIsActive = false;
	try
	{
		_bIsActive = start(sPathToAnalyticRunnerExecutable, ssArgs.str());
		return _bIsActive;
	}catch(opencctv::Exception &e)
	{
		throw e;
	}
}*/

bool AnalyticProcess::startAnalytic(std::string sPathToAnalyticRunnerExecutable,
		std::string& sAnalyticPluginFilename, std::string& sAnalyticResultsDir)
{
	std::stringstream ssArgs;
	ssArgs << _iAnalyticInstanceId << " ";
	ssArgs << sAnalyticPluginFilename << " ";
	ssArgs << sAnalyticResultsDir;
	_bIsActive = false;
	try
	{
		_bIsActive = start(sPathToAnalyticRunnerExecutable, _iAnalyticInstanceId, sAnalyticPluginFilename, sAnalyticResultsDir);
		//_bIsActive = start(sPathToAnalyticRunnerExecutable, ssArgs.str());
		return _bIsActive;
	}catch(opencctv::Exception &e)
	{
		throw e;
	}
}

/*bool AnalyticProcess::startAnalytic(const std::string sPathToAnalyticRunnerExecutable, unsigned int iAnalyticInstanceId, const std::string &sAnalyticPluginDirLocation, const std::string &sAnalyticPluginFilename, const std::string &sInputAnalyticQueueAddress, const std::string &sOutputAnalyticQueueAddress)
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
}*/

bool analytic::AnalyticProcess::stopAnalytic()
{
	bool bResult = stop();
	if(bResult)
	{
		_bIsActive = false;
	}

	return bResult;
}

bool AnalyticProcess::isIsActive() const
{
	return _bIsActive;
}

void AnalyticProcess::setIsActive(bool isActive)
{
	this->_bIsActive = isActive;
}

/*bool AnalyticProcess::updateStatus()
{
  bool bResult = true;
  result::db::AnalyticInstanceGateway analyticInstanceGateway;

  try
  {
	  unsigned int iStatus = boost::lexical_cast<unsigned int>(getStatus());
	  std::cout << "Analytic status : " << _iAnalyticInstanceId << " : " << iStatus << std::endl;
	  //TODO Update the updated_at filed also in the DB table
	  analyticInstanceGateway.updateStatus(_iAnalyticInstanceId, iStatus);
  }catch(opencctv::Exception &e)
  {
	  bResult =  false;
	  throw e;
  }catch(const boost::bad_lexical_cast &e)
  {
	  bResult =  false;
	  throw opencctv::Exception(e.what());
  }

  return bResult;
}*/

unsigned int AnalyticProcess::getAnalyticStatus()
{
	unsigned int iStatus = ANALYTIC_STATUS_UNKNOWN;

	try
	{
	  iStatus = boost::lexical_cast<unsigned int>(getStatus());

	}catch(const boost::bad_lexical_cast &e)
	{
		iStatus = ANALYTIC_STATUS_UNKNOWN;
	}

	return iStatus;
}

unsigned int AnalyticProcess::getAnalyticInstanceId() const
{
	return _iAnalyticInstanceId;
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


