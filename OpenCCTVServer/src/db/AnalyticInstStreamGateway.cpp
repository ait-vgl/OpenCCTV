/*
 * AnalyticInstStreamGateway.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: anjana
 */

#include "AnalyticInstStreamGateway.hpp"

namespace db {

const string AnalyticInstStreamGateway::_SELECT_ANALYTICINSTSTREAM_INFO_SQL = "SELECT ais.id, ais.analytic_instance_id, ais.stream_id, aip.name, a.id as analytic_id, a.location FROM analytics as a, analytic_instances as ai, analytic_instance_streams as ais, analytic_input_streams as aip WHERE ai.analytic_id = a.id AND ais.analytic_instance_id = ai.id AND ais.analytic_input_stream_id = aip.id AND ais.stream_id = ?";

AnalyticInstStreamGateway::AnalyticInstStreamGateway() {
	_pDbConn = DbConnector::getConnection();
}

AnalyticInstStreamGateway::~AnalyticInstStreamGateway() {
	(*_pDbConn).close();
	delete _pDbConn;
	_pDbConn = NULL;
}

vector<AnalyticInstStream> AnalyticInstStreamGateway::findAnalyticInstances(unsigned int iStreamId){

	vector<AnalyticInstStream> vAnalyticInstStream;

	try{

		PreparedStatement* pStatement = (*_pDbConn).prepareStatement(_SELECT_ANALYTICINSTSTREAM_INFO_SQL);
			(*pStatement).setInt(1, iStreamId);
			ResultSet* pResults = (*pStatement).executeQuery();

			AnalyticInstStream _analyticInstStream;

			while ((*pResults).next()) {

				_analyticInstStream.setId((*pResults).getInt("id"));
				_analyticInstStream.setAnalyticInstanceId((*pResults).getInt("analytic_instance_id"));
				_analyticInstStream.setStreamId(iStreamId);
				_analyticInstStream.setInputName((*pResults).getString("name"));
				_analyticInstStream.setAnalyticId((*pResults).getInt("analytic_id"));
				_analyticInstStream.setLocation((*pResults).getString("location"));

				vAnalyticInstStream.push_back(_analyticInstStream);
			}
			(*pResults).close();
			(*pStatement).close();
			delete pResults;
			delete pStatement;

	}catch(sql::SQLException &e)
	{
		std::cerr << "AnalyticInstStreamGateway:findAnalyticInstances: Error while finding all streams from the database. " << e.what() << std::endl;
		// TODO: log
		// TODO: throw application exception
	}

	return vAnalyticInstStream;
}

} /* namespace db */
