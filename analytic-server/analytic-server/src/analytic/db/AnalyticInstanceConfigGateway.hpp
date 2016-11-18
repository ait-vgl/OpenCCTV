/*
 * AnalyticInstanceConfigGatewayGateway.hpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#ifndef STREAMGATEWAY_HPP_
#define STREAMGATEWAY_HPP_

#include <vector>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "DbConnector.hpp"
#include "../dto/AnalyticInstanceConfig.hpp"
#include "../../opencctv/Exception.hpp"
#include "../util/Config.hpp"
//#include "../util/analytic/Util.hpp"

namespace analytic {
namespace db {

class AnalyticInstanceConfigGateway {
private:
	sql::Connection* _pDbConnPtr;
	//sql::Statement* _pStatement;
	sql::PreparedStatement* _pStatementPtr;
	static const std::string _SELECT_STREAM_SQL;

public:
	AnalyticInstanceConfigGateway();
	virtual ~AnalyticInstanceConfigGateway();
	void getAnalyticInstanceConfig(unsigned int iAnalyticInstanceId, std::vector<analytic::dto::AnalyticInstanceConfig>& vAnalyticInstanceConfigs);
};

} /* namespace db */
} /* namespace analytic */

#endif /* STREAMGATEWAY_HPP_ */
