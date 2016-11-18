/**
 * A simple thread-safe multiple producer, multiple consumer queue implemented using Boost mutex and conditional variable.
 * Reference: https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
 */

#ifndef OPENCCTV_CONCURRENTQUEUE_HPP_
#define OPENCCTV_CONCURRENTQUEUE_HPP_

#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <sys/time.h>

namespace opencctv {

template<typename T>
class ConcurrentQueue {
private:
	typedef struct {
		unsigned long long pushedTime;
		T* data;
	} Element;

	std::queue<Element> _queue;
	boost::mutex _mutex;
	boost::condition_variable _conditionVar;
	size_t size;

	unsigned long long getCurrentTimeMs() {
		unsigned long long ret = 0;
		struct timeval t;
		gettimeofday(&t, NULL);
		ret = (((unsigned long long) t.tv_sec) * 1000)
				+ (((unsigned long long) t.tv_usec) / 1000);
		return ret;
	}

public:
	ConcurrentQueue(size_t size) {
		this->size = size;
	}

	void push(T* data) {
		boost::mutex::scoped_lock lock(_mutex);
		if (_queue.size() >= size) {
			T* pDataToDelete = _queue.front().data;
			_queue.pop();
			if(pDataToDelete)
			{
				delete pDataToDelete;
			}
		}
		Element e = { getCurrentTimeMs(), data };
		_queue.push(e);
		lock.unlock();
		_conditionVar.notify_one();
	}

	unsigned long long tryPop(T* &pData) {
		boost::mutex::scoped_lock lock(_mutex);
		if (_queue.empty()) {
			return 0;
		}
		pData = _queue.front().data;
		unsigned long long ret = _queue.front().pushedTime;
		_queue.pop();
		return ret;
	}

	unsigned long long waitAndPop(T* &pData) {
		boost::mutex::scoped_lock lock(_mutex);
		while (_queue.empty()) {
			_conditionVar.wait(lock);
		}
		pData = _queue.front().data;
		unsigned long long ret = _queue.front().pushedTime;
		_queue.pop();
		return ret;
	}

	unsigned long long waitAndGetFrontElement(T* &pFrontData) {
		boost::mutex::scoped_lock lock(_mutex);
		while (_queue.empty()) {
			_conditionVar.wait(lock);
		}
		pFrontData = _queue.front().data;
		return _queue.front().pushedTime;
	}

	bool tryRemoveFrontElement() {
		boost::mutex::scoped_lock lock(_mutex);
		if (_queue.empty()) {
			return false;
		}
		T* pDataToDelete = _queue.front().data;
		_queue.pop();
		if(pDataToDelete)
		{
			delete pDataToDelete;
		}
		return true;
	}

	bool empty() const {
		boost::mutex::scoped_lock lock(_mutex);
		return _queue.empty();
	}

	void setSize(size_t size) {
		this->size = size;
	}
};

} //namespace opencctv
#endif //OPENCCTV_CONCURRENTQUEUE_HPP_
