/*
 * AnalyticInstStreamGateway.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: anjana
 */

#include "AnalyticInstStreamGateway.hpp"

namespace db {

const string AnalyticInstStreamGateway::_SELECT_ANALYTICINSTSTREAM_INFO_SQL = "SELECT ais.id, ais.analyticInstanceId, ais.streamId, aip.inputName, a.id as analyticId, a.location FROM Analytic as a, AnalyticInstance as ai, AnalyticInstanceStream as ais, AnalyticInput as aip WHERE ai.analyticId = a.id AND ais.analyticInstanceId = ai.id AND ais.analyticInputId = aip.id AND ais.streamId = ?";

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
				_analyticInstStream.setAnalyticInstanceId((*pResults).getInt("analyticInstanceId"));
				_analyticInstStream.setStreamId(iStreamId);
				_analyticInstStream.setInputName((*pResults).getString("inputName"));
				_analyticInstStream.setAnalyticId((*pResults).getInt("analyticId"));
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
