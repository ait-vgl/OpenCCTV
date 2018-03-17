
#include "Process.hpp"

namespace analytic {

Process::Process()
{
	_pReadStream = NULL;
	_pid = 0;
	_iStatus = ANALYTIC_STATUS_STOPPED;
}

bool Process::start(const std::string& sPathToExecutable, const std::string& sCommandLineParams)
{
	bool bRet = false;
	std::stringstream ssCommand;
	ssCommand << "exec ";
	ssCommand << sPathToExecutable << " ";
	ssCommand << sCommandLineParams;
    _pReadStream = popen(ssCommand.str().data(), "r");
    _iStatus = ANALYTIC_STATUS_STARTING;
	if (!_pReadStream)
	{
		std::string sErrMsg = "Failed to start Analytic process. Failed to execute command: ";
		sErrMsg.append(ssCommand.str());
		throw opencctv::Exception(sErrMsg);
	} else
	{
		const int iBufSize = 1024;
		char buf[iBufSize];
		char* pCh = fgets(buf, iBufSize, _pReadStream);
     
		std::string sStreamOutput = std::string(buf);
		if (pCh)
		{
		   	try
			{
				_pid = opencctv::util::Util::getPid(sStreamOutput);
                std::cout << "From Process: return pid ......: "<<_pid << std::endl;
			} catch (opencctv::Exception &e) {
				std::string sErrMsg = "Failed to retrieve PID of Analytic process. Analytic process output: ";
				sErrMsg.append(sStreamOutput);
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}
		}

		if (_pid > 0)
		{
			_iStatus = ANALYTIC_STATUS_STARTED;
			bRet = true;
		}
	}
	return bRet;
}

bool Process::start(std::string sPathToAnalyticRunnerExecutable,
		unsigned int iAnalyticInstanceId, std::string& sAnalyticPluginFilename, std::string& sAnalyticResultsDir)
{
	_pid = fork();
	if (_pid < 0)
	{
		return false;
	}

	if(_pid == 0)
	{
		_iStatus = ANALYTIC_STATUS_STARTING;
		char* exec_args[5];

		exec_args[0] = &sPathToAnalyticRunnerExecutable[0];

		std::string sStremaIdStr = boost::lexical_cast<std::string>(iAnalyticInstanceId);
		exec_args[1] = &sStremaIdStr[0];

		exec_args[2] = &sAnalyticPluginFilename[0];

		exec_args[3] = &sAnalyticResultsDir[0];

		exec_args[4] = NULL;

		execv(exec_args[0], exec_args);

		//If exec returns, process must have failed.
		throw opencctv::Exception("Failed to start the analytic instance");
		_iStatus = ANALYTIC_STATUS_ERROR;
		_exit(-1);
	}

	_iStatus = ANALYTIC_STATUS_STARTED;
	return true;
}

bool Process::stop()
{
	bool died = false;
	int status;
	if (_pid > 0)
	{
		kill(_pid, SIGTERM);
		sleep(2);
		for (int loop = 0; !died && loop < 10; ++loop)
		{
			pid_t result = waitpid(_pid, &status, WNOHANG);
			if (result == _pid)
			{
				died = true;
				break;
			}
			sleep(1);
		}

		if (!died)
		{
			kill(_pid, SIGKILL);
			waitpid(_pid, &status, 0);
			died = true;
		}
	}
	_iStatus = ANALYTIC_STATUS_STOPPED;
	_pid = 0;
	if (_pReadStream)
	{
		pclose(_pReadStream); _pReadStream = NULL;

	}
	return died;
}

bool Process::close()
{
	if(_pReadStream)
	{
		if(pclose(_pReadStream) == 0)
		{
			return true;
		}
	}
	return false;
}

pid_t Process::getPid()
{
	return _pid;
}

bool Process::readLine(std::string& sStreamOutputLine)
{
	const int iSize = 2048;
	char buf[iSize];
	char* pCh = fgets(buf, iSize, _pReadStream);
	if (pCh)
	{
		sStreamOutputLine = std::string(buf);
		return true;
	}
	return false;
}

unsigned int Process::getStatus()
{
	if(_pid == 0)
	{
		_iStatus = ANALYTIC_STATUS_STOPPED;
		return _iStatus;
	}

	int status;
	pid_t result = waitpid(_pid, &status, WNOHANG);
	if (result == 0) /* Child is still running */
	{
		_iStatus = ANALYTIC_STATUS_STARTED;
	} else if (result == _pid) /* Child has finished execution */
	{
		_iStatus = ANALYTIC_STATUS_STOPPED;
	} else /* Error */
	{
		_iStatus = ANALYTIC_STATUS_ERROR;
	}

	return _iStatus;
}

Process::~Process()
{
	if (_pReadStream)
	{
		pclose(_pReadStream);
		delete _pReadStream; _pReadStream = NULL;
	}
}

} /* namespace analytic */
