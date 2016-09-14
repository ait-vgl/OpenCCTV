#ifndef OPENCCTV_STREAM_HPP_
#define OPENCCTV_STREAM_HPP_

#include <string> // std::string

namespace opencctv {
namespace dto {

class Stream {
private:
	/* Stream fields */
	unsigned int _iId;
	unsigned int _iWidth;
	unsigned int _iHeight;
	bool _bKeepAspectRatio;
	bool _bAllowUpSizing;
	unsigned short int _iCompressionRate;
	/* Camera fields */
	std::string _sCameraId; // not PK
	/* VMS fields */
	std::string _sServerIp; // or server name
	unsigned short int _iServerPort;
	std::string _sUsername;
	std::string _sPassword;
	unsigned int _iVmsTypeId; // to store vms plugin loaders in application model by this id
	std::string _sVmsConnectorDirLocation;
	std::string _sVmsConnectorFilename;
public:
	Stream();
	virtual ~Stream();
	/* getters and setter */
	void setId(unsigned int iId);
	unsigned int getId() const;
	void setWidth(unsigned int iWidth);
	unsigned int getWidth() const;
	void setHeight(unsigned int iHeight);
	unsigned int getHeight() const;
	void setKeepAspectRatio(bool bKeepAspectRatio);
	bool getKeepAspectRatio() const;
	void setAllowUpSizing(bool bAllowupSizing);
	bool getAllowUpSizing() const;
	void setCompressionRate(unsigned short int iCompressionRate);
	unsigned short int getCompressionRate() const;
	const std::string& getCameraId() const;
	void setCameraId(const std::string& sCameraId);
	void setVmsServerIp(const std::string& sServerIp);
	const std::string& getVmsServerIp() const;
	void setVmsServerPort(unsigned short int iServerPort);
	unsigned short int getVmsServerPort() const;
	void setVmsUsername(const std::string& sUsername);
	const std::string& getVmsUsername() const;
	void setVmsPassword(const std::string& sPassword);
	const std::string& getVmsPassword() const;
	void setVmsTypeId(unsigned int iVmsTypeId);
	unsigned int getVmsTypeId() const;
	void setVmsConnectorDirLocation(const std::string& sLocation);
	const std::string& getVmsConnectorDirLocation() const;
	void setVmsConnectorFilename(const std::string& sFilename);
	const std::string& getVmsConnectorFilename() const;
};

} /* namespace dto */
} /* namespace opencctv */

#endif /* OPENCCTV_STREAM_HPP_ */
