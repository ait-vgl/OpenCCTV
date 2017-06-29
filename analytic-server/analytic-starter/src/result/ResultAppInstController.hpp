/*
 * ResultAppInstController.hpp
 *
 *  Created on: May 11, 2017
 *      Author: anjana
 */

#ifndef RESULTAPPINSTCONTROLLER_HPP_
#define RESULTAPPINSTCONTROLLER_HPP_

#include <string>
#include <sstream>
#include <map>
#include <boost/lexical_cast.hpp>
#include "../opencctv/Exception.hpp"
#include "../opencctv/util/log/Loggers.hpp"
//#include "../analytic/ApplicationModel.hpp"
#include "api/ResultsAppConnector.hpp"
#include "db/ResultsAppInstanceGateway.hpp"
#include "db/AnalyticInstanceGateway.hpp"
#include "db/AnalyticResultGateway.hpp"
#include "db/dto/ResultsAppInstance.hpp"
#include "db/dto/AnalyticInstance.hpp"
#include "db/dto/AnalyticResult.hpp"

//class analytic::ApplicationModel;

namespace result
{

class ResultAppInstController
{
private:
	typedef struct
	{
		result::db::dto::AnalyticInstance analyticInstance;
		bool isInfoSent;
	} AnalyticInstance_t;

	unsigned int _iResultsAppInstId;
	result::api::ResultsAppConnector* _pRAppConnector;
	std::map<unsigned int, AnalyticInstance_t> _mapAnalyticInstances;
	bool sendAnalyticInstInfo(result::db::dto::AnalyticInstance AnalyticInstance);
	void sendAnalyticResult(result::db::dto::AnalyticResult& analyticResult);

public:
	ResultAppInstController(unsigned int iResultAppInstId);
	virtual ~ResultAppInstController();
	bool init();
	bool authenticate();
	bool sendAllAnalyticInstInfo();
	void sendAnalyticResults();
	void addAnalyticInstance(unsigned int iAnalyticInstId);
	void removeAnalyticInstance(unsigned int iAnalyticInstId);
	//bool init(const result::db::dto::ResultsAppInstance& rAppInst);
};

} /* namespace result */

#endif /* RESULTAPPINSTCONTROLLER_HPP_ */
