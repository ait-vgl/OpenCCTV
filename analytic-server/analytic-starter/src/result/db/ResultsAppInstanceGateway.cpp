/*
 * ResultsAppInstanceGateway.cpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#include "ResultsAppInstanceGateway.hpp"

namespace result {
namespace db {

const std::string ResultsAppInstanceGateway::_SELECT_ALL_RAPP_INST_SQL = "SELECT rai.id as inst_id, "
		"rai.name as inst_name, rac.id as conn_id, rac.name as conn_name, rac.lib_file_path "
		"FROM results_app_instances as rai, results_apps as ra, results_app_connectors rac "
		"WHERE rai.results_app_id = ra.id AND ra.results_app_connector_id = rac.id";

const std::string ResultsAppInstanceGateway::_SELECT_RAPP_INST_SQL = "SELECT rai.id as inst_id, "
		"rai.name as inst_name, rac.id as conn_id, rac.name as conn_name, rac.lib_file_path FROM "
		"results_app_instances as rai, results_apps as ra, results_app_connectors rac WHERE "
		"rai.results_app_id = ra.id AND ra.results_app_connector_id = rac.id AND rai.id = ?";

const std::string ResultsAppInstanceGateway::_SELECT_ALL_RAPP_INST_FILE_SQL = "SELECT raif.file_name, "
		"raif.file_path FROM results_app_instances as rai, results_app_instance_files as raif "
		"WHERE raif.results_app_instance_id = rai.id AND rai.id = ?";

const std::string ResultsAppInstanceGateway::_SELECT_ALL_RAPP_INST_PARAM_SQL = "SELECT raip.param_name, "
		"raip.param_value FROM results_app_instances as rai, "
		"results_app_instance_parameters as raip "
		"WHERE raip.results_app_instance_id = rai.id AND rai.id = ?";

const std::string ResultsAppInstanceGateway::_SELECT_RAPP_INST_FOR_ANALYTIC_INST_SQL = "SELECT rai.id as inst_id, "
		"rai.name as inst_name, rac.id as conn_id, rac.name as conn_name, rac.lib_file_path FROM "
		"results_app_registrations as rar, results_app_instances as rai, results_apps as ra, "
		"results_app_connectors as rac WHERE rar.results_app_instance_id = rai.id AND rai.results_app_id = ra.id "
		"AND ra.results_app_connector_id = rac.id AND rar.analytic_instance_id = ? ";


ResultsAppInstanceGateway::ResultsAppInstanceGateway()
{
	try
	{
		_pDbConnectorPtr = new DbConnector();
		_pConnectionPtr = (*_pDbConnectorPtr).getConnection();
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the ResultsAppInstanceGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}
}

void ResultsAppInstanceGateway::findAllRAppInstances(
		std::vector<result::db::dto::ResultsAppInstance>& vResultsAppInstance)
{
	try
	{
		sql::Statement* pStatement = (*_pConnectionPtr).createStatement();
		sql::ResultSet* pResultsPtr = (*pStatement).executeQuery(_SELECT_ALL_RAPP_INST_SQL);
		result::db::dto::ResultsAppInstance resultsAppInst;
		while((*pResultsPtr).next())
		{
			resultsAppInst.setResultsAppInstanceId((*pResultsPtr).getUInt("inst_id"));
			resultsAppInst.setResultsAppInstanceName((*pResultsPtr).getString("inst_name"));
			resultsAppInst.setResultsAppConnectorId((*pResultsPtr).getUInt("conn_id"));
			resultsAppInst.setResultsAppConnectorName((*pResultsPtr).getString("conn_name"));
			resultsAppInst.setLibFilePath((*pResultsPtr).getString("lib_file_path"));
			vResultsAppInstance.push_back(resultsAppInst);
		}
		(*pResultsPtr).close();
		(*pStatement).close();
		delete pResultsPtr; pResultsPtr = NULL;
		delete pStatement; pStatement = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "ResultsAppInstanceGateway::findAllRAppInstances: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

void ResultsAppInstanceGateway::findRAppInstance(const unsigned int iRAppInstanceId,
		result::db::dto::ResultsAppInstance& resultsAppInstance)
{
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_RAPP_INST_SQL);
		(*pStatementPtr).setInt(1, iRAppInstanceId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		//result::db::dto::ResultsAppInstance resultsAppInst;
		while((*pResultsPtr).next())
		{
			resultsAppInstance.setResultsAppInstanceId((*pResultsPtr).getUInt("inst_id"));
			resultsAppInstance.setResultsAppInstanceName((*pResultsPtr).getString("inst_name"));
			resultsAppInstance.setResultsAppConnectorId((*pResultsPtr).getUInt("conn_id"));
			resultsAppInstance.setResultsAppConnectorName((*pResultsPtr).getString("conn_name"));
			resultsAppInstance.setLibFilePath((*pResultsPtr).getString("lib_file_path"));
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

void ResultsAppInstanceGateway::findRAppInstancesForAnalyticInst(
		const unsigned int iAnalyticInstId,
		std::vector<result::db::dto::ResultsAppInstance>& vResultsAppInstance)
{
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_RAPP_INST_FOR_ANALYTIC_INST_SQL);
		(*pStatementPtr).setInt(1, iAnalyticInstId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		result::db::dto::ResultsAppInstance resultsAppInst;
		while((*pResultsPtr).next())
		{
			resultsAppInst.setResultsAppInstanceId((*pResultsPtr).getUInt("inst_id"));
			resultsAppInst.setResultsAppInstanceName((*pResultsPtr).getString("inst_name"));
			resultsAppInst.setResultsAppConnectorId((*pResultsPtr).getUInt("conn_id"));
			resultsAppInst.setResultsAppConnectorName((*pResultsPtr).getString("conn_name"));
			resultsAppInst.setLibFilePath((*pResultsPtr).getString("lib_file_path"));
			vResultsAppInstance.push_back(resultsAppInst);
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

void ResultsAppInstanceGateway::findRAppInstanceFiles(const int iRAppInstId,
		std::map<std::string, std::string>& mapInputFiles)
{
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_ALL_RAPP_INST_FILE_SQL);
		(*pStatementPtr).setInt(1, iRAppInstId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		while((*pResultsPtr).next())
		{

			mapInputFiles.insert (std::pair<std::string,std::string>(
					(*pResultsPtr).getString("file_name"),
					(*pResultsPtr).getString("file_path")));
		}
		(*pResultsPtr).close();
		(*pStatementPtr).close();
		delete pResultsPtr; pResultsPtr = NULL;
		delete pStatementPtr; pStatementPtr = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "ResultsAppInstanceGateway::findRAppInstanceFiles : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

void ResultsAppInstanceGateway::findRAppInstanceParams(const int iRAppInstId,
		std::map<std::string, std::string>& mapInputParams)
{
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_ALL_RAPP_INST_PARAM_SQL);
		(*pStatementPtr).setInt(1, iRAppInstId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		while((*pResultsPtr).next())
		{

			mapInputParams.insert (std::pair<std::string,std::string>(
					(*pResultsPtr).getString("param_name"),
					(*pResultsPtr).getString("param_value")));
		}
		(*pResultsPtr).close();
		(*pStatementPtr).close();
		delete pResultsPtr; pResultsPtr = NULL;
		delete pStatementPtr; pStatementPtr = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "ResultsAppInstanceGateway::findRAppInstanceParams : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

ResultsAppInstanceGateway::~ResultsAppInstanceGateway()
{
	delete _pConnectionPtr; _pConnectionPtr = NULL;
	delete _pDbConnectorPtr; _pDbConnectorPtr = NULL;
}

} /* namespace db */
} /* namespace result */
