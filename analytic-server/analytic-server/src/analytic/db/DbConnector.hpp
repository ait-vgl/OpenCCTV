/*
 * DbConnector.hpp
 *
 *  Created on: Jun 3, 2015
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
#include "../util/Config.hpp"
#include "../../opencctv/util/log/Loggers.hpp"

namespace analytic {
namespace db {

class DbConnector
{
private:
	sql::Driver* _pDriverPrt;
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
} /* namespace analytic */

#endif /* DBCONNECTOR_HPP_ */
