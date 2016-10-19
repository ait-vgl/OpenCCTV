
#ifndef OPENCCTV_LOGGER_HPP_
#define OPENCCTV_LOGGER_HPP_

#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

namespace opencctv {
namespace util {
namespace log {

class Logger
{
protected:
	boost::mutex _mutex;
	/* boost::mutex::scoped_lock lock(_mutex);
	 * lock.unlock(); */
public:
	virtual void error( std::string sMessage) = 0;
	virtual void warn( std::string sMessage) = 0;
	virtual void info( std::string sMessage) = 0;
	virtual void debug( std::string sMessage) = 0;
	virtual ~Logger(){}
};

} /* namespace opencctv */
} /* namespace util */
} /* namespace log */

#endif /* OPENCCTV_LOGGER_HPP_ */
