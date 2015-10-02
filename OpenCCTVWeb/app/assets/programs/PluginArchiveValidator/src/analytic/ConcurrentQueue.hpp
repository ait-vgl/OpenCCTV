
#ifndef ANALYTIC_CONCURRENTQUEUE_HPP_
#define ANALYTIC_CONCURRENTQUEUE_HPP_

#include <queue>
#include <pthread.h>

namespace analytic {

template<typename T>
class ConcurrentQueue {
private:
	std::queue<T> _queue;
	pthread_mutex_t _mutex;
	pthread_cond_t _cond;
	size_t _size;
public:
	ConcurrentQueue(size_t size) {
		pthread_mutex_init(&_mutex, NULL);
		pthread_cond_init(&_cond, NULL);
		_size = size;
	}

	void push(T data) {
		pthread_mutex_lock(&_mutex);
		_queue.push(data);
		pthread_cond_signal(&_cond);
		pthread_mutex_unlock(&_mutex);
	}

	T pop() {
		pthread_mutex_lock(&_mutex);
		while (_queue.empty()) {
			pthread_cond_wait(&_cond, &_mutex);
		}
		T ret = _queue.front();
		_queue.pop();
		pthread_mutex_unlock(&_mutex);
		return ret;
	}

	bool empty() {
		pthread_mutex_lock(&_mutex);
		bool bRet = _queue.empty();
		pthread_mutex_unlock(&_mutex);
		return bRet;
	}

	virtual ~ConcurrentQueue() {
	}
};

} /* namespace analytic */

#endif /* ANALYTIC_CONCURRENTQUEUE_HPP_ */
