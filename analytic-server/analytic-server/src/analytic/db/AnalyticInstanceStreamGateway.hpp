/*
 * AnalyticInstanceStreamGateway.hpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#ifndef ANALYTICINSTANCESTREAMGATEWAY_HPP_
#define ANALYTICINSTANCESTREAMGATEWAY_HPP_

#include <vector>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "DbConnector.hpp"
#include "../dto/AnalyticInstanceStream.hpp"
#include "../../opencctv/Exception.hpp"
#include "../util/Config.hpp"
//#include "../util/Util.hpp"

namespace analytic {
namespace db {

class AnalyticInstanceStreamGateway {
private:
	sql::Connection* _pDbConnPtr;
	sql::PreparedStatement* _pStatementPtr;
	static const std::string _SELECT_ANALYTIC_INSTANCE_STREAM_SQL;

public:
	AnalyticInstanceStreamGateway();
	void getAnalyticInstanceStreams(unsigned int iAnalyticInstanceId, std::vector<analytic::dto::AnalyticInstanceStream>& vAnalyticInstanceStream);
	virtual ~AnalyticInstanceStreamGateway();
};

} /* namespace db */
} /* namespace analytic */

#endif /* ANALYTICINSTANCESINPUTTREAMGATEWAY_HPP_ */
