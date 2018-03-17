/*
 * AnalyticServerGateway.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: anjana
 */

#ifndef ANALYTICSERVERGATEWAY_HPP_
#define ANALYTICSERVERGATEWAY_HPP_

#include <vector>
//#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "DbConnector.hpp"
#include "../../opencctv/Exception.hpp"

namespace result {
namespace db {

class AnalyticServerGateway
{
private:
	DbConnector* _pDbConnectorPtr;
	sql::Connection* _pConnectionPtr;
	static const std::string _SELECT_ANALYTIC_INST_SQL;
	//static const std::string _SELECT_ANALYTIC_SERVER_STATUS_SQL;
	static const std::string _UPDATE_ANALYTIC_SERVER_STATUS_SQL;

public:
	AnalyticServerGateway();
	virtual ~AnalyticServerGateway();
	void findAllAnalyticInstances(const unsigned int iServerId, std::vector<unsigned int>& vAnalyticInstances);
	//const std::string& findAnalyticServerStatus(const std::string& host, const std::string& port);
	int updateAnalyticServerStatus(const std::string& sHost, const unsigned int iPort, const std::string& sStatus);
};

} /* namespace db */
} /* namespace result */

#endif /* ANALYTICSERVERGATEWAY_HPP_ */
