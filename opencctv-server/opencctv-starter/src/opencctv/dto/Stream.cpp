
#include "Stream.hpp"

namespace opencctv {
namespace dto {

Stream::Stream() {
	_iId = 0;
	_iWidth = 0;
	_iHeight = 0;
	_iCompressionRate = 0;
	_iVmsTypeId = 0;
	_iServerPort = 0;
	_bKeepAspectRatio = true;
	_bAllowUpSizing = false;
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

void Stream::setAllowUpSizing(bool bAllowupSizing)
{
	_bAllowUpSizing = bAllowupSizing;
}

bool Stream::getAllowUpSizing() const
{
	return _bAllowUpSizing;
}

void Stream::setCompressionRate(unsigned short int iCompressionRate)
{
	_iCompressionRate = iCompressionRate;
}

unsigned short int Stream::getCompressionRate() const
{
	return _iCompressionRate;
}

const std::string& Stream::getCameraId() const
{
	return _sCameraId;
}
void Stream::setCameraId(const std::string& sCameraId)
{
	_sCameraId = sCameraId;
}

void Stream::setVmsServerIp(const std::string& sServerIp)
{
	_sServerIp = sServerIp;
}
const std::string& Stream::getVmsServerIp() const
{
	return _sServerIp;
}

void Stream::setVmsServerPort(unsigned short int iServerPort)
{
	_iServerPort = iServerPort;
}
unsigned short int Stream::getVmsServerPort() const
{
	return _iServerPort;
}

void Stream::setVmsUsername(const std::string& sUsername)
{
	_sUsername = sUsername;
}
const std::string& Stream::getVmsUsername() const
{
	return _sUsername;
}

void Stream::setVmsPassword(const std::string& sPassword)
{
	_sPassword = sPassword;
}
const std::string& Stream::getVmsPassword() const
{
	return _sPassword;
}

void Stream::setVmsTypeId(unsigned int iVmsTypeId)
{
	_iVmsTypeId = iVmsTypeId;
}
unsigned int Stream::getVmsTypeId() const
{
	return _iVmsTypeId;
}

void Stream::setVmsConnectorDirLocation(const std::string& sLocation)
{
	_sVmsConnectorDirLocation = sLocation;
}

const std::string& Stream::getVmsConnectorDirLocation() const
{
	return _sVmsConnectorDirLocation;
}

void Stream::setVmsConnectorFilename(const std::string& sFilename)
{
	_sVmsConnectorFilename = sFilename;
}

const std::string& Stream::getVmsConnectorFilename() const
{
	return _sVmsConnectorFilename;
}

Stream::~Stream() {
}

} /* namespace dto */
} /* namespace opencctv */
