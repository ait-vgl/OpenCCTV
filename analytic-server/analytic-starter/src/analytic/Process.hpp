
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

const unsigned int ANALYTIC_STATUS_STOPPED = 0;
const unsigned int ANALYTIC_STATUS_STARTING = 1;
const unsigned int ANALYTIC_STATUS_STARTED = 2;
const unsigned int ANALYTIC_STATUS_ERROR = 3;
const unsigned int ANALYTIC_STATUS_UNKNOWN = 4;

class Process
{
protected:
	//enum Process_Status_t {STOPPED = 0, STARTING = 1, STARTED = 2, ERROR = 3, UNKNOWN = 4};
	FILE* _pReadStream;
	pid_t _pid;
	//Process_Status_t _iStatus;
	//Process_Status_t getStatus();
	unsigned int _iStatus;
	unsigned int getStatus();

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
