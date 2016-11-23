
#ifndef OPENCCTV_COLORLOGGER_HPP_
#define OPENCCTV_COLORLOGGER_HPP_

#include "../Logger.hpp"
#include <iostream>

namespace opencctv {
namespace util {
namespace log {

class ColorLogger : public Logger {
public:
	ColorLogger();
	void error(const std::string& sMessage);
	void warn(const std::string& sMessage);
	void info(const std::string& sMessage);
	void debug(const std::string& sMessage);
	virtual ~ColorLogger();
};

} /* namespace log */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTV_COLORLOGGER_HPP_ */
