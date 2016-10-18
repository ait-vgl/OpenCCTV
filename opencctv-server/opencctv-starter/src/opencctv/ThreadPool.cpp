/*
 * ThreadPool.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: nhutumdai
 */

#include "ThreadPool.hpp"

namespace opencctv {

ThreadPool* ThreadPool::_pThreadPool = NULL; // Static variable has to be written again.
ThreadPool::ThreadPool() {
	// TODO Auto-generated constructor stub
}

ThreadPool* ThreadPool::getInstance() {
	if (!_pThreadPool) {
		_pThreadPool = new ThreadPool();
	}
	return _pThreadPool;
}

void ThreadPool::stopAll(int analyticServerId) {

	//consumerThreadStopAll();

	//resultRouterThreadStopAll();



	//producerThreadStopAll(analyticServerId);
}
/*
void ThreadPool::consumerThreadStopAll() {

	std::cout << "1.0 start to stop consumer" << std::endl;

	// TODO:: Check delete per analytic server
	std::map<unsigned int, boost::thread*>::iterator it;


	// For all consumer in opencctv

	std::cout << "1.0.1 Consumer all thread size " << _mConsumerThreadManagers.size() << std::endl;

	for (it = _mConsumerThreadManagers.begin(); it != _mConsumerThreadManagers.end(); ++it) {

		boost::thread* pConsumerThread = it->second;

		pthread_t id;

		if (pConsumerThread) {

			id = pConsumerThread->native_handle();
			pConsumerThread->detach();

			pthread_cancel(id);

			if (pConsumerThread) {
				delete pConsumerThread;
				pConsumerThread = NULL;
			}


		//	_mConsumerThreadManagers.erase(it->first);
			opencctv::util::log::Loggers::getDefaultLogger()->error("1. Delete Consumer");
		}
	}
}

void ThreadPool::producerThreadStopAll(int iAnalyticServerid) {

	std::cout << "2.0 start to stop producer" << std::endl;

	// TODO:: Check delete per analytic server

	std::map<unsigned int, boost::thread*>::iterator it;

	// Get stream manage check and delete


	////std::map<unsigned int, std::vector<unsigned int> > mStreamManagers = ApplicationModel::getInstance()->getStreamManages();


	// for All stream in opencctv
	for (it = _mProducerThreadManagers.begin(); it != _mProducerThreadManagers.end(); ++it) {

		////if (ApplicationModel::getInstance()->isStreamRunning(it->first)) {

			////std::cout << "Before delete producer " << it->first << " : " << mStreamManagers[it->first].size() << std::endl;

		//	if (mStreamManagers[it->first] > 1) { // Check analytic per stream is more than one, it's meant that other analytics are using.
		//		mStreamManagers[it->first]--; // reduce value of analytic running count
		//	}else {
				boost::thread* pProducerThread = it->second;

				pthread_t id;
				if (pProducerThread) {

					id = pProducerThread->native_handle();

					pProducerThread->detach();

					pthread_cancel(id);

					if (pProducerThread) {
						delete pProducerThread;
						pProducerThread = NULL;
					}

					// Delete Stream when there is only one analytic per stream
					////ApplicationModel::getInstance()->removeDataByStreamId(it->first);

					opencctv::util::log::Loggers::getDefaultLogger()->error("2. Delete Producer");

				}

			//}

			//	_mProducerThreadManagers.erase(it->first);
		}
	}
}

void ThreadPool::resultRouterThreadStopAll() {

	std::map<unsigned int, boost::thread*>::iterator it;

	for (it = _mResultRouterThreadManagers.begin();
			it != _mResultRouterThreadManagers.end(); ++it) {

		boost::thread* pResultRouterTherad = it->second;

		pthread_t id;
		if (pResultRouterTherad) {

			id = pResultRouterTherad->native_handle();
			pResultRouterTherad->detach();

			pthread_cancel(id);

			if (pResultRouterTherad) {
				delete pResultRouterTherad;
				pResultRouterTherad = NULL;
			}


		//	_mResultRouterThreadManagers.erase(it->first);
			opencctv::util::log::Loggers::getDefaultLogger()->error(
					"3. Delete Result router ");
		}
	}
}


void ThreadPool::setConsumerThreadManagers(unsigned int streamId, boost::thread* pConsumerThread) {

	if((_mConsumerThreadManagers.find(streamId)) == _mConsumerThreadManagers.end()){
		_mConsumerThreadManagers[streamId] = pConsumerThread;
	}
}

void ThreadPool::setProducerThreadManagers(unsigned int streamId,boost::thread* pProducerThread) {

	if((_mProducerThreadManagers.find(streamId)) == _mProducerThreadManagers.end()){
		_mProducerThreadManagers[streamId] = pProducerThread;
	}
}

void ThreadPool::setResultRouterThreadManagers(unsigned int analyticId,
		boost::thread* pResulterThread) {
	_mResultRouterThreadManagers[analyticId] = pResulterThread;
}
*/
ThreadPool::~ThreadPool() {
	// TODO Auto-generated destructor stub
}

} /* namespace  */
