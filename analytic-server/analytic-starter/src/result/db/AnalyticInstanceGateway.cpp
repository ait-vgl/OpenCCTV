/*
 * AnalyticInstanceGateway.cpp
 *
 *  Created on: May 12, 2017
 *      Author: anjana
 */

#include "AnalyticInstanceGateway.hpp"

namespace result {
namespace db {

/*const std::string AnalyticInstanceGateway::_SELECT_ANALYTIC_INST_SQL = "SELECT * "
		"FROM analytic_server_db.analytic_instances WHERE id = ?";*/

//Executed on OpenCCTV server DB
const std::string AnalyticInstanceGateway::_SELECT_ANALYTIC_INST_SQL = "SELECT ai.id, ai.name, ai.description FROM analytic_instances as ai WHERE ai.id = ?";

/*const std::string AnalyticInstanceGateway::_SELECT_ANALYTIC_INST_FOR_RAPP_INST_SQL = "SELECT ai.id, ai.opencctv_id, ai.name, ai.description, ai.location "
		"FROM analytic_instances as ai, results_app_registrations as rar WHERE ai.id = rar.analytic_instance_id AND rar.results_app_instance_id = ?";*/

//Executed on OpenCCTV server DB
const std::string AnalyticInstanceGateway::_SELECT_ANALYTIC_INST_FOR_RAPP_INST_SQL = "SELECT ai.id, ai.name, ai.description FROM analytic_instances as ai, "
		"analytic_instance_results_apps as aira WHERE ai.id = aira.analytic_instance_id AND aira.results_app_id = ?";

//Executed on OpenCCTV server DB
const std::string AnalyticInstanceGateway::_UPDATE_ANALYTIC_INST_STATUS_SQL = "UPDATE analytic_instances SET status = ? WHERE id = ?";

AnalyticInstanceGateway::AnalyticInstanceGateway()
{
	try
	{
		_pDbConnectorPtr = new DbConnector();
		_pConnectionPtr = (*_pDbConnectorPtr).getConnection_OpenCCTVServerDB();
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticInstanceGateway. ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}
}

//Used==========
void AnalyticInstanceGateway::findAnalyticInstance(const unsigned int iAnalyticInstanceId,
			result::db::dto::AnalyticInstance& analyticInstance)
{
	try
		{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_ANALYTIC_INST_SQL);
		(*pStatementPtr).setInt(1, iAnalyticInstanceId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		while((*pResultsPtr).next())
		{
			analyticInstance.setAnalyticInstId((*pResultsPtr).getUInt("id"));
			analyticInstance.setName((*pResultsPtr).getString("name"));
			analyticInstance.setDescription((*pResultsPtr).getString("description"));
		}
		(*pResultsPtr).close();
		(*pStatementPtr).close();
		delete pResultsPtr; pResultsPtr = NULL;
		delete pStatementPtr; pStatementPtr = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "ResultsAppInstanceGateway::findRAppInstancesForAnalyticInst: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

//Used==========
void AnalyticInstanceGateway::findAnalyticInstancesForRAppInst(
		const unsigned int iRAppInstId,
		std::vector<result::db::dto::AnalyticInstance>& vAnalyticInstances)

{
	try
	{
	sql::PreparedStatement* pStatementPtr;
	pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_ANALYTIC_INST_FOR_RAPP_INST_SQL);
	(*pStatementPtr).setInt(1, iRAppInstId);
	sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
	result::db::dto::AnalyticInstance analyticInstance;
	while((*pResultsPtr).next())
	{
		analyticInstance.setAnalyticInstId((*pResultsPtr).getUInt("id"));
		//analyticInstance.setOpenCctvId((*pResultsPtr).getUInt("opencctv_id"));
		analyticInstance.setName((*pResultsPtr).getString("name"));
		analyticInstance.setDescription((*pResultsPtr).getString("description"));
		//analyticInstance.setLocation((*pResultsPtr).getString("location"));
		vAnalyticInstances.push_back(analyticInstance);
	}
	(*pResultsPtr).close();
	(*pStatementPtr).close();
	delete pResultsPtr; pResultsPtr = NULL;
	delete pStatementPtr; pStatementPtr = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "ResultsAppInstanceGateway::findRAppInstancesForAnalyticInst: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

//Used==========
int AnalyticInstanceGateway::updateStatus(const unsigned int iAnalyticInstanceId, const unsigned int iStatus)
{
	int iResult = 0;

	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_UPDATE_ANALYTIC_INST_STATUS_SQL);
		(*pStatementPtr).setInt(1, iStatus);
		(*pStatementPtr).setInt(2, iAnalyticInstanceId);
		iResult = (*pStatementPtr).executeUpdate();
		(*pStatementPtr).close();
		delete pStatementPtr; pStatementPtr = NULL;
	}catch(sql::SQLException &e)
	{
		iResult = -1;
		std::string sErrorMsg = "AnalyticInstanceGateway::updateStatus : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}

	return iResult;
}

AnalyticInstanceGateway::~AnalyticInstanceGateway()
{
	delete _pConnectionPtr; _pConnectionPtr = NULL;
	delete _pDbConnectorPtr; _pDbConnectorPtr = NULL;
}

} /* namespace db */
} /* namespace result */
