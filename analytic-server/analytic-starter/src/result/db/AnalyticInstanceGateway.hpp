/*
 * AnalyticInstanceGateway.hpp
 *
 *  Created on: May 12, 2017
 *      Author: anjana
 */

#ifndef ANALYTICINSTANCEGATEWAY_HPP_
#define ANALYTICINSTANCEGATEWAY_HPP_

#include <vector>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "DbConnector.hpp"
#include "dto/AnalyticInstance.hpp"
#include "../../opencctv/Exception.hpp"

namespace result {
namespace db {

class AnalyticInstanceGateway
{
private:
	DbConnector* _pDbConnectorPtr;
	sql::Connection* _pConnectionPtr;
	static const std::string _SELECT_ANALYTIC_INST_SQL;
	static const std::string _SELECT_ANALYTIC_INST_FOR_RAPP_INST_SQL;
public:
	AnalyticInstanceGateway();
	virtual ~AnalyticInstanceGateway();
	void findAnalyticInstancesForRAppInst(const unsigned int iRAppInstId,
			std::vector<result::db::dto::AnalyticInstance>& vAnalyticInstances);
	void findAnalyticInstance(const unsigned int iAnalyticInstanceId,
			result::db::dto::AnalyticInstance& analyticInstance);
};

} /* namespace db */
} /* namespace result */

#endif /* ANALYTICINSTANCEGATEWAY_HPP_ */
