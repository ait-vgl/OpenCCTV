/*
 * AnalyticExecException.hpp
 *
 *  Created on: Oct 10, 2014
 *      Author: anjana
 */

#ifndef ANALYTICEXECEXCEPTION_HPP_
#define ANALYTICEXECEXCEPTION_HPP_

#include <exception>
#include <string>

using namespace std;

namespace analytic_exception {

class AnalyticExecException: public std::exception {
private:
	string message;

public:
	AnalyticExecException();
	AnalyticExecException(string message);
	virtual ~AnalyticExecException() throw();
	const char * what () const throw ();

	const string& getMessage() const;

	void setMessage(const string& message);
};

} /* namespace analytic_exception */

#endif /* ANALYTICEXECEXCEPTION_HPP_ */
