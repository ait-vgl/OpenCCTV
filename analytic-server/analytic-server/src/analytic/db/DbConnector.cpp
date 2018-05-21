/*
 * DbConnector.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "DbConnector.hpp"

namespace analytic {
namespace db {

DbConnector::DbConnector()
{
	_pDriverPrt = get_driver_instance();
}

sql::Connection* DbConnector::getConnection_OpenCCTVServerDB()
{
	sql::Connection* connPrt = NULL;
	std::string dbUrl, mysqlUsername, mysqlPassword, dbName, mysqlServerName, mysqlPort;

	try
	{
		analytic::util::Config* pConfig = analytic::util::Config::getInstance();
		mysqlServerName = pConfig->get(analytic::util::PROPERTY_OPENCCTV_DB_SERVER);
		mysqlPort = pConfig->get(analytic::util::PROPERTY_OPENCCTV_DB_PORT);
		dbName = pConfig->get(analytic::util::PROPERTY_OPENCCTV_DB_NAME);
		mysqlUsername = pConfig->get(analytic::util::PROPERTY_OPENCCTV_DB_USERNAME);
		mysqlPassword = pConfig->get(analytic::util::PROPERTY_OPENCCTV_DB_PASSWORD);

		dbUrl.append("tcp://");
		dbUrl.append(mysqlServerName);
		dbUrl.append(":");
		dbUrl.append(mysqlPort);

	}catch(opencctv::Exception &e)
	{
		std::string sErrorMsg = "DbConnector::getConnection_OpenCCTVServerDB : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}

	try
	{
		//driverPrt = get_driver_instance();
		connPrt = (*_pDriverPrt).connect(dbUrl, mysqlUsername, mysqlPassword);
		(*connPrt).setSchema(dbName);
	}
	catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "DbConnector::getConnection_OpenCCTVServerDB : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	return connPrt;
}

sql::Connection* DbConnector::getConnection_AnalyticServerDB()
{
	sql::Connection* connPrt = NULL;
	std::string dbUrl, mysqlUsername, mysqlPassword, dbName, mysqlServerName, mysqlPort;

	try
	{
		analytic::util::Config* pConfig = analytic::util::Config::getInstance();
		mysqlServerName = pConfig->get(analytic::util::PROPERTY_RESULTS_DB_SERVER);
		mysqlPort = pConfig->get(analytic::util::PROPERTY_RESULTS_DB_PORT);
		dbName = pConfig->get(analytic::util::PROPERTY_RESULTS_DB_NAME);
		mysqlUsername = pConfig->get(analytic::util::PROPERTY_RESULTS_DB_USERNAME);
		mysqlPassword = pConfig->get(analytic::util::PROPERTY_RESULTS_DB_PASSWORD);

		dbUrl.append("tcp://");
		dbUrl.append(mysqlServerName);
		dbUrl.append(":");
		dbUrl.append(mysqlPort);

	}catch(opencctv::Exception &e)
	{
		std::string sErrorMsg = "DbConnector::getConnection_AnalyticServerDB : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}

	try
	{
		//driverPrt = get_driver_instance();
		connPrt = (*_pDriverPrt).connect(dbUrl, mysqlUsername, mysqlPassword);
		(*connPrt).setSchema(dbName);
	}
	catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "DbConnector::getConnection_AnalyticServerDB : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	return connPrt;
}

DbConnector::~DbConnector()
{
	_pDriverPrt->threadEnd();
}

} /* namespace db */
} /* namespace analytic */
