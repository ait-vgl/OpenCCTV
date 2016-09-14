/*
 * AnalyticResultGateway.hpp
 *
 *  Created on: Jun 4, 2015
 *      Author: anjana
 */

#ifndef ANALYTICRESULTGATEWAY_HPP_
#define ANALYTICRESULTGATEWAY_HPP_

#include <vector>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "DbConnector.hpp"
#include "../dto/Stream.hpp"
#include "../Exception.hpp"
#include "../util/Config.hpp"
#include "../util/Util.hpp"
#include "../../analytic/AnalyticResult.hpp"

namespace opencctv {
namespace db {

class AnalyticResultGateway {
private:
private:
	sql::Connection* _pDbConnPtr;
	sql::PreparedStatement* _pStatementPtr;
	static const std::string _INSERT_ANALYTIC_RESULT_SQL;

public:
	AnalyticResultGateway();
	virtual ~AnalyticResultGateway();
	int insertResults(unsigned int iAnalyticInstanceId,analytic::AnalyticResult analyticResult);
};

} /* namespace db */
} /* namespace opencctv */

#endif /* ANALYTICRESULTGATEWAY_HPP_ */
