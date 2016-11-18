
#include "Process.hpp"

namespace opencctv {

Process::Process()
{
	_pid = 0;
}

bool Process::start(const std::string& sPathToExecutable, const std::vector<std::string>& vParamList)
{
	bool bRet = false;

	pid_t pid = fork();

	if(pid == 0) //Child Process
	{
		//Copy the program path to the exec command's first argument
		char* exec_args[vParamList.size()+2];
		std::string sPath = sPathToExecutable;
		exec_args[0] = &sPath[0];

		//Copy the rest of the exec command's arguments
		int iParamNum = 1;
		std::string param;
		for(unsigned int i = 0; i < vParamList.size(); i++)
		{
			param = vParamList[i];
			exec_args[iParamNum] = &param[0];
			++iParamNum;
		}

		//Copy NULL as the exec command's last arguments
		exec_args[iParamNum] = NULL;

		execv(exec_args[0], exec_args);

		//If exec returns, process must have failed.
		throw opencctv::Exception("Starting the OpenCCTVServer Process Failed");
	}
	else if(pid > 0)//Parent Process
	{
		_pid = pid;
		bRet = true;
	}
	else
	{	bRet = false;
		throw opencctv::Exception("Starting the OpenCCTVServer Process Failed");
	}

	return bRet;
}

bool Process::stop()
{
	bool died = false;

	if (_pid > 0) {
		kill(_pid, SIGTERM);
		for (int loop = 1; !died && loop < 10; ++loop) {
			int status;
			if (waitpid(_pid, &status, WNOHANG) == -1)
			{
				died = true;
				break;
			}
			sleep(1);
		}
		if (!died)
			kill(_pid, SIGKILL);
	}
	_pid = 0;

	return died;
}

pid_t Process::getPid()
{
	return _pid;
}

/*
 * If the pid = 0, the process may have not have yet started, thus returns false and
 * the iStatus has no meaning
 *
 * If pid > 0, the process has started, thus iStatus will be set to the result return by
 * waitpid() system call; 0 - Child alive, -1 - Error and >0 - Child exited
 */
bool Process::readStatus(int& iStatus)
{
	bool bResullt = false;
	if(_pid > 0 )
	{
		bResullt = true;
		int status;
		pid_t result = waitpid(_pid, &status, WNOHANG);
		iStatus = result;
	}
	else
	{
		bResullt = false;
	}

	return bResullt;
}

Process::~Process()
{

}

} /* namespace opencctv */
