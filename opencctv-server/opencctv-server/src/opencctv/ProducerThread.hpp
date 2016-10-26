
#ifndef OPENCCTV_PRODUCERTHREAD_HPP_
#define OPENCCTV_PRODUCERTHREAD_HPP_

#include "api/VmsConnector.hpp"
#include "ApplicationModel.hpp"
#include "Exception.hpp"
#include "util/log/Loggers.hpp"

namespace opencctv {

class ProducerThread {
private:
	opencctv::api::VmsConnector* _pVmsConn;
	opencctv::ConcurrentQueue<opencctv::Image>* _pQueue;
	PluginLoader<api::VmsConnector>* _pPluginLoader;
	bool _bActive;
public:
	ProducerThread(unsigned int iStreamId, opencctv::api::VmsConnector* pVmsConn);
	void operator()();
	bool isStillRunning();
};

} /* namespace opencctv */

#endif /* OPENCCTV_PRODUCERTHREAD_HPP_ */
