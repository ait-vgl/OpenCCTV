/*
 * AnalyticResultGateway.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: anjana
 */

#include "AnalyticResultGateway.hpp"

namespace db {

const string AnalyticResultGateway::_INSERT_RESULT_SQL = "INSERT INTO AnalyticResult (StreamId, AnalyticInstanceId, AnalyticId, ImgTimestamp, Result) VALUES(?,?,?,?,?)";

AnalyticResultGateway::AnalyticResultGateway() {
	_pDbConn = DbConnector::getConnection();

}

int AnalyticResultGateway::insertResults(int iStreamId, int iAnalyticInstId, int iAnalyticId, string sImgTimestamp, string sResult)
{
	int result = 0;

	try{
		PreparedStatement* statementPtr = (*_pDbConn).prepareStatement(_INSERT_RESULT_SQL);
		(*statementPtr).setInt(1, iStreamId);
		(*statementPtr).setInt(2, iAnalyticInstId);
		(*statementPtr).setInt(3, iAnalyticId);
		(*statementPtr).setString(4, sImgTimestamp);
		(*statementPtr).setString(5, sResult);
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
