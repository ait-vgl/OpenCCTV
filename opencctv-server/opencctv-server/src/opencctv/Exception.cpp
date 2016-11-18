
#include "Exception.hpp"

namespace opencctv {

Exception::Exception() : std::exception() {
}

Exception::Exception(const std::string& sErrorMessage)
{
	_sMessage = sErrorMessage;
}

const char* Exception::what() const throw() {
	return _sMessage.c_str();
}

void Exception::setMessage(const std::string& sMessage)
{
	_sMessage =sMessage;
}

Exception::~Exception() throw() {
}

} /* namespace opencctv */
