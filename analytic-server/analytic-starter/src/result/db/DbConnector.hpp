/*
 * DbConnector.hpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#ifndef DBCONNECTOR_HPP_
#define DBCONNECTOR_HPP_

#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>
#include "../../opencctv/Exception.hpp"
#include "../../analytic/util/Config.hpp"

namespace result {
namespace db {

class DbConnector
{
private:
	sql::Driver* _driverPrt;

public:
	DbConnector();
	virtual ~DbConnector();

	/**
	 * Creates connection to a analytic_server_db mysql db
	 * using the given server IP, port number, username, password and database name.
	 */
	sql::Connection* getConnection_AnalyticServerDB();

	/**
	 * Creates connection to a opencctv_db mysql db
	 * using the given server IP, port number, username, password and database name.
	 */
	sql::Connection* getConnection_OpenCCTVServerDB();
};

} /* namespace db */
} /* namespace result */

#endif /* DBCONNECTOR_HPP_ */
