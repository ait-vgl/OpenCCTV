/*
 * DbConnection.hpp
 *
 *  Created on: Sep 17, 2014
 *      Author: Anjana Tisera
 */

#ifndef DBCONNECTOR_HPP_
#define DBCONNECTOR_HPP_

#include <cppconn/statement.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sql;

namespace db
{
/**
 * This static class creates connection to mysql database.
 */
class DbConnector
{
public:
	/**
	 * Creates connection to a particular mysql db
	 * using the given server IP, port number, username, password and database name.
	 */
	static Connection* getConnection();
	static Connection* getConnection_ResultsDB();
};
}

#endif /* DBCONNECTOR_HPP_ */
