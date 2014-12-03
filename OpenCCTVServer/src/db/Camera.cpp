/*
 * Camera.cpp
 *
 * This is the implementation of the Camera data transfer object
 *
 *  Created on: Sep 17, 2014
 *      Author: Anjana Tisera
 */

#include "Camera.hpp"

namespace db
{
Camera::Camera()
{
	id = 0;
	cameraId = "";
    vmsId = 0;
	vmsType = "";
}

Camera::Camera(unsigned int id, const string& cameraId, unsigned int vmsId, const string& vmsType)
{
	this->id = id;
	this->cameraId = cameraId;
	this->vmsId = vmsId;
	this->vmsType = vmsType;
}

Camera::~Camera()
{
	/*delete &id;
	delete &cameraId;
	delete &vmsId;
	delete &vmsType;*/
}

const string& Camera::getCameraId() const
{
	return cameraId;
}

void Camera::setCameraId(const string& cameraId)
{
	this->cameraId = cameraId;
}

unsigned int Camera::getId() const
{
	return id;
}

void Camera::setId(unsigned int id)
{
	this->id = id;
}

unsigned int Camera::getVmsId() const
{
	return vmsId;
}

void Camera::setVmsId(unsigned int vmsId)
{
	this->vmsId = vmsId;
}

const string& Camera::getVmsType() const
{
	return vmsType;
}

void Camera::setVmsType(const string& vmsType)
{
	this->vmsType = vmsType;
}
} /* namespace db */
