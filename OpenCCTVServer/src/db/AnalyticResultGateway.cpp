/*
 * AnalyticResultGateway.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: anjana
 */

#include "AnalyticResultGateway.hpp"

namespace db {

const string AnalyticResultGateway::_INSERT_RESULT_SQL = "INSERT INTO analytic_results (analytic_instance_id, image_timestamp, result, image_file_path) VALUES (?,?,?,?)";

AnalyticResultGateway::AnalyticResultGateway() {
	_pDbConn = DbConnector::getConnection_ResultsDB();

}

int AnalyticResultGateway::insertResults(int iAnalyticInstId, string sImgTimestamp, string sResult, string sImgFilePath)
{
	int result = 0;

	try{
		PreparedStatement* statementPtr = (*_pDbConn).prepareStatement(_INSERT_RESULT_SQL);
		(*statementPtr).setInt(1, iAnalyticInstId);
		(*statementPtr).setString(2, sImgTimestamp);
		(*statementPtr).setString(3, sResult);
		(*statementPtr).setString(4, sImgFilePath);
		result = (*statementPtr).executeUpdate();

		(*statementPtr).close();
		delete statementPtr;

	}catch(sql::SQLException &e)
	{
		result = -1;
		std::cerr << "AnalyticResultGateway:insertResults: Error while Inserting analytics results to the database. " << e.what() << std::endl;
	}

	return result;

}

AnalyticResultGateway::~AnalyticResultGateway() {
	delete _pDbConn;
	_pDbConn = NULL;
}

} /* namespace db */
