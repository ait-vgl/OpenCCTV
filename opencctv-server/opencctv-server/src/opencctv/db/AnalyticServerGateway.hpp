/*
 * StreamGateway.hpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#ifndef ANALYTICSERVERGATEWAY_HPP_
#define ANALYTICSERVERGATEWAY_HPP_

#include <vector>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "DbConnector.hpp"
#include "../dto/AnalyticServerDTO.hpp"
#include "../Exception.hpp"
#include "../util/Config.hpp"
#include "../util/Util.hpp"

namespace opencctv {
namespace db {

class AnalyticServerGateway {
private:
	sql::Connection* _pDbConnPtr;
	sql::Statement* _pStatement;
	//sql::PreparedStatement* _pStatementPtr;
	static const std::string _SQL;

public:
	AnalyticServerGateway();
	virtual ~AnalyticServerGateway();
	void findAll(std::vector<opencctv::dto::AnalyticServerDTO>& vAnalyticServers);
};

} /* namespace db */
} /* namespace opencctv */

#endif /* ANALYTICSERVERGATEWAY_HPP_ */
