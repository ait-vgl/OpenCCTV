
#ifndef ANALYTIC_PROCESS_HPP_
#define ANALYTIC_PROCESS_HPP_

//#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>
#include "../opencctv/Exception.hpp"
#include "../opencctv/util/Util.hpp"

namespace analytic {

//enum class Process_Status_t {STOPPED, STARTING, STARTED, ERROR};

class Process
{
protected:
	enum Process_Status_t {STOPPED = 0, STARTING = 1, STARTED = 2, ERROR = 3};
	FILE* _pReadStream;
	pid_t _pid;
	Process_Status_t _iStatus;
	Process_Status_t getStatus();

public:
	Process();
	bool start(const std::string& sPathToExecutable, const std::string& sCommandLineParams);
	bool start(std::string sPathToAnalyticRunnerExecutable,
			unsigned int iAnalyticInstanceId, std::string& sAnalyticPluginFilename, std::string& sAnalyticResultsDir);
	bool stop();
	bool close();
	pid_t getPid();
	bool readLine(std::string& sStreamOutput);
	virtual ~Process();
};

} /* namespace analytic */

#endif /* ANALYTIC_PROCESS_HPP_ */
