
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
#include <fcntl.h>
#include <errno.h>
#include "util/TypeDefinitions.hpp"

namespace opencctv {

class Process {
protected:
	//FILE* _pReadStream;
	pid_t _pid;
public:
	Process();
	bool start(const std::string& sPathToExecutable, const std::vector<std::string>& vParamList);
	bool stop();
	pid_t getPid();
	bool readStatus(int& iStatus);
	virtual ~Process();
};

} /* namespace opencctv */

#endif /* OPENCCTV_PROCESS_HPP_ */
