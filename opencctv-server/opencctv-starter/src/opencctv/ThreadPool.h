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

	void consumerThreadJoinAll();
	void consumerInterruptAll();
	void producerThreadJoinAll();
	void producerInterruptAll();

	void resultRouterThreadJoinAll();

public:

	ThreadPool();
	virtual ~ThreadPool();

	void interruptAll();
	void joinAll();



	//void setResultRouterThreadManagers(unsigned int analyticId, const std::map<unsigned int, ::ResultRouterThread*>& resultRouterThreadManagers);

	void setConsumerThreadManagers(unsigned int analyticId, boost::thread* pConsumerThread);

	void setProducerThreadManagers(unsigned int analyticId, boost::thread* pProducerThread);

	void setResultRouterThreadManagers(unsigned int analyticId, boost::thread* pResulterThread);

	static ThreadPool* getInstance();

	//static const std::map<unsigned int, boost::thread*>& getProducerThreadManagers() {
	//	return _mProducerThreadManagers;
	//}


};

} /* namespace  */

#endif /* THREADPOOL_H_ */
