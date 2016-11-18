/*
 * AnalyticInstanceConfigGateway.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "AnalyticInstanceConfigGateway.hpp"

namespace analytic {
namespace db {

//"SELECT DISTINCT s.id, s.width, s.height, s.keep_aspect_ratio, s.allow_upsizing, s.compression_rate, ca.camera_id, vms.vms_connector_id, vms.server_ip, vms.server_port, vms.username, vms.password, vmsc.filename FROM streams AS s, cameras AS ca, vmses as vms, vms_connectors as vmsc WHERE (s.verified = TRUE) AND (s.camera_id = ca.id) AND (ca.vms_id = vms.id) AND (vms.vms_connector_id = vmsc.id) AND (s.id IN (SELECT DISTINCT ais.stream_id FROM analytic_instance_streams as ais))";

const std::string AnalyticInstanceConfigGateway::_SELECT_STREAM_SQL = "SELECT  id,fileName, data FROM analytic_instance_configs WHERE analytic_instance_id = ?;";

AnalyticInstanceConfigGateway::AnalyticInstanceConfigGateway()
{
	try
	{
		_pDbConnPtr = DbConnector::getConnection();
		//_pStatement = (*_pDbConnPtr).createStatement();
		_pStatementPtr = (*_pDbConnPtr).prepareStatement(_SELECT_STREAM_SQL);
	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "Error while initializing the AnalyticInstanceConfigGateway - .";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
	}
	catch(opencctv::Exception& e)
	{
		throw opencctv::Exception(e);
	}

}

AnalyticInstanceConfigGateway::~AnalyticInstanceConfigGateway() {
	(*_pStatementPtr).close();
	delete _pStatementPtr; _pStatementPtr = NULL;
	delete _pDbConnPtr; _pDbConnPtr = NULL;
}

void AnalyticInstanceConfigGateway::getAnalyticInstanceConfig(unsigned int iAnalyticInstanceId, std::vector<analytic::dto::AnalyticInstanceConfig>& vAnalyticInstanceConfigs)
{
	try
	{ 
		(*_pStatementPtr).setInt64(1, iAnalyticInstanceId);
		sql::ResultSet* pResultsPtr = (*_pStatementPtr).executeQuery();
		
		analytic::dto::AnalyticInstanceConfig aic;
		while((*pResultsPtr).next())
		{
			aic.setId((*pResultsPtr).getInt("id"));
			aic.setFileName((*pResultsPtr).getString("fileName"));
			aic.setData((*pResultsPtr).getString("data"));
			
			vAnalyticInstanceConfigs.push_back(aic);
		}
		(*pResultsPtr).close();
		delete pResultsPtr;

	}catch(sql::SQLException &e)
	{
		std::string sErrorMsg = "AnalyticInstanceConfigGateway:findAll: ";
		throw opencctv::Exception(sErrorMsg.append(e.what()));
		//std::cerr << "AnalyticInstanceConfigGateway:findAll: Error while finding all streams from the database. " << e.what() << std::endl;
	}
}

} /* namespace db */
} /* namespace analytic */
