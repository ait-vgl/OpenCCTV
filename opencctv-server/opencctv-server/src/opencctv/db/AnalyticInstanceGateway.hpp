/*
 * AnalyticInstanceGateway.hpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#ifndef ANALYTICINSTANCEGATEWAY_HPP_
#define ANALYTICINSTANCEGATEWAY_HPP_

#include <vector>
#include <cppconn/prepared_statement.h>
#include "DbConnector.hpp"

#include "../Exception.hpp"
#include "../util/Config.hpp"
#include "../util/Util.hpp"

namespace opencctv {
namespace db {

class AnalyticInstanceGateway {
private:
    sql::Connection* _pDbConnPtr;
	sql::PreparedStatement* _pStatementPtr;
	static const std::string _UPDATE_ANALYTICINSTANCE_SQL;


public:
	AnalyticInstanceGateway();
	virtual ~AnalyticInstanceGateway();

	int updateStatus(unsigned int iAnalyticInstanceId, int iStatus);
};

} /* namespace db */
} /* namespace opencctv */

#endif /* ANALYTICINSTANCEGATEWAY_HPP_ */
