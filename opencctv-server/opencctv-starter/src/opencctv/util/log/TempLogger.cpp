
#include "TempLogger.hpp"

namespace opencctv {
namespace util {
namespace log {

TempLogger::TempLogger() {
}

void TempLogger::error(const std::string& sMessage)
{
	boost::mutex::scoped_lock lock(_mutex);
	fprintf(stderr, "Error: %s\n", sMessage.c_str());
	fflush(stderr);
}

void TempLogger::info(const std::string& sMessage)
{
	boost::mutex::scoped_lock lock(_mutex);
	fprintf(stdout, "Info: %s\n", sMessage.c_str());
	fflush(stderr);
}

void TempLogger::warn(const std::string& sMessage)
{
	boost::mutex::scoped_lock lock(_mutex);
	fprintf(stdout, "Warning: %s\n", sMessage.c_str());
	fflush(stdout);
}

void TempLogger::debug(const std::string& sMessage)
{
	boost::mutex::scoped_lock lock(_mutex);
	fprintf(stdout, "Debug: %s\n", sMessage.c_str());
	fflush(stdout);
}

TempLogger::~TempLogger() {
}

} /* namespace opencctv */
} /* namespace util */
} /* namespace log */
