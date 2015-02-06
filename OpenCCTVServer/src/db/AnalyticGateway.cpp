/*
 * CameraAnalyticGateway.cpp
 *
 *  Created on: Oct 9, 2014
 *      Author: lakindu
 */

#include "AnalyticGateway.hpp"

namespace db
{
const string AnalyticGateway::SELECT_ANALYTIC_INFO_SQL = "SELECT DISTINCT ca.analyticId, a.location FROM CameraAnalytic as ca, Analytic as a WHERE ca.analyticId = a.id AND ca.cameraId = ?";

AnalyticGateway::AnalyticGateway()
{
	dbConnPtr = DbConnector::getConnection();
}

AnalyticGateway::~AnalyticGateway()
{
	(*dbConnPtr).close();
	delete dbConnPtr;
}

vector<Analytic*> AnalyticGateway::findAnalytics(unsigned int cameraId)
{
	vector<Analytic*> analytics;
	try
	{
		PreparedStatement* statementPtr = (*dbConnPtr).prepareStatement(SELECT_ANALYTIC_INFO_SQL);
		(*statementPtr).setInt(1, cameraId);
		ResultSet* resultsPtr = (*statementPtr).executeQuery();
		while((*resultsPtr).next())
		{
			Analytic* analyticPtr = new Analytic();
			(*analyticPtr).setAnalyticId((*resultsPtr).getInt("analyticId"));
			(*analyticPtr).setLocation((*resultsPtr).getString("location"));
			analytics.push_back(analyticPtr);
		}
		(*resultsPtr).close();
		(*statementPtr).close();
		delete resultsPtr;
		delete statementPtr;
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "AnalyticGateway:findAnalytics: Error while finding analytics from the database. " << e.what() << std::endl;
	}
	return analytics;
}


} /* namespace db */
