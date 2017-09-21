/*
 * ResultsAppInstanceGateway.hpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#ifndef RESULTSAPPINSTANCEGATEWAY_HPP_
#define RESULTSAPPINSTANCEGATEWAY_HPP_

#include <vector>
#include <string>
#include <sstream>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "DbConnector.hpp"
#include "dto/ResultsAppInstance.hpp"
#include "../../opencctv/Exception.hpp"
#include "../../analytic/util/Config.hpp"
#include "../../opencctv/util/Util.hpp"
#include "../../opencctv/util/log/Loggers.hpp"

namespace result {
namespace db {

class ResultsAppInstanceGateway
{
private:
	DbConnector* _pDbConnectorPtr;
	sql::Connection* _pConnectionPtr;
	static const std::string _SELECT_ALL_RAPP_INST_SQL;
	static const std::string _SELECT_ALL_RAPP_INST_FILE_SQL;
	static const std::string _SELECT_ALL_RAPP_INST_PARAM_SQL;
	static const std::string _SELECT_RAPP_INST_FOR_ANALYTIC_INST_SQL;
	static const std::string _SELECT_RAPP_INST_SQL;
public:
	ResultsAppInstanceGateway();
	void findAllRAppInstances(
			std::vector<result::db::dto::ResultsAppInstance>& vResultsAppInstance);
	void findRAppInstance(
			const unsigned int iRAppInstanceId,
			result::db::dto::ResultsAppInstance& resultsAppInstance);
	/*void findRAppInstancesForAnalyticInst(const unsigned int iAnalyticInstId,
			std::vector<unsigned int>& vResultsAppInstanceIds);*/
	void findRAppInstanceFiles(const int iRAppInstId,
			std::map<std::string, std::string>& mapInputFiles);
	void findRAppInstanceParams(const int iRAppInstId,
				std::map<std::string, std::string>& mapInputFiles);
	virtual ~ResultsAppInstanceGateway();
};

} /* namespace db */
} /* namespace result */

#endif /* RESULTSAPPINSTANCEGATEWAY_HPP_ */
