
#ifndef OPENCCTV_RESULTROUTERTHREAD_HPP_
#define OPENCCTV_RESULTROUTERTHREAD_HPP_

#include <sstream>

#include "db/AnalyticResultGateway.hpp"
#include "mq/TcpMqReceiver.hpp"
#include "Exception.hpp"

#include "util/Config.hpp"
#include "util/serialization/Serializers.hpp"
#include "util/log/Loggers.hpp"
#include "util/flow/FlowController.hpp"

//#include "../analytic/AnalyticServer.hpp"
#include "../analytic/AnalyticData.hpp"
//#include "../analytic/AnalyticServerManager.hpp"

namespace opencctv {

class ResultRouterThread {
private:
	unsigned int _iAnalyticInstanceId;
   // analytic::AnalyticData* _pAD ;
	//util::flow::FlowController* _pFlowController;
    std::string _sAnalyticServerIp;
    std::string _sAnalyticQueueOutPort;
	util::serialization::Serializable* _pSerializer;

public:
	ResultRouterThread(unsigned int iAnalyticInstanceId, std::string sAnalyticServerIp, std::string sAnalyticQueueOutPort);
	void operator()();
	~ResultRouterThread();
};

} /* namespace opencctv */

#endif /* OPENCCTV_RESULTROUTERTHREAD_HPP_ */
