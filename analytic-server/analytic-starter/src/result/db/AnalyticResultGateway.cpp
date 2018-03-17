/*
 * AnalyticResultGateway.cpp
 *
 *  Created on: May 12, 2017
 *      Author: anjana
 */

#include "AnalyticResultGateway.hpp"

namespace result {
namespace db {

//Executed on the analytic server DB
const std::string AnalyticResultGateway::_SELECT_ANALYTIC_RESULTS_FOR_RAPP_INST_SQL = "SELECT ar.id as result_id, ar.analytic_instance_id, ar.timestamp, ar.result FROM analytic_results as ar, "
		"analytic_results_app_registrations as arar WHERE arar.results_app_instance_id = ? AND ar.analytic_instance_id = arar.analytic_instance_id AND (ar.id, arar.results_app_instance_id) "
		"NOT IN (SELECT sar.analytic_result_id as result_id, sar.results_app_instance_id as inst_id FROM sent_analytic_results as sar WHERE sar.results_app_instance_id = ?) ORDER BY ar.id";

//Executed on the analytic server DB
const std::string AnalyticResultGateway::_SELECT_RESULT_IMAGES_SQL = "SELECT image_file_path FROM images WHERE analytic_result_id = ?";

//Executed on the analytic server DB
const std::string AnalyticResultGateway::_SELECT_RESULT_VIDEOS_SQL = "SELECT video_file_path FROM videos WHERE analytic_result_id = ?";

//Executed on the analytic server DB
const std::string AnalyticResultGateway::_INSERT_SENT_RESULT_SQL = "INSERT INTO sent_analytic_results (analytic_result_id, results_app_instance_id) VALUES(?,?)";

//Executed on the analytic server DB
const std::string AnalyticResultGateway::_SELECT_ANALYTICS_WITH_UNSENT_RSLTS_SQL = "SELECT DISTINCT ar.analytic_instance_id FROM analytic_results as ar, analytic_results_app_registrations as rar "
		"WHERE ar.analytic_instance_id = rar.analytic_instance_id AND (ar.id, rar.results_app_instance_id) NOT IN (SELECT sar.analytic_result_id as result_id, sar.results_app_instance_id as inst_id "
		"FROM sent_analytic_results as sar) ORDER BY ar.id";

//Executed on the analytic server DB
const std::string AnalyticResultGateway::_SELECT_RAPP_INST_FOR_ANALYTIC_INST_SQL = "SELECT arar.results_app_instance_id "
		"FROM analytic_results_app_registrations as arar "
		"WHERE arar.analytic_instance_id = ?";

AnalyticResultGateway::AnalyticResultGateway()
{
	try
	{
		_pDbConnectorPtr = new DbConnector();
		_pConnectionPtr = (*_pDbConnectorPtr).getConnection_AnalyticServerDB();
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticResultGateway. ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}
}

//Used==========
void AnalyticResultGateway::findAnalyticResultsForRAppInst(const unsigned int iRAppInstId,
		std::vector<result::db::dto::AnalyticResult>& vAnalyticResults)

{
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_ANALYTIC_RESULTS_FOR_RAPP_INST_SQL);
		(*pStatementPtr).setInt(1, iRAppInstId);
		(*pStatementPtr).setInt(2, iRAppInstId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		result::db::dto::AnalyticResult analyticResult;
		while((*pResultsPtr).next())
		{
			analyticResult.setResultsId((*pResultsPtr).getUInt("result_id"));
			analyticResult.setAnalyticInstId((*pResultsPtr).getUInt("analytic_instance_id"));
			analyticResult.setTimestamp((*pResultsPtr).getString("timestamp"));
			analyticResult.setResult((*pResultsPtr).getString("result"));
			vAnalyticResults.push_back(analyticResult);
		}
		(*pResultsPtr).close();
		(*pStatementPtr).close();
		delete pResultsPtr; pResultsPtr = NULL;
		delete pStatementPtr; pStatementPtr = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "AnalyticResultGateway::findAnalyticResultsForRAppInst: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

//Used==========
void AnalyticResultGateway::findResultImages(const unsigned int iResultId,
		std::map<std::string, std::string>& mapImages)
{
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_RESULT_IMAGES_SQL);
		(*pStatementPtr).setInt(1, iResultId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		std::string sImagePath;
		while((*pResultsPtr).next())
		{
			//TODO At the moment full path to images are stored in the DB
			//If not modify the filename, path info as necessary before inserting to the mapImages
			sImagePath = (*pResultsPtr).getString("image_file_path");
			boost::filesystem::path sPath(sImagePath);
			mapImages.insert(std::pair<std::string,std::string>(sPath.filename().string(),sImagePath));
		}
		(*pResultsPtr).close();
		(*pStatementPtr).close();
		delete pResultsPtr; pResultsPtr = NULL;
		delete pStatementPtr; pStatementPtr = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "AnalyticResultGateway::findResultImages : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

//Used==========
void AnalyticResultGateway::findResultVideos(const unsigned int iResultId,
		std::map<std::string, std::string>& mapVideos)
{
	try
	{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_SELECT_RESULT_VIDEOS_SQL);
		(*pStatementPtr).setInt(1, iResultId);
		sql::ResultSet* pResultsPtr = (*pStatementPtr).executeQuery();
		std::string sVideoPath;
		while((*pResultsPtr).next())
		{
			//TODO At the moment full path to videos are stored in the DB
			//If not modify the filename, path info as necessary before inserting to the mapVideos
			sVideoPath = (*pResultsPtr).getString("video_file_path");
			boost::filesystem::path sPath(sVideoPath);
			mapVideos.insert(std::pair<std::string,std::string>(sPath.filename().string(),sVideoPath));
		}
		(*pResultsPtr).close();
		(*pStatementPtr).close();
		delete pResultsPtr; pResultsPtr = NULL;
		delete pStatementPtr; pStatementPtr = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "AnalyticResultGateway::findResultImages : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

//Used==========
int AnalyticResultGateway::insertToSentResults(const unsigned int iResultId,
		const unsigned int iRAppInstId)
{
	int iResult = 0;

	try{
		sql::PreparedStatement* pStatementPtr;
		pStatementPtr = (*_pConnectionPtr).prepareStatement(_INSERT_SENT_RESULT_SQL);
		(*pStatementPtr).setInt(1, iResultId);
		(*pStatementPtr).setInt(2, iRAppInstId);
		iResult = (*pStatementPtr).executeUpdate();

	}catch(sql::SQLException &e)
	{
		iResult = -1;
		std::string sErrorMsg = "AnalyticResultGateway::insertToSentResults : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
	return iResult;
}

//Used==========
void AnalyticResultGateway::findAnalyticsWithUnsentResults(std::vector<int>& vAnalyticInstanceIds)
{
	try
	{
		sql::Statement* pStatement = (*_pConnectionPtr).createStatement();
		sql::ResultSet* pResultsPtr = (*pStatement).executeQuery(_SELECT_ANALYTICS_WITH_UNSENT_RSLTS_SQL);
		while((*pResultsPtr).next())
		{
			vAnalyticInstanceIds.push_back((*pResultsPtr).getUInt("analytic_instance_id"));
		}
		(*pResultsPtr).close();
		(*pStatement).close();
		delete pResultsPtr; pResultsPtr = NULL;
		delete pStatement; pStatement = NULL;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "ResultsAppInstanceGateway::findAnalyticsWithUnsentResults: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}
}

//Used==========
void AnalyticResultGateway::findRAppInstancesForAnalyticInst(
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
}

AnalyticResultGateway::~AnalyticResultGateway()
{
	delete _pConnectionPtr; _pConnectionPtr = NULL;
	delete _pDbConnectorPtr; _pDbConnectorPtr = NULL;
}

} /* namespace db */
} /* namespace result */
