/*
 * CameraGateway.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: Anjana Tisera
 */

#include "CameraGateway.hpp"

namespace db
{
const string CameraGateway::_SELECT_CAMERA_INFO_SQL = "SELECT DISTINCT c.id, c.camera_id, c.vms_id, v.vms_type FROM cameras AS c, vmses AS v WHERE c.vms_id = v.id AND c.id = ?";

CameraGateway::CameraGateway()
{
	_pDbConn = DbConnector::getConnection();
}

CameraGateway::~CameraGateway()
{
	delete _pDbConn;
	_pDbConn = NULL;
}

Camera CameraGateway::find(int iCameraId)
{
	Camera camera;
	//int result = 1;
	try
	{
		PreparedStatement* statementPtr = (*_pDbConn).prepareStatement(_SELECT_CAMERA_INFO_SQL);
		(*statementPtr).setInt(1, iCameraId);
		ResultSet* resultsPtr = (*statementPtr).executeQuery();
		while((*resultsPtr).next())
		{
			camera.setId((*resultsPtr).getInt("id"));
			camera.setCameraId((*resultsPtr).getString("camera_id"));
			camera.setVmsId((*resultsPtr).getInt("vms_id"));
			camera.setVmsType((*resultsPtr).getString("vms_type"));
		}
		(*resultsPtr).close();
		(*statementPtr).close();
		delete resultsPtr;
		delete statementPtr;
	}
	catch(sql::SQLException &e)
	{
		//result = -1;
		std::cerr << "CameraGateway:find: Error while finding a camera when id is given from the database. " << e.what() << std::endl;
	}
	return camera;
}
} /* namespace db */
