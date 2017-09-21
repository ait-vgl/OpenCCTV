
#ifndef OPENCCTV_PROCESS_HPP_
#define OPENCCTV_PROCESS_HPP_

//#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include "Exception.hpp"
#include "util/Util.hpp"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>

namespace opencctv {

enum class Process_Status_t {STOPPED = 0, STARTING = 1, STARTED = 2, ERROR = 3};

class Process
{
protected:
	FILE* _pReadStream;
	pid_t _pid;
	Process_Status_t _iStatus;

public:
	Process();
	bool start(const std::string& sPathToExecutable, const std::string& sCommandLineParams);
	bool start(std::string sPathToAnalyticRunnerExecutable,
			unsigned int iAnalyticInstanceId, std::string& sAnalyticPluginFilename, std::string& sAnalyticResultsDir);
	bool stop();
	bool close();
	pid_t getPid();
	bool readLine(std::string& sStreamOutput);
	Process_Status_t getStatus();
	virtual ~Process();
};

} /* namespace opencctv */

#endif /* OPENCCTV_PROCESS_HPP_ */
