/*
 * AnalyticServerDTO.hpp
 *
 *  Created on: Jun 4, 2015
 *      Author: anjana
 */

#ifndef ANALYTICSERVERDTO_HPP_
#define ANALYTICSERVERDTO_HPP_

#include <string> // std::string

namespace opencctv {
namespace dto {

class AnalyticServerDTO {
private:
	unsigned int _iAnalyticServerId;
	std::string _sIp;
	int _iPort;
	std::string _sName;
	std::string _sStatus;
	int _iPid;
	std::string _sTag;
public:
	AnalyticServerDTO();
	virtual ~AnalyticServerDTO();

	unsigned int getAnalyticServerId() {
		return _iAnalyticServerId;
	}
	void setAnalyticServerId(unsigned int analyticServerId) {
		_iAnalyticServerId = analyticServerId;
	}

	std::string getIp() {
		return _sIp;
	}
	void setIp(std::string ip) {
		_sIp = ip;
	}

	int getPort() {
		return _iPort;
	}
	void setPort(int port) {
		_iPort = port;
	}
	
	std::string getName() {
		return _sName;
	}
	void setName(std::string name) {
		_sName = name;
	}
	
	std::string getStatus() {
		return _sStatus;
	}
	void setStatus(std::string status) {
		_sStatus = status;
	}
	
	int getPid() {
		return _iPid;
	}
	void setPid(int pId) {
		_iPid = pId;
	}
	
	std::string getTag() {
		return _sTag;
	}
	void setTag(std::string tag) {
		_sTag = tag;
	}
};

} /* namespace dto */
} /* namespace opencctv */

#endif /* ANALYTICSERVERDTO_HPP_ */
