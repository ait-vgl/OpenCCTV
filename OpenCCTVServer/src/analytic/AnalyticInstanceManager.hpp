
#ifndef ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_
#define ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_

#include "../opencctv/mq/MqUtil.hpp"
#include "../opencctv/Exception.hpp"
#include "xml/AnalyticMessage.hpp"

namespace analytic {

class AnalyticInstanceManager {
private:
	zmq::socket_t* _pSocket;
public:
	AnalyticInstanceManager(const std::string& sAnalyticServerIp, const std::string& sAnalyticServerPort);
	bool startAnalyticInstance(unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename, std::string& sAnalyticQueueInAddress, std::string& sAnalyticQueueOutAddress);
	bool killAllAnalyticInstances();
	virtual ~AnalyticInstanceManager();
};

} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_ */
