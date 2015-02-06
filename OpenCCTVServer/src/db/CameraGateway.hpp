/*
 * CameraGateway.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: Anjana Tisera
 */

#ifndef CAMERAGATEWAY_HPP_
#define CAMERAGATEWAY_HPP_

#include "DbConnector.hpp"
#include "Camera.hpp"
#include <vector>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

namespace db
{
class CameraGateway
{
private:
	Connection* _pDbConn;
	static const string _SELECT_CAMERA_INFO_SQL;
public:
	CameraGateway();
	~CameraGateway();
	Camera find(int iCameraId);
	//int find(int iCameraId,Camera &camera);
};
} /* namespace db */

#endif /* CAMERAGATEWAY_HPP_ */
