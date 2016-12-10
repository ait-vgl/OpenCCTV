
#ifndef OPENCCTV_LOGGERS_HPP_
#define OPENCCTV_LOGGERS_HPP_

#include "Logger.hpp"

namespace analytic {
namespace util {
namespace log {

class Loggers {
private:
	static Logger* _pDefaultLogger;
public:
	static Logger* getDefaultLogger();
};

} /* namespace analytic */
} /* namespace util */
} /* namespace log */

#endif /* OPENCCTV_LOGGERS_HPP_ */
