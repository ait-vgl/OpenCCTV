/*
 * AnalyticResultGateway.hpp
 *
 *  Created on: May 12, 2017
 *      Author: anjana
 */

#ifndef ANALYTICRESULTGATEWAY_HPP_
#define ANALYTICRESULTGATEWAY_HPP_

#include <vector>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <boost/filesystem.hpp>
#include "DbConnector.hpp"
#include "dto/AnalyticResult.hpp"
#include "../../opencctv/Exception.hpp"

namespace result {
namespace db {

class AnalyticResultGateway
{
private:
	DbConnector* _pDbConnectorPtr;
	sql::Connection* _pConnectionPtr;
	static const std::string _SELECT_ANALYTIC_RESULTS_FOR_RAPP_INST_SQL;
	static const std::string _SELECT_RESULT_IMAGES_SQL;
	static const std::string _SELECT_RESULT_VIDEOS_SQL;
	static const std::string _INSERT_SENT_RESULT_SQL;
	static const std::string _SELECT_ANALYTICS_WITH_UNSENT_RSLTS_SQL;
	static const std::string _SELECT_RAPP_INST_FOR_ANALYTIC_INST_SQL;

public:
	AnalyticResultGateway();
	virtual ~AnalyticResultGateway();
	void findAnalyticResultsForRAppInst(const unsigned int iRAppInstId,
			std::vector<result::db::dto::AnalyticResult>& vAnalyticResults);
	void findResultImages(const unsigned int iResultId,
				std::map<std::string, std::string>& mapImages);
	void findResultVideos(const unsigned int iResultId,
					std::map<std::string, std::string>& mapVideos);
	int insertToSentResults(const unsigned int iResultId, const unsigned int iRAppInstId);
	void findAnalyticsWithUnsentResults(std::vector<int>& vAnalyticInstanceIds);
	void findRAppInstancesForAnalyticInst(const unsigned int iAnalyticInstId,
				std::vector<unsigned int>& vResultsAppInstanceIds);
};

} /* namespace db */
} /* namespace result */

#endif /* ANALYTICRESULTGATEWAY_HPP_ */
