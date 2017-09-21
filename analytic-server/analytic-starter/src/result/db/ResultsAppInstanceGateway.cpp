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

/*const std::string ResultsAppInstanceGateway::_SELECT_RAPP_INST_SQL = "SELECT rai.id as inst_id, "
		"rai.name as inst_name, rac.id as conn_id, rac.name as conn_name, rac.lib_file_path FROM "
		"results_app_instances as rai, results_apps as ra, results_app_connectors rac WHERE "
		"rai.results_app_id = ra.id AND ra.results_app_connector_id = rac.id AND rai.id = ?";*/

//Executed on the OpenCCTV DB
const std::string ResultsAppInstanceGateway::_SELECT_RAPP_INST_SQL = "SELECT ra.id, ra.name, rac.id as conn_id, "
		"rac.name as conn_name, rac.filename FROM results_apps as ra, results_app_connectors rac "
		"WHERE ra.results_app_connector_id = rac.id AND ra.id = ?";

/*const std::string ResultsAppInstanceGateway::_SELECT_ALL_RAPP_INST_FILE_SQL = "SELECT raif.file_name, "
		"raif.file_path FROM results_app_instances as rai, results_app_instance_files as raif "
		"WHERE raif.results_app_instance_id = rai.id AND rai.id = ?";*/

//Executed on the OpenCCTV DB
const std::string ResultsAppInstanceGateway::_SELECT_ALL_RAPP_INST_FILE_SQL = "SELECT racf.name as file_name, raif.filename as file_path "
		"FROM  results_app_input_files as raif, results_app_connector_files as racf "
		"WHERE raif.results_app_connector_file_id = racf.id AND raif.results_app_id = ?";

/*const std::string ResultsAppInstanceGateway::_SELECT_ALL_RAPP_INST_PARAM_SQL = "SELECT raip.param_name, "
		"raip.param_value FROM results_app_instances as rai, "
		"results_app_instance_parameters as raip "
		"WHERE raip.results_app_instance_id = rai.id AND rai.id = ?";*/

//Executed on the OpenCCTV DB
const std::string ResultsAppInstanceGateway::_SELECT_ALL_RAPP_INST_PARAM_SQL = "SELECT racp.name, raip.value "
		"FROM  results_app_input_parameters as raip, results_app_connector_parameters as racp "
		"WHERE raip.results_app_connector_parameter_id = racp.id AND raip.results_app_id = ?";

/*const std::string ResultsAppInstanceGateway::_SELECT_RAPP_INST_FOR_ANALYTIC_INST_SQL = "SELECT rai.id as inst_id, "
		"rai.name as inst_name, rac.id as conn_id, rac.name as conn_name, rac.lib_file_path FROM "
		"results_app_registrations as rar, results_app_instances as rai, results_apps as ra, "
		"results_app_connectors as rac WHERE rar.results_app_instance_id = rai.id AND rai.results_app_id = ra.id "
		"AND ra.results_app_connector_id = rac.id AND rar.analytic_instance_id = ?"*/

//Moved
/*const std::string ResultsAppInstanceGateway::_SELECT_RAPP_INST_FOR_ANALYTIC_INST_SQL = "SELECT arar.results_app_instance_id "
		"FROM analytic_results_app_registrations as arar WHERE arar.analytic_instance_id = ?"*/;


ResultsAppInstanceGateway::ResultsAppInstanceGateway()
{
	try
	{
		_pDbConnectorPtr = new DbConnector();
		_pConnectionPtr = (*_pDbConnectorPtr).getConnection_OpenCCTVServerDB();
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

//Used =================
void ResultsAppInstanceGateway::findRAppInstance(const unsigned int iRAppInstanceId,
		result::db::dto::ResultsAppInstance& resultsAppInstance)
{
	std::stringstream ssMsg;
	std::string sLibFolder = "";
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_RAPP_INST_SQL);
		(*pStatementPtr).setInt(1, iRAppInstanceId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		//result::db::dto::ResultsAppInstance resultsAppInst;
		while((*pResultsPtr).next())
		{
			resultsAppInstance.setResultsAppInstanceId((*pResultsPtr).getUInt("id"));
			resultsAppInstance.setResultsAppInstanceName((*pResultsPtr).getString("name"));
			resultsAppInstance.setResultsAppConnectorId((*pResultsPtr).getUInt("conn_id"));
			resultsAppInstance.setResultsAppConnectorName((*pResultsPtr).getString("conn_name"));
			sLibFolder = (*pResultsPtr).getString("filename");
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

	/*The filename returned by the query is the folder/directory name in which the library
	 * file contains. Thus need to find the full path to the library in this directory.
	 */
	analytic::util::Config* _pConfig =  NULL;
	std::string sResultsAppConnectorPath = "";
	std::string sLibFilePath = "";
	try
	{
		_pConfig = analytic::util::Config::getInstance();

	}catch(opencctv::Exception& e)
	{

		ssMsg << "ResultsAppInstanceGateway::findRAppInstance - Error reading from the analytic server configuration file" << ".\n";;
		ssMsg << e.what();
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
	}

	if(_pConfig)
	{
		sResultsAppConnectorPath = _pConfig->get(analytic::util::PROPERTY_RESULTS_APP_CONNECTOR_DIR);
	}

	if(*sResultsAppConnectorPath.rbegin() != '/')
	{
		sResultsAppConnectorPath.append("/");
	}
	sResultsAppConnectorPath.append(sLibFolder);
	if(opencctv::util::Util::findSharedLibOfPlugin(sResultsAppConnectorPath,sLibFilePath))
	{
		resultsAppInstance.setLibFilePath(sResultsAppConnectorPath.append(sLibFilePath));
	}else
	{
		resultsAppInstance.setLibFilePath("");
		ssMsg.clear();
		ssMsg << "ResultsAppInstanceGateway::findRAppInstance - ";
		ssMsg << "Failed to find the connector library for results application ";
		ssMsg << resultsAppInstance.getResultsAppConnectorName();
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
	}
}

/*//Moved=================
void ResultsAppInstanceGateway::findRAppInstancesForAnalyticInst(
		const unsigned int iAnalyticInstId,
		std::vector<unsigned int>& vResultsAppInstanceIds)
{
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_RAPP_INST_FOR_ANALYTIC_INST_SQL);
		(*pStatementPtr).setInt(1, iAnalyticInstId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		//result::db::dto::ResultsAppInstance resultsAppInst;
		unsigned int iRAppInstId = 0;
		while((*pResultsPtr).next())
		{
			iRAppInstId = (*pResultsPtr).getUInt("results_app_instance_id");
			vResultsAppInstanceIds.push_back(iRAppInstId);
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
}*/

//Used==========
void ResultsAppInstanceGateway::findRAppInstanceFiles(const int iRAppInstId,
		std::map<std::string, std::string>& mapInputFiles)
{
	/*The file_path returned by the query is not the absolute path.
	 * Thus need to find the absolute path of input files.
	 */
	analytic::util::Config* _pConfig =  NULL;
	std::string sResultsAppInputFilePath = "";
	std::string sLibFilePath = "";
	std::stringstream ssMsg;
	try
	{
		_pConfig = analytic::util::Config::getInstance();

	}catch(opencctv::Exception& e)
	{

		ssMsg << "ResultsAppInstanceGateway::findRAppInstanceFiles - Error reading from the analytic server configuration file" << ".\n";
		ssMsg << e.what();
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
	}

	if(_pConfig)
	{
		sResultsAppInputFilePath = _pConfig->get(analytic::util::PROPERTY_RESULTS_APP_INPUT_FILE_DIR);
	}

	if(sResultsAppInputFilePath.empty())
	{
		ssMsg << "ResultsAppInstanceGateway::findRAppInstanceFiles - Failed to read the results application input file path";
		opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
		return;
	}

	//Example Path - /usr/local/opencctv/results_app_input_files/22/config.json
	//File Path - <analytic::util::PROPERTY_RESULTS_APP_INPUT_FILE_DIR>/<resultsAppInstId>/<filename>
	if(*sResultsAppInputFilePath.rbegin() != '/')
	{
		sResultsAppInputFilePath.append("/");
	}

	ssMsg << sResultsAppInputFilePath << iRAppInstId << "/";
	sResultsAppInputFilePath = ssMsg.str();

	//Running the query
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
					sResultsAppInputFilePath.append((*pResultsPtr).getString("file_path"))));
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

//Used==========
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
					(*pResultsPtr).getString("name"),
					(*pResultsPtr).getString("value")));
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
