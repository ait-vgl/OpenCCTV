
#ifndef OPENCCTV_PROCESS_HPP_
#define OPENCCTV_PROCESS_HPP_

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <signal.h>
#include "Exception.hpp"
#include "util/Util.hpp"
#include <sys/wait.h>

namespace opencctv {

class Process {
protected:
	FILE* _pReadStream;
	pid_t _pid;
public:
	Process();
	bool start(const std::string& sPathToExecutable, const std::string& sCommandLineParams);
	bool stop();
	bool close();
	pid_t getPid();
	bool readLine(std::string& sStreamOutput);
	virtual ~Process();
};

} /* namespace opencctv */

#endif /* OPENCCTV_PROCESS_HPP_ */
