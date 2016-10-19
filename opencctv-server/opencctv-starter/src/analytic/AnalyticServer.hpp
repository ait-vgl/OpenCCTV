
#ifndef ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_
#define ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_

#include <map>

#include "../opencctv/mq/MqUtil.hpp"
#include "../opencctv/Exception.hpp"
#include "xml/AnalyticMessage.hpp"
#include "../opencctv/util/log/Loggers.hpp"

#include "AnalyticData.hpp"

namespace analytic {

class AnalyticServer {
private:
	int _iServerId;
	zmq::socket_t* _pSocket;
	std::map<unsigned int, AnalyticData*> _mAnalyticDatas; // Key is analytic instance id, Value is analytic data

public:
	AnalyticServer(const int& analyticServerId, const std::string& sAnalyticServerIp, const std::string& sAnalyticServerPort);
	bool startAnalyticInstance(unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename, std::string& sAnalyticQueueInAddress, std::string& sAnalyticQueueOutAddress);
	bool killAllAnalyticInstances();

	const std::map<unsigned int, AnalyticData*>& getAllAnalyticData();

	void setAnalyticData(unsigned int iAnalyticInstanceId, AnalyticData*& pAnalyticData);
	AnalyticData* getAnalyticData(unsigned int iAnalyticInstanceId);

	bool isAnalyticInstance(int iAnalyticInstanceId);

	//bool isMultipleStreamPerAnalytic(const unsigned int& iAnalyticInstanceId);

	//bool isImageInputQueueAddress(const int& );

	virtual ~AnalyticServer();

	int getServerId(){
		return _iServerId;
	}
};

} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_ */
