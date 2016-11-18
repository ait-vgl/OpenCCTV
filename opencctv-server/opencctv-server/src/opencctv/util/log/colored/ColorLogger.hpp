
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
	void error(std::string sMessage);
	void warn(std::string sMessage);
	void info(std::string sMessage);
	void debug(std::string sMessage);
	virtual ~ColorLogger();
};

} /* namespace log */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTV_COLORLOGGER_HPP_ */
