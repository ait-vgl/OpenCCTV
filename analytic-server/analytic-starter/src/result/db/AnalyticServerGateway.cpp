/*
 * AnalyticServerGateway.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: anjana
 */

#include "AnalyticServerGateway.hpp"

namespace result {
namespace db {

const std::string AnalyticServerGateway::_SELECT_ANALYTIC_INST_SQL = "SELECT ai.id FROM analytic_instances as ai, analytic_servers as ans "
		"WHERE ai.analytic_server_id = ans.id AND ans.id = ?";

const std::string AnalyticServerGateway::_UPDATE_ANALYTIC_SERVER_STATUS_SQL = "UPDATE analytic_servers SET status = ? "
		"WHERE ip = ? AND port = ?;";

AnalyticServerGateway::AnalyticServerGateway() {
	try
	{
		_pDbConnectorPtr = new DbConnector();
		_pConnectionPtr = (*_pDbConnectorPtr).getConnection_OpenCCTVServerDB();
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

//Executed on OpenCCTV Database
void AnalyticServerGateway::findAllAnalyticInstances(const unsigned int iServerId, std::vector<unsigned int>& vAnalyticInstances)
{
	sql::ResultSet* pResultsPtr = NULL;
	sql::PreparedStatement* pStatementPtr = NULL;

	try
	{
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_ANALYTIC_INST_SQL);
		(*pStatementPtr).setUInt(1, iServerId);
		pResultsPtr = (*pStatementPtr).executeQuery();
		unsigned int iAnalyticId = 0;
		while((*pResultsPtr).next())
		{
			iAnalyticId = (*pResultsPtr).getUInt("id");
			vAnalyticInstances.push_back(iAnalyticId);
		}

		if(pStatementPtr){ (*pStatementPtr).close(); delete pStatementPtr; pStatementPtr = NULL;}
		if(pResultsPtr){(*pResultsPtr).close(); delete pResultsPtr; pResultsPtr = NULL;}

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "AnalyticServerGateway::findAllAnalyticInstances : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

//Executed on OpenCCTV Database
int AnalyticServerGateway::updateAnalyticServerStatus(const std::string& sHost,
		const unsigned int iPort, const std::string& sStatus)
{
	int iResult = 0;

	try{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_UPDATE_ANALYTIC_SERVER_STATUS_SQL);
		(*pStatementPtr).setString(1, sStatus);
		(*pStatementPtr).setString(2, sHost);
		(*pStatementPtr).setUInt(3, iPort);
		iResult = (*pStatementPtr).executeUpdate();

	}catch(sql::SQLException &e)
	{
		iResult = -1;
		std::string sErrorMsg = "AnalyticResultGateway::updateAnalyticServerStatus : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}

	//std::cout << "iResult : " << iResult << std::endl;

	return iResult;
}

AnalyticServerGateway::~AnalyticServerGateway() {
	delete _pConnectionPtr; _pConnectionPtr = NULL;
	delete _pDbConnectorPtr; _pDbConnectorPtr = NULL;
}

} /* namespace db */
} /* namespace result */
