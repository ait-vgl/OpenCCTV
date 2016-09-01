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
#include "../Exception.hpp"
#include "../util/Config.hpp"

namespace opencctv {
namespace db {

class DbConnector {
public:
	/**
	 * Creates connection to a particular mysql db
	 * using the given server IP, port number, username, password and database name.
	 */
	static sql::Connection* getConnection();
	static sql::Connection* getConnection_ResultsDB();
};

} /* namespace db */
} /* namespace opencctv */

#endif /* DBCONNECTOR_HPP_ */
