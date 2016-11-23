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
#include "../Exception.hpp"
#include "../util/Config.hpp"
#include "../util/Util.hpp"

namespace opencctv {
namespace db {

class AnalyticInstanceStreamGateway {
private:
	sql::Connection* _pDbConnPtr;
	sql::PreparedStatement* _pStatementPtr;
	static const std::string _SELECT_ANALYTIC_INSTANCE_STREAM_SQL;

public:
	AnalyticInstanceStreamGateway();
    	void findAllForStream(unsigned int iStreamId, std::vector<opencctv::dto::AnalyticInstanceStream>& vToStoreAIS);
	void findAllForStreamByAnalyticInstanceId(unsigned int iStreamId, std::vector<opencctv::dto::AnalyticInstanceStream>& vToStoreAIS, unsigned int iAnalyticInstanceId);
	virtual ~AnalyticInstanceStreamGateway();
};

} /* namespace db */
} /* namespace opencctv */

#endif /* ANALYTICINSTANCESTREAMGATEWAY_HPP_ */
