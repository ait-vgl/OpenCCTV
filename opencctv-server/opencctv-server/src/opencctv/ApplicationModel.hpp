
#ifndef OPENCCTV_APPLICATIONMODEL_HPP_
#define OPENCCTV_APPLICATIONMODEL_HPP_

#include <map>
#include <string>
#include "mq/Mq.hpp"
#include "PluginLoader.hpp"
#include "api/VmsConnector.hpp"
#include "util/flow/FlowController.hpp"
#include "ConcurrentQueue.hpp"
#include "Image.hpp"
#include "../analytic/AnalyticInstanceManager.hpp"

namespace opencctv {

class ApplicationModel {
private:
	static ApplicationModel* _pModel;
	ApplicationModel();
	std::map<unsigned int, std::string> _mImageInputQueueAddresses; // Analytic Instance ID as key
	std::map<unsigned int, std::string> _mResultsOutputQueueAddresses; // Analytic Instance ID as key
	std::map<unsigned int, util::flow::FlowController*> _mFlowControllers; // Analytic Instance ID as key
	std::map<unsigned int, mq::Sender*> _mMulticastDestinations; // Analytic Instance Stream ID as key
	std::map<unsigned int, PluginLoader<api::VmsConnector>*> _mVmsPluginLoaders; //VMS Type ID as key
	std::map<unsigned int, ConcurrentQueue<Image>*> _mInternalQueues; // Stream ID as key
	std::map<unsigned int, analytic::AnalyticInstanceManager*> _mAnalyticInstanceManagers; // Analytic Server ID as key
public:
	static ApplicationModel* getInstance();
	bool containsImageInputQueueAddress(unsigned int iAnalyticInstanceId);
	bool containsResultsOutputQueueAddress(unsigned int iAnalyticInstanceId);
	bool containsFlowController(unsigned int iAnalyticInstanceId);
	bool containsMulticastDestination(unsigned int iId);
	bool containsVmsPluginLoader(unsigned int iVmsTypeId);
	bool containsInternalQueue(unsigned int iStreamId);
	bool containsAnalyticInstanceManager(unsigned int iAnalyticServerId);
	std::map<unsigned int, std::string>& getImageInputQueueAddresses();
	std::map<unsigned int, std::string>& getResultsOutputQueueAddresses();
	std::map<unsigned int, util::flow::FlowController*>& getFlowControllers();
	std::map<unsigned int, mq::Sender*>& getMulticastDestinations();
	std::map<unsigned int, PluginLoader<api::VmsConnector>*>& getVmsPluginLoaders();
	std::map<unsigned int, ConcurrentQueue<Image>*>& getInternalQueues();
	std::map<unsigned int, analytic::AnalyticInstanceManager*>& getAnalyticInstanceManagers();
	virtual ~ApplicationModel();
};

} /* namespace opencctv */

#endif /* OPENCCTV_APPLICATIONMODEL_HPP_ */
