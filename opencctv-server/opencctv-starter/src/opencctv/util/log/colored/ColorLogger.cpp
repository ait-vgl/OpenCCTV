
#include "ColorLogger.hpp"

namespace opencctv {
namespace util {
namespace log {

// TODO: Implement log level from config file
// info, warn, debug => dev
// error => deployment

ColorLogger::ColorLogger() {
}

void ColorLogger::error(std::string sMessage)
{
	boost::mutex::scoped_lock lock(_mutex);
	std::cerr << "Error: " << "\033[31m" << sMessage << "\033[0m" << std::endl;
}

void ColorLogger::warn(std::string sMessage)
{
	boost::mutex::scoped_lock lock(_mutex);
	std::cerr << "Warning: " << "\033[33m" << sMessage << "\033[0m" << std::endl;
}

void ColorLogger::info(std::string sMessage)
{
	boost::mutex::scoped_lock lock(_mutex);
	std::cerr << "Info: " << "\033[34m" << sMessage << "\033[0m" << std::endl;
}

void ColorLogger::debug(std::string sMessage)
{
	boost::mutex::scoped_lock lock(_mutex);
	std::cerr << "Debug: " <<  sMessage << std::endl;
}

ColorLogger::~ColorLogger() {
}

} /* namespace log */
} /* namespace util */
} /* namespace opencctv */
