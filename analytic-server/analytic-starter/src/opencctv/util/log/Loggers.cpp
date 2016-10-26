
#include "Loggers.hpp"
#include "TempLogger.hpp"
#include "colored/ColorLogger.hpp"

namespace opencctv {
namespace util {
namespace log {

Logger* Loggers::_pDefaultLogger = NULL;

Logger* Loggers::getDefaultLogger()
{
	if(!_pDefaultLogger)
	{
		// _pDefaultLogger = new TempLogger();
		_pDefaultLogger = new ColorLogger();
	}
	return _pDefaultLogger;
}

} /* namespace opencctv */
} /* namespace util */
} /* namespace log */
