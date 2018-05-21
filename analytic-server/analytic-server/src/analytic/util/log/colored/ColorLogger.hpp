
#ifndef ANALYTIC_COLORLOGGER_HPP_
#define ANALYTIC_COLORLOGGER_HPP_

#include "../../../../opencctv/util/log/Logger.hpp"
#include <iostream>

namespace analytic {
namespace util {
namespace log {

class ColorLogger : public opencctv::util::log::Logger {
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
} /* namespace analytic */

#endif /* ANALYTIC_COLORLOGGER_HPP_ */
