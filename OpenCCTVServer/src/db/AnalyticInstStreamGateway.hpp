/*
 * AnalyticInstStreamGateway.hpp
 *
 *  Created on: Oct 28, 2014
 *      Author: anjana
 */

#ifndef ANALYTICINSTSTREAMGATEWAY_HPP_
#define ANALYTICINSTSTREAMGATEWAY_HPP_

#include <vector>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "DbConnector.hpp"
#include "AnalyticInstStream.hpp"

using namespace std;

namespace db {

class AnalyticInstStreamGateway {
private:
	Connection* _pDbConn;
	static const string _SELECT_ANALYTICINSTSTREAM_INFO_SQL;

public:
	AnalyticInstStreamGateway();
	virtual ~AnalyticInstStreamGateway();

	//Find all the analytics instances registered for a particular stream of a camera
	vector<AnalyticInstStream> findAnalyticInstances(unsigned int iStreamId);
};

} /* namespace db */

#endif /* ANALYTICINSTSTREAMGATEWAY_HPP_ */
