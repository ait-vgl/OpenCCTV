/*
 * AnalyticResultsGateway.cpp
 *
 *  Created on: Aug 30, 2017
 *      Author: anjana
 */

#include "AnalyticResultsGateway.hpp"

namespace analytic {
namespace db {

//Executed on the analytic server DB
const std::string AnalyticResultsGateway::_INSERT_RESULT_SQL = "INSERT INTO analytic_results(analytic_instance_id, timestamp, result) VALUES (?,?,?)";

//Executed on the analytic server DB
const std::string AnalyticResultsGateway::_SELECT_ANALYTIC_INST_MAX_RESULTS_ID = "SELECT MAX(id) as result_id FROM analytic_results WHERE analytic_instance_id = ?";

//Executed on the analytic server DB
//const std::string AnalyticResultsGateway::_INSERT_FILES_SQL = "SELECT MAX(id) FROM analytic_results WHERE analytic_instance_id = ?";

AnalyticResultsGateway::AnalyticResultsGateway()
{
	try
	{
		_pDbConnectorPtr = new DbConnector();
		_pDbConnPtr = (*_pDbConnectorPtr).getConnection_AnalyticServerDB();
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticResultsGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}

}

/**
 * If successful insert the analytic results and return the auto generated ID
 * for that results entry in the DB
 */
int AnalyticResultsGateway::insertResults(const unsigned int iAnalyticIinstId,
		const std::string& sTimestamp, const std::string& sResult )
{
	int iResult = 0;
	sql::PreparedStatement* pStatementPtr = NULL;
	//Insert the analytic result
	try
	{
		pStatementPtr = (*_pDbConnPtr).prepareStatement(_INSERT_RESULT_SQL);
		(*pStatementPtr).setUInt(1, iAnalyticIinstId);
		(*pStatementPtr).setString(2, sTimestamp);
		(*pStatementPtr).setString(3, sResult);
		iResult = (*pStatementPtr).executeUpdate();
		(*pStatementPtr).close(); delete pStatementPtr; pStatementPtr = NULL;

	}catch(sql::SQLException &e)
	{
		iResult = -1;
		if(pStatementPtr){(*pStatementPtr).close(); delete pStatementPtr; pStatementPtr = NULL;}
		std::string sErrorMsg = "AnalyticResultsGateway::insertResults : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}

	//Retrieve the auto generated results id for this result
	/*
	 * NOTE : It is assumed that there is only one analytic instance with the same ID is executed
	 *        on an analytic server, thus analytic id is used to retrieve the results id
	 */
	if(iResult <= 0) //Return if executing _INSERT_RESULT_SQL above has not return a +ve value
	{
		return iResult;
	}

	iResult = 0;

	sql::ResultSet* pResultsPtr = NULL;
	try
	{
		pStatementPtr = (*_pDbConnPtr).prepareStatement(_SELECT_ANALYTIC_INST_MAX_RESULTS_ID);
		(*pStatementPtr).setUInt(1, iAnalyticIinstId);
		pResultsPtr = (*pStatementPtr).executeQuery();

		while((*pResultsPtr).next())
		{
			iResult = (*pResultsPtr).getInt("result_id");
		}

		if(pStatementPtr){ (*pStatementPtr).close(); delete pStatementPtr; pStatementPtr = NULL;}
		if(pResultsPtr){(*pResultsPtr).close(); delete pResultsPtr; pResultsPtr = NULL;}

	}catch(sql::SQLException &e)
	{
		if(pStatementPtr){ (*pStatementPtr).close(); delete pStatementPtr; pStatementPtr = NULL;}
		if(pResultsPtr){(*pResultsPtr).close(); delete pResultsPtr; pResultsPtr = NULL;}
		std::string sErrorMsg = "AnalyticResultsGateway::insertResults : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}

	return iResult;
}

//sql::Statement* pStatement = (*_pConnectionPtr).createStatement();
int AnalyticResultsGateway::insertFiles(const int iResultsId,
		const std::vector<std::string>* pVFiles, const std::string& sFileType)
{
	int iResult = 0;
	std::string sPath = "";

	if(!pVFiles || pVFiles->empty())
	{
		iResult =  0;
		return iResult;
	}

	if(sFileType.compare("results_files") == 0)
	{
		sPath = "file_path";
	}else if(sFileType.compare("images") == 0)
	{
		sPath = "image_file_path";
	}else if(sFileType.compare("videos") == 0)
	{
		sPath = "video_file_path";
	}else
	{
		iResult =  0;
		return iResult;
	}

	//Prepare the insert query
	//INSERT INTO results_files(analytic_result_id, file_path) VALUES (1, 'test1'),(1, 'test2');
	std::stringstream sQuery;
	sQuery << "INSERT INTO " << sFileType << "(analytic_result_id, " << sPath <<") VALUES ";
	std::string sSep = "";

	for (std::vector<std::string>::const_iterator it = pVFiles->begin() ; it != pVFiles->end(); ++it)
	{
	    //std::cout << ' ' << *it;
		sQuery << sSep <<"(" << iResultsId << "," << "'" << *it << "')";
		sSep = ",";
	}

	//std::cout << "sQuery : " << sQuery.str() << std::endl;
	sql::Statement* pStatement = NULL;

	try
	{
		pStatement = (*_pDbConnPtr).createStatement();
		iResult = (*pStatement).executeUpdate(sQuery.str());
		if(pStatement){ (*pStatement).close(); delete pStatement; pStatement = NULL;}

	}catch(sql::SQLException &e)
	{
		if(pStatement){ (*pStatement).close(); delete pStatement; pStatement = NULL;}
		iResult = -1;
		std::string sErrorMsg = "AnalyticResultsGateway::insertFiles : ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		// TODO: log
	}

	return iResult;
}


AnalyticResultsGateway::~AnalyticResultsGateway()
{
	if(_pDbConnectorPtr)
	{
		delete _pDbConnectorPtr; _pDbConnectorPtr = NULL;
	}

	if(_pDbConnPtr)
	{
		delete _pDbConnPtr; _pDbConnPtr = NULL;
	}
}

} /* namespace db */
} /* namespace analytic */
