/*
 * AnalyticResultGateway.hpp
 *
 *  Created on: Nov 5, 2014
 *      Author: anjana
 */

#ifndef ANALYTICRESULTGATEWAY_HPP_
#define ANALYTICRESULTGATEWAY_HPP_

#include "DbConnector.hpp"
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

namespace db {

class AnalyticResultGateway {
private:
	Connection* _pDbConn;
	static const string _INSERT_RESULT_SQL;

public:
	AnalyticResultGateway();
	virtual ~AnalyticResultGateway();
	int insertResults(int iAnalyticInstId, string sImgTimestamp, string sResult, string sImgFilePath);


};

} /* namespace db */

#endif /* ANALYTICRESULTGATEWAY_HPP_ */
