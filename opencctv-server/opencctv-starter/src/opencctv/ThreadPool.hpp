/*
 * ThreadPool.h
 *
 *  Created on: Sep 1, 2016
 *      Author: nhutumdai
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_


#include <map>
#include <string>
#include <boost/thread/thread.hpp>

#include "ConsumerThread.hpp"
#include "ProducerThread.hpp"
#include "ResultRouterThread.hpp"

namespace opencctv {

class ThreadPool {

private:
	static ThreadPool* _pThreadPool;
	std::map<unsigned int, boost::thread*> _mProducerThreadManagers;
	std::map<unsigned int, boost::thread*> _mConsumerThreadManagers;
	std::map<unsigned int, boost::thread*> _mResultRouterThreadManagers;

	void consumerThreadStopAll();
	//void consumerInterruptAll();
	void producerThreadStopAll(int analyticServerId);
	//void producerInterruptAll();

	void resultRouterThreadStopAll();

public:

	ThreadPool();
	virtual ~ThreadPool();

	//void interruptAll();
	void stopAll(int analyticServerId);



	//void setResultRouterThreadManagers(unsigned int analyticId, const std::map<unsigned int, ::ResultRouterThread*>& resultRouterThreadManagers);

	void setConsumerThreadManagers(unsigned int streamId, boost::thread* pConsumerThread);

	void setProducerThreadManagers(unsigned int streamId, boost::thread* pProducerThread);

	void setResultRouterThreadManagers(unsigned int analyticId, boost::thread* pResulterThread);

	static ThreadPool* getInstance();

	//static const std::map<unsigned int, boost::thread*>& getProducerThreadManagers() {
	//	return _mProducerThreadManagers;
	//}


};

} /* namespace  */

#endif /* THREADPOOL_H_ */
