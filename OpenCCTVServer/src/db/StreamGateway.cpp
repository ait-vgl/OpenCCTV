/*
 * StreamGateway.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: lakindu
 */

#include "StreamGateway.hpp"

namespace db
{

const string StreamGateway::_SELECT_STREAM_INFO_SQL = "SELECT * FROM streams AS s WHERE s.id IN (SELECT DISTINCT ais.stream_id FROM analytic_instance_streams as ais)";

StreamGateway::StreamGateway()
{
	_pDbConn = DbConnector::getConnection();
}

StreamGateway::~StreamGateway()
{
	delete _pDbConn;
	_pDbConn = NULL;
}

vector<Stream> StreamGateway::findAll()
{
	vector<Stream> vStreams;
	Stream stream;
	try
	{
		Statement* pStatement = (*_pDbConn).createStatement();
		ResultSet* pResults = (*pStatement).executeQuery(_SELECT_STREAM_INFO_SQL);
		while((*pResults).next())
		{
			stream.setId((*pResults).getInt("id"));
			stream.setWidth((*pResults).getInt("width"));
			stream.setHeight((*pResults).getInt("height"));
			stream.setKeepAspectRatio((*pResults).getBoolean("keep_aspect_ratio"));
			stream.setAllowupSizing((*pResults).getBoolean("allow_upsizing"));
			stream.setCompressionRate((*pResults).getInt("compression_rate"));
			stream.setCamerId((*pResults).getInt("camera_id"));
			vStreams.push_back(stream);
		}
		(*pResults).close();
		(*pStatement).close();
		delete pResults;
		delete pStatement;
	}
	catch(sql::SQLException &e)
	{
		std::cerr << "StreamGateway:findAll: Error while finding all streams from the database. " << e.what() << std::endl;
		// TODO: log
		// TODO: throw application exception
	}
	return vStreams;
}
} /* namespace db */
