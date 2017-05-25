/*
 * ResultsTxThread.hpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#ifndef RESULTSTXTHREAD_HPP_
#define RESULTSTXTHREAD_HPP_

#include <string>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include "ResultAppInstController.hpp"
#include "../analytic/ApplicationModel.hpp"

namespace result
{

class ResultsTxThread
{
private:
	unsigned int _iResultsAppInstId;
	ResultAppInstController* _pResultAppInstController;

public:
	ResultsTxThread(unsigned int iResultsAppInstId);
	void operator()();
	virtual ~ResultsTxThread();
};

} /* namespace result */

#endif /* RESULTSTXTHREAD_HPP_ */
