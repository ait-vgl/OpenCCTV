/*
 * DbConnection.cpp
 *
 *  Created on: Sep 17, 2014
 *      Author: Anjana Tisera
 */

#include "DbConnector.hpp"

namespace db
{
Connection* DbConnector::getConnection()
{
	Connection* connPrt = NULL;
	Driver* driverPrt;
	string dbUrl, mysqlUsername, mysqlPassword, dbName;
	try
	{
		string mysqlServerName = "mysqlServerName";
		string mysqlPort = "mysqlPort";
		mysqlUsername = "mysqlUsername";
		mysqlPassword = "mysqlPassword";
		dbName = "OpenCCTVDB";

		dbUrl.append("tcp://");
		dbUrl.append(mysqlServerName);
		dbUrl.append(":");
		dbUrl.append(mysqlPort);
	}
	catch(exception &e)
	{
		std::cerr << "Exception: Error in applicationConfiguration.properties file. " << e.what() << std::endl;
		return NULL;
	}

	try
	{
		driverPrt = get_driver_instance();
		connPrt = (*driverPrt).connect(dbUrl, mysqlUsername, mysqlPassword);
		(*connPrt).setSchema(dbName);
	}
	catch(SQLException &e)
	{
		std::cerr << "DbConnector:getConnection: Error while creating connection to the database. " << e.what() << std::endl;
	}
	return connPrt;
}

Connection* DbConnector::getConnection_ResultsDB()
{
	Connection* connPrt = NULL;
	Driver* driverPrt;
	string dbUrl, mysqlUsername, mysqlPassword, dbName;
	try
	{
		string mysqlServerName = "mysqlServerName";
		string mysqlPort = "mysqlPort";
		mysqlUsername = "mysqlUsername";
		mysqlPassword = "mysqlPassword";
		dbName = "OpenCCTVDBResults";

		dbUrl.append("tcp://");
		dbUrl.append(mysqlServerName);
		dbUrl.append(":");
		dbUrl.append(mysqlPort);
	}
	catch(exception &e)
	{
		std::cerr << "Exception: Error in applicationConfiguration.properties file. " << e.what() << std::endl;
		return NULL;
	}

	try
	{
		driverPrt = get_driver_instance();
		connPrt = (*driverPrt).connect(dbUrl, mysqlUsername, mysqlPassword);
		(*connPrt).setSchema(dbName);
	}
	catch(SQLException &e)
	{
		std::cerr << "DbConnector:getConnection: Error while creating connection to the database. " << e.what() << std::endl;
	}
	return connPrt;
}
}
