
#ifndef OPENCCTV_EXCEPTION_HPP_
#define OPENCCTV_EXCEPTION_HPP_

#include <string>

namespace opencctv {

class Exception : public std::exception {
private:
	std::string _sMessage;
public:
	Exception();
	Exception(const std::string& sErrorMessage);
	const char* what() const throw();
	void setMessage(const std::string& sMessage);
	virtual ~Exception() throw();
};

} /* namespace opencctv */

#endif /* OPENCCTV_EXCEPTION_HPP_ */
