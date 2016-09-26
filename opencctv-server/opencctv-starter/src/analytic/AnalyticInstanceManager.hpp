
#ifndef ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_
#define ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_

#include <map>

#include "../opencctv/mq/MqUtil.hpp"
#include "../opencctv/Exception.hpp"
#include "xml/AnalyticMessage.hpp"
#include "AnalyticData.hpp"

namespace analytic {

class AnalyticInstanceManager {
private:
	zmq::socket_t* _pSocket;
	std::map<unsigned int, AnalyticData*> _mAnalyticInstances; // Key is analytic instance id, Value is analytic data

public:
	AnalyticInstanceManager(const std::string& sAnalyticServerIp, const std::string& sAnalyticServerPort);
	bool startAnalyticInstance(unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename, std::string& sAnalyticQueueInAddress, std::string& sAnalyticQueueOutAddress);
	bool killAllAnalyticInstances();

	std::map<unsigned int, AnalyticData*>& getAnalyticInstances();
	void setAnalyticInstances(unsigned int analyticInstanceId, AnalyticData*);

	AnalyticData* getAanalyticData(const unsigned int& iAnalyticInstanceId);

	bool isMultipleStreamPerAnalytic(const unsigned int& iAnalyticInstanceId);

	virtual ~AnalyticInstanceManager();


};

} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_ */
