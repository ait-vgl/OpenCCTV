
#ifndef OPENCCTV_CONSUMERTHREAD_HPP_
#define OPENCCTV_CONSUMERTHREAD_HPP_

//#include "ApplicationModel.hpp"
#include "mq/TcpMqSender.hpp"
#include "ImageMulticaster.hpp"
//#include <vector>

#include "OpenCCTVServerManager.hpp"
#include "OpenCCTVServer.hpp"

#include "ConcurrentQueue.hpp"
#include "Image.hpp"

namespace opencctv {

class ConsumerThread {
private:
	ImageMulticaster* _pImageMulticaster;
	ConcurrentQueue<Image>* _pQueue; // should be deleted in Producer

public:
	ConsumerThread(unsigned int iStreamId, ImageMulticaster* pImageMulticaster);
	void operator()();
	~ConsumerThread();
};

} /* namespace opencctv */

#endif /* OPENCCTV_CONSUMERTHREAD_HPP_ */
