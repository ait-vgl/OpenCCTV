/*
 * AnalyticExecException.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: anjana
 */

#include "AnalyticExecException.hpp"

namespace analytic_exception {

AnalyticExecException::AnalyticExecException() {
	this->message = "";

}

AnalyticExecException::AnalyticExecException(string message){
	this->message = message;
}

AnalyticExecException::~AnalyticExecException() throw() {
	// TODO Auto-generated destructor stub
}

const string& AnalyticExecException::getMessage() const {
	return message;
}

void AnalyticExecException::setMessage(const string& message) {
	this->message = message;
}

const char * AnalyticExecException::what () const throw (){
	string error_msg = "AnalyticExec Exception -- ";
	error_msg.append(message);
    return error_msg.c_str();
}

} /* namespace analytic_exception */
