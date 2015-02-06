/*
 * Stream.cpp
 *
 *  Created on: Oct 27, 2014
 *      Author: lakindu
 */

#include "Stream.hpp"

namespace db
{
Stream::Stream()
{
	_iId = 0;
	_iWidth = 0;
	_iHeight = 0;
	_bKeepAspectRatio = true;
	_bAllowupSizing = false;
	_iCompressionRate = 0;
	_iCameraId = 0;
}

void Stream::setId(unsigned int iId)
{
	_iId = iId;
}

unsigned int Stream::getId() const
{
	return _iId;
}

void Stream::setWidth(unsigned int iWidth)
{
	_iWidth = iWidth;
}

unsigned int Stream::getWidth() const
{
	return _iWidth;
}

void Stream::setHeight(unsigned int iHeight)
{
	_iHeight = iHeight;
}

unsigned int Stream::getHeight() const
{
	return _iHeight;
}

void Stream::setKeepAspectRatio(bool bKeepAspectRatio)
{
	_bKeepAspectRatio = bKeepAspectRatio;
}

bool Stream::getKeepAspectRatio() const
{
	return _bKeepAspectRatio;
}

void Stream::setAllowupSizing(bool bAllowupSizing)
{
	_bAllowupSizing = bAllowupSizing;
}

bool Stream::getAllowupSizing() const
{
	return _bAllowupSizing;
}

void Stream::setCompressionRate(int iCompressionRate)
{
	_iCompressionRate = iCompressionRate;
}

int Stream::getCompressionRate() const
{
	return _iCompressionRate;
}

void Stream::setCamerId(unsigned int iCameraId)
{
	_iCameraId = iCameraId;
}

unsigned int Stream::getCameraId() const
{
	return _iCameraId;
}

Stream::~Stream()
{}
} /* namespace db */
