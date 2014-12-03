/*
 * Camera.hpp
 *
 * This is the definition of the Camera data transfer object
 *
 *  Created on: Sep 17, 2014
 *      Author: Anjana Tisera
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <string>

using namespace std;

namespace db
{
/**
 * Class to help camera information from database.
 */
class Camera
{
private:
	unsigned int id;
	string cameraId;
	unsigned int vmsId;
	string vmsType;

public:
	Camera();
	Camera(unsigned int id, const string& cameraId, unsigned int vmsID, const string& vmsType);
	~Camera();
	const string& getCameraId() const;
	void setCameraId(const string& camId);
	unsigned int getId() const;
	void setId(unsigned int id);
	unsigned int getVmsId() const;
	void setVmsId(unsigned int vmsId);
	const string& getVmsType() const;
	void setVmsType(const string& vmsType);
};
} /* namespace db */

#endif /* CAMERA_HPP_ */
