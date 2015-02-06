/*
 * CameraAnalyticGateway.hpp
 *
 *  Created on: Oct 9, 2014
 *      Author: lakindu
 */

#ifndef CAMERAANALYTICGATEWAY_HPP_
#define CAMERAANALYTICGATEWAY_HPP_

#include "DbConnector.hpp"
#include "Analytic.hpp"
#include <vector>
#include <cppconn/prepared_statement.h>

using namespace std;

namespace db
{
class AnalyticGateway
{
private:
	Connection* dbConnPtr;
	static const string SELECT_ANALYTIC_INFO_SQL;
public:
	AnalyticGateway();
	~AnalyticGateway();
	/**
	 * Finds all analytic information from database for a particular camera.
	 */
	vector<Analytic*> findAnalytics(unsigned int cameraId);


};
} /* namespace db */

#endif /* CAMERAANALYTICGATEWAY_HPP_ */
