/*
 * AnalyticServerGateway.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "AnalyticServerGateway.hpp"

namespace opencctv {
namespace db {


const std::string AnalyticServerGateway::_SQL = "SELECT * FROM analytic_servers;";

AnalyticServerGateway::AnalyticServerGateway()
{
	try
	{
		_pDbConnPtr = DbConnector::getConnection();
		_pStatement = (*_pDbConnPtr).createStatement();
		//_pStatementPtr = NULL;
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticServerGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}

}

AnalyticServerGateway::~AnalyticServerGateway() {
	(*_pStatement).close();
	delete _pStatement; _pStatement = NULL;
	delete _pDbConnPtr; _pDbConnPtr = NULL;
}

void AnalyticServerGateway::findAll(std::vector<opencctv::dto::AnalyticServerDTO>& vAnalyticServers)
{
	try
	{
		sql::ResultSet* pResultsPtr = (*_pStatement).executeQuery(_SQL);
		opencctv::dto::AnalyticServerDTO analyticServer;
		while((*pResultsPtr).next())
		{
			analyticServer.setAnalyticServerId((*pResultsPtr).getInt("id"));
            analyticServer.setIp((*pResultsPtr).getString("ip"));
			analyticServer.setName((*pResultsPtr).getString("name"));
			analyticServer.setPort((*pResultsPtr).getInt("port"));
			analyticServer.setStatus((*pResultsPtr).getString("status"));
			analyticServer.setPid((*pResultsPtr).getInt("pid"));
			analyticServer.setTag((*pResultsPtr).getString("tag"));
			
			vAnalyticServers.push_back(analyticServer);
		}
		(*pResultsPtr).close();
		delete pResultsPtr;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "AnalyticServerGateway:findAll: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
}

} /* namespace db */
} /* namespace opencctv */
