
#ifndef OPENCCTV_CONSUMERTHREAD_HPP_
#define OPENCCTV_CONSUMERTHREAD_HPP_

#include "ApplicationModel.hpp"
#include "mq/TcpMqSender.hpp"
#include "ImageMulticaster.hpp"
#include <vector>

namespace opencctv {

class ConsumerThread {
private:
	ImageMulticaster* _pImageMulticaster;
	ConcurrentQueue<Image>* _pQueue;
public:
	ConsumerThread(unsigned int iStreamId, ImageMulticaster* pImageMulticaster);
	void operator()();
};

} /* namespace opencctv */

#endif /* OPENCCTV_CONSUMERTHREAD_HPP_ */
