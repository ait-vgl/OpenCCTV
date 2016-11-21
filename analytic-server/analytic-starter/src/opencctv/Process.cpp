
#include "Process.hpp"

namespace opencctv {

Process::Process() {
	_pReadStream = NULL;
	_pid = 0;
}

bool Process::start(const std::string& sPathToExecutable, const std::string& sCommandLineParams) {
	bool bRet = false;
	std::stringstream ssCommand;
	ssCommand << sPathToExecutable << " ";
	ssCommand << sCommandLineParams;
    //std::cout << "From Process:start(): "<< ssCommand.str().data() << std::endl;
	_pReadStream = popen(ssCommand.str().data(), "r");
	if (!_pReadStream) {
		std::string sErrMsg = "Failed to start Analytic process. Failed to execute command: ";
		sErrMsg.append(ssCommand.str());
		throw opencctv::Exception(sErrMsg);
	} else {
		const int iBufSize = 1024;
		char buf[iBufSize];
		char* pCh = fgets(buf, iBufSize, _pReadStream);
         //  std::cout << "From Process: pCh: "<< pCh << std::endl;
           // std::cout << "From Process: _pReadStream: "<< _pReadStream << std::endl;
           //std::cout << "From Process: buf: "<< buf << std::endl;
        	//std::string sStreamOutput = std::string(buf);
             //std::cout << "From Process: sStreamOutput: "<< sStreamOutput << std::endl;
     
		if (pCh) {
		   	std::string sStreamOutput = std::string(buf);
			try {
				_pid = util::Util::getPid(sStreamOutput);
                //std::cout << "From Process: return pid: "<<_pid << std::endl;
			} catch (opencctv::Exception &e) {
				std::string sErrMsg = "Failed to retrieve PID of Analytic process. Analytic process output: ";
				sErrMsg.append(sStreamOutput);
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}
		}
		if (_pid > 0) {
			bRet = true;
		}
	}
	return bRet;
}

bool Process::stop() {
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
	if (_pReadStream) {
		pclose(_pReadStream);
		// delete _pReadStream;
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

bool Process::readLine(std::string& sStreamOutputLine) {
	const int iSize = 2048;
	char buf[iSize];
	char* pCh = fgets(buf, iSize, _pReadStream);
	if (pCh) {
		sStreamOutputLine = std::string(buf);
		return true;
	}
	return false;
}

Process::~Process() {
	if (_pReadStream) {
		// pclose(_pReadStream);
		// delete _pReadStream;
	}
}

} /* namespace opencctv */
