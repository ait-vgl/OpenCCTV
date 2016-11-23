/*
 * StreamGateway.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "StreamGateway.hpp"

namespace opencctv {
namespace db {

//"SELECT DISTINCT s.id, s.width, s.height, s.keep_aspect_ratio, s.allow_upsizing, s.compression_rate, ca.camera_id, vms.vms_connector_id, vms.server_ip, vms.server_port, vms.username, vms.password, vmsc.filename FROM streams AS s, cameras AS ca, vmses as vms, vms_connectors as vmsc WHERE (s.verified = TRUE) AND (s.camera_id = ca.id) AND (ca.vms_id = vms.id) AND (vms.vms_connector_id = vmsc.id) AND (s.id IN (SELECT DISTINCT ais.stream_id FROM analytic_instance_streams as ais))";

    // for start all ai.status = 2
const std::string StreamGateway::_SELECT_STREAM_SQL = "SELECT DISTINCT s.id, s.width, s.height, s.keep_aspect_ratio, s.allow_upsizing, "
 "s.compression_rate, "
 "ca.camera_id, "
 "vms.vms_connector_id, vms.server_ip, vms.server_port, vms.username, vms.password, "
 "vmsc.filename "
"FROM streams AS s, cameras AS ca, vmses as vms, vms_connectors as vmsc,analytic_instance_streams as ais, analytic_instances as ai "
"WHERE (s.verified = TRUE) AND (ai.status = 2) "
"AND (s.camera_id = ca.id) AND (ca.vms_id = vms.id) "
"AND (vms.vms_connector_id = vmsc.id) "
"AND (ais.stream_id = s.id) "
"AND (ai.id = ais.analytic_instance_id);" ;

// for start individual, no ai.status, because we have its id (analytic instance id)
const std::string StreamGateway::_SELECT_STREAM_BY_ID_SQL = "SELECT DISTINCT s.id, s.width, s.height, s.keep_aspect_ratio, s.allow_upsizing, "
 "s.compression_rate, "
 "ca.camera_id, "
 "vms.vms_connector_id, vms.server_ip, vms.server_port, vms.username, vms.password, "
 "vmsc.filename "
"FROM streams AS s, cameras AS ca, vmses as vms, vms_connectors as vmsc,analytic_instance_streams as ais, analytic_instances as ai "
"WHERE (ais.analytic_instance_id = ?) AND (s.verified = TRUE) "
"AND (s.camera_id = ca.id) AND (ca.vms_id = vms.id) "
"AND (vms.vms_connector_id = vmsc.id) "
"AND (ais.stream_id = s.id) "
"AND (ai.id = ais.analytic_instance_id);" ;

StreamGateway::StreamGateway()
{
	try
	{
		_pDbConnPtr = DbConnector::getConnection();
		_pStatement = (*_pDbConnPtr).createStatement();
       
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the StreamGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}

}

StreamGateway::~StreamGateway() {
	(*_pStatement).close();
	delete _pStatement; _pStatement = NULL;
    
    (*_pStatementPtr).close();
	delete _pStatementPtr;  _pStatementPtr = NULL;
	delete _pDbConnPtr; _pDbConnPtr = NULL;
    
}

void StreamGateway::findAll(std::vector<opencctv::dto::Stream>& vToStoreStreams)
{
	try
	{
		sql::ResultSet* pResultsPtr = (*_pStatement).executeQuery(_SELECT_STREAM_SQL);
		opencctv::dto::Stream stream;
		while((*pResultsPtr).next())
		{
			stream.setId((*pResultsPtr).getInt("id"));
			stream.setWidth((*pResultsPtr).getInt("width"));
			stream.setHeight((*pResultsPtr).getInt("height"));
			stream.setKeepAspectRatio((*pResultsPtr).getBoolean("keep_aspect_ratio"));
			stream.setAllowUpSizing((*pResultsPtr).getBoolean("allow_upsizing"));
			stream.setCompressionRate((*pResultsPtr).getInt("compression_rate"));
			stream.setCameraId((*pResultsPtr).getString("camera_id"));
			stream.setVmsTypeId((*pResultsPtr).getInt("vms_connector_id"));
			stream.setVmsServerIp((*pResultsPtr).getString("server_ip"));
			stream.setVmsServerPort((*pResultsPtr).getInt("server_port"));
			stream.setVmsUsername((*pResultsPtr).getString("username"));
			stream.setVmsPassword((*pResultsPtr).getString("password"));
			stream.setVmsConnectorFilename((*pResultsPtr).getString("filename"));
			stream.setVmsConnectorDirLocation((*pResultsPtr).getString("filename"));

			vToStoreStreams.push_back(stream);
		}
		(*pResultsPtr).close();
		delete pResultsPtr;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "StreamGateway:findAll: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		//std::cerr << "StreamGateway:findAll: Error while finding all streams from the database. " << e.what() << std::endl;
		// TODO: log
	}
}

void StreamGateway::findAllByAnalyticInstanceId(std::vector<opencctv::dto::Stream>& vToStoreStreams, unsigned int analyticInstanceId)
{
	try
	{
         _pStatementPtr = (*_pDbConnPtr).prepareStatement(_SELECT_STREAM_BY_ID_SQL);
         
         std::cout << _SELECT_STREAM_BY_ID_SQL << std::endl;
         std::cout << "  Analytic instance id  " << std::endl;
         std::cout << analyticInstanceId << std::endl;
		
        (*_pStatementPtr).setInt(1, analyticInstanceId); // set the analytic instance id

		sql::ResultSet* pResultsPtr = (*_pStatementPtr).executeQuery();
        
        std::cout << "  Row count  " << std::endl;
        std::cout << (*pResultsPtr).rowsCount() << std::endl;
        
		opencctv::dto::Stream stream;
		while((*pResultsPtr).next())
		{
			stream.setId((*pResultsPtr).getInt("id"));
			stream.setWidth((*pResultsPtr).getInt("width"));
			stream.setHeight((*pResultsPtr).getInt("height"));
			stream.setKeepAspectRatio((*pResultsPtr).getBoolean("keep_aspect_ratio"));
			stream.setAllowUpSizing((*pResultsPtr).getBoolean("allow_upsizing"));
			stream.setCompressionRate((*pResultsPtr).getInt("compression_rate"));
			stream.setCameraId((*pResultsPtr).getString("camera_id"));
			stream.setVmsTypeId((*pResultsPtr).getInt64("vms_connector_id"));
			stream.setVmsServerIp((*pResultsPtr).getString("server_ip"));
			stream.setVmsServerPort((*pResultsPtr).getInt("server_port"));
			stream.setVmsUsername((*pResultsPtr).getString("username"));
			stream.setVmsPassword((*pResultsPtr).getString("password"));
			stream.setVmsConnectorFilename((*pResultsPtr).getString("filename"));
			stream.setVmsConnectorDirLocation((*pResultsPtr).getString("filename"));

			vToStoreStreams.push_back(stream);
		}
		(*pResultsPtr).close();
		delete pResultsPtr;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "StreamGateway:findAll: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		//std::cerr << "StreamGateway:findAll: Error while finding all streams from the database. " << e.what() << std::endl;
		// TODO: log
	}
}

} /* namespace db */
} /* namespace opencctv */
