
#include "Loggers.hpp"
#include "../../../analytic/util/log/colored/ColorLogger.hpp"

namespace opencctv {
namespace util {
namespace log {

Logger* Loggers::_pDefaultLogger = NULL;

Logger* Loggers::getDefaultLogger()
{
	if(!_pDefaultLogger)
	{
		// _pDefaultLogger = new TempLogger();
		_pDefaultLogger = new analytic::util::log::ColorLogger();
	}
	return _pDefaultLogger;
}

} /* namespace opencctv */
} /* namespace util */
} /* namespace log */
