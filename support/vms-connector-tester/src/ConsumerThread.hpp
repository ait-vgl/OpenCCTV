
#ifndef CONSUMERTHREAD_HPP_
#define CONSUMERTHREAD_HPP_

#include "opencctv/ConcurrentQueue.hpp"
#include "opencctv/Image.hpp"
using namespace opencctv;

class ConsumerThread {
private:
	opencctv::ConcurrentQueue<opencctv::Image>* _pQueue;
public:
	ConsumerThread(opencctv::ConcurrentQueue<opencctv::Image>* pQueue);
	void operator ()();
	virtual ~ConsumerThread();
};

#endif /* CONSUMERTHREAD_HPP_ */
