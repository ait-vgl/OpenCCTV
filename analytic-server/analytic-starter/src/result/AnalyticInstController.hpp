/*
 * AnalyticInstController.hpp
 *
 *  Created on: Jun 29, 2017
 *      Author: anjana
 */

#ifndef ANALYTICINSTCONTROLLER_HPP_
#define ANALYTICINSTCONTROLLER_HPP_

#include <string>
#include <vector>
#include "../analytic/ApplicationModel.hpp"
#include "db/ResultsAppInstanceGateway.hpp"
#include "db/dto/ResultsAppInstance.hpp"
#include "ResultAppInstController.hpp"
#include "ResultsTxThread.hpp"

namespace result
{

class AnalyticInstController
{
private:
	//unsigned int _iAnalyticInstId;

public:
	//AnalyticInstController(unsigned int iAnalyticInstId);
	AnalyticInstController();
	virtual ~AnalyticInstController();
	void startResultsRouting(unsigned int iAnalyticInstId, std::string& sOutputMsg);

};

} /* namespace result */

#endif /* ANALYTICINSTCONTROLLER_HPP_ */
