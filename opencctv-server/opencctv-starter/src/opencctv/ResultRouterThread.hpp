
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

#include "../analytic/AnalyticServer.hpp"
#include "../analytic/AnalyticServerManager.hpp"



namespace opencctv {

class ResultRouterThread {
private:
	analytic::AnalyticServer* _pAS;
	analytic::AnalyticData* _pAD;

	unsigned int _iAnalyticInstanceId;
	util::flow::FlowController* _pFlowController;
	util::serialization::Serializable* _pSerializer;

public:
	ResultRouterThread(unsigned int iAnalyticInstanceId);
	void operator()();
	~ResultRouterThread();
};

} /* namespace opencctv */

#endif /* OPENCCTV_RESULTROUTERTHREAD_HPP_ */
