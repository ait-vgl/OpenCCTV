/*
 * AnalyticResultsGateway.hpp
 *
 *  Created on: Aug 30, 2017
 *      Author: anjana
 */

#ifndef ANALYTICRESULTSGATEWAY_HPP_
#define ANALYTICRESULTSGATEWAY_HPP_

#include <sstream>
#include <vector>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "DbConnector.hpp"
#include "../../opencctv/Exception.hpp"

namespace analytic {
namespace db {

class AnalyticResultsGateway
{
private:
	sql::Connection* _pDbConnPtr;
	DbConnector* _pDbConnectorPtr;

	static const std::string _INSERT_RESULT_SQL;
	static const std::string _SELECT_ANALYTIC_INST_MAX_RESULTS_ID;
	static const std::string _INSERT_FILES_SQL;
	static const std::string _INSERT_IMAGES_SQL;
	static const std::string _INSERT_VIDEOS_SQL;

public:
	AnalyticResultsGateway();
	int insertResults(const unsigned int iAnalyticIinstId, const std::string& sTimestamp, const std::string& sResult);
	int insertFiles(const int iResultsId, const std::vector<std::string>* pVFiles, const std::string& sFileType);
	virtual ~AnalyticResultsGateway();
};

} /* namespace db */
} /* namespace analytic */

#endif /* ANALYTICRESULTSGATEWAY_HPP_ */
