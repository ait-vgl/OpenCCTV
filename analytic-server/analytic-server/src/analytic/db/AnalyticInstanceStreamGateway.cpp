/*
 * AnalyticInstanceStreamGateway.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "AnalyticInstanceStreamGateway.hpp"

namespace analytic {
namespace db {

//"SELECT ainsts.id, ainsts.analytic_instance_id, ainps.name, a.filename FROM analytic_instance_streams AS ainsts, analytic_input_streams AS ainps, analytics AS a WHERE (ainsts.stream_id = ?) AND (ainsts.analytic_input_stream_id = ainps.id ) AND (ainps.analytic_id = a.id)";

const std::string AnalyticInstanceStreamGateway::_SELECT_ANALYTIC_INSTANCE_STREAM_SQL = "select  str.id as stream_id, aInputStream.name,str.url,str.width, str.height,  cam.username, cam.password, aInstanceStream.config "
"FROM analytic_instances as aInstance, analytic_instance_streams as aInstanceStream, streams as str, cameras as cam, analytic_input_streams as aInputStream "
 "WHERE aInstance.id = ? "
 "and aInstanceStream.analytic_instance_id = aInstance.id "
 "and aInstanceStream.analytic_input_stream_id = aInputStream.id "
 "and aInstanceStream.stream_id = str.id "
 "and str.camera_id = cam.id;" ;

AnalyticInstanceStreamGateway::AnalyticInstanceStreamGateway()
{
	try
	{
		_pDbConnPtr = DbConnector::getConnection();
		_pStatementPtr = (*_pDbConnPtr).prepareStatement(_SELECT_ANALYTIC_INSTANCE_STREAM_SQL);

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticInstanceStreamGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}
}

AnalyticInstanceStreamGateway::~AnalyticInstanceStreamGateway() {
	(*_pStatementPtr).close();
	delete _pStatementPtr;  _pStatementPtr = NULL;
	delete _pDbConnPtr; _pDbConnPtr = NULL;
}

void AnalyticInstanceStreamGateway::getAnalyticInstanceStreams(unsigned int iAnalyticInstanceId, std::vector<analytic::dto::AnalyticInstanceStream>& vAnalyticInstanceStream)
{
	try
	{
		(*_pStatementPtr).setInt64(1, iAnalyticInstanceId);
		sql::ResultSet* pResultsPtr = (*_pStatementPtr).executeQuery();

		analytic::dto::AnalyticInstanceStream ais;
		while((*pResultsPtr).next())
		{
			ais.setStreamId((*pResultsPtr).getInt("stream_id"));
			ais.setInputName((*pResultsPtr).getString("name"));
			ais.setURL((*pResultsPtr).getString("url"));
			ais.setWidth((*pResultsPtr).getInt("width"));
			ais.setHeight((*pResultsPtr).getInt("height"));
			ais.setUsername((*pResultsPtr).getString("username"));
			ais.setPassword((*pResultsPtr).getString("password"));
			ais.setConfig((*pResultsPtr).getString("config"));

			vAnalyticInstanceStream.push_back(ais);
			
		}
		(*pResultsPtr).close();
		delete pResultsPtr;
	}
	catch(sql::SQLException &e)
	{
		//std::cerr << "AnalyticGateway:findAnalytics: Error while finding analytics from the database. " << e.what() << std::endl;
		std::string sErrorMsg = "AnalyticInstanceStreamGateway::findAllForStream: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
}

} /* namespace db */
} /* namespace analytic */


