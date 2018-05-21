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

const std::string AnalyticInstanceStreamGateway::_SELECT_ANALYTIC_INSTANCE_STREAM_SQL = "SELECT  str.id as stream_id, aInputStream.name,str.url,"
		"str.width, str.height, str.fps, cam.username, cam.password, aInstanceStream.config "
        "FROM analytic_instances as aInstance, analytic_instance_streams as aInstanceStream, streams as str, cameras as cam, "
        "analytic_input_streams as aInputStream "
        "WHERE aInstance.id = ? "
        "AND aInstanceStream.analytic_instance_id = aInstance.id "
        "AND aInstanceStream.analytic_input_stream_id = aInputStream.id "
        "AND aInstanceStream.stream_id = str.id "
        "AND str.camera_id = cam.id" ;

AnalyticInstanceStreamGateway::AnalyticInstanceStreamGateway()
{
	try
	{
		_pDbConnectorPtr = new DbConnector();
		_pDbConnPtr = (*_pDbConnectorPtr).getConnection_OpenCCTVServerDB();
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

void AnalyticInstanceStreamGateway::getAnalyticInstanceStreams(unsigned int iAnalyticInstanceId, std::vector<analytic::dto::AnalyticInstanceStream>& vAnalyticInstanceStream)
{
	sql::ResultSet* pResultsPtr = NULL;
	sql::PreparedStatement* pStatementPtr;
	try
	{
		pStatementPtr = (*_pDbConnPtr).prepareStatement(_SELECT_ANALYTIC_INSTANCE_STREAM_SQL);
		(*pStatementPtr).setInt64(1, iAnalyticInstanceId);
		pResultsPtr = (*pStatementPtr).executeQuery();

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
			ais.setFps((*pResultsPtr).getInt("fps"));
			vAnalyticInstanceStream.push_back(ais);
		}

		if(pStatementPtr){ (*pStatementPtr).close(); delete pStatementPtr; pStatementPtr = NULL;}
		if(pResultsPtr){(*pResultsPtr).close(); delete pResultsPtr; pResultsPtr = NULL;}
	}
	catch(sql::SQLException &e)
	{
		if(pStatementPtr){ (*pStatementPtr).close(); delete pStatementPtr; pStatementPtr = NULL;}
		if(pResultsPtr){(*pResultsPtr).close(); delete pResultsPtr; pResultsPtr = NULL;}
		std::string sErrorMsg = "AnalyticInstanceStreamGateway::findAllForStream: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
}

AnalyticInstanceStreamGateway::~AnalyticInstanceStreamGateway()
{
	if(_pDbConnPtr)
	{
		delete _pDbConnPtr; _pDbConnPtr = NULL;
	}

	if(_pDbConnectorPtr)
	{
		delete _pDbConnectorPtr; _pDbConnectorPtr = NULL;
	}
}

} /* namespace db */
} /* namespace analytic */


