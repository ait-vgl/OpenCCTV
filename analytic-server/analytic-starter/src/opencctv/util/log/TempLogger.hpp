
#ifndef TEMPLOGGER_HPP_
#define TEMPLOGGER_HPP_

#include "Logger.hpp"
#include <stdio.h>

namespace opencctv {
namespace util {
namespace log {

class TempLogger : public Logger {
public:
	TempLogger();
	void error(const std::string& sMessage);
	void warn(const std::string& sMessage);
	void info(const std::string& sMessage);
	void debug(const std::string& sMessage);
	virtual ~TempLogger();
};

} /* namespace opencctv */
} /* namespace util */
} /* namespace log */

#endif /* TEMPLOGGER_HPP_ */
