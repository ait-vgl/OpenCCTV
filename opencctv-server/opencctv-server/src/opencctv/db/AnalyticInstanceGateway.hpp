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

#include "../dto/AnalyticInstance.hpp"

#include "../Exception.hpp"
#include "../util/Config.hpp"
#include "../util/Util.hpp"

namespace opencctv {
namespace db {

class AnalyticInstanceGateway {
private:
    sql::Connection* _pDbConnPtr;
	sql::PreparedStatement* _pStatementPtr;

public:
	AnalyticInstanceGateway();
	virtual ~AnalyticInstanceGateway();

    void findAll(std::vector<opencctv::dto::AnalyticInstance>& vToStoreAIS);
	void findAnalyticInstance(std::vector<opencctv::dto::AnalyticInstance>& vToStoreAIS, unsigned int iAnalyticInstanceId);
    int updateStatus(unsigned int iAnalyticInstanceId, int iStatus);
};

} /* namespace db */
} /* namespace opencctv */

#endif /* ANALYTICINSTANCEGATEWAY_HPP_ */
