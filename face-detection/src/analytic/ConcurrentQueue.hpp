
#ifndef GENDERDETECTIONANALYTIC_SRC_ANALYTIC_CONCURRENTQUEUE_HPP_
#define GENDERDETECTIONANALYTIC_SRC_ANALYTIC_CONCURRENTQUEUE_HPP_

#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

namespace analytic {

template<typename T>
class ConcurrentQueue {
private:
	std::queue<T> _queue;
	pthread_mutex_t _mutex;
	sem_t _full, _empty;
	size_t _size;
public:
	ConcurrentQueue(size_t size) {
		pthread_mutex_init(&_mutex, NULL);
		sem_init(&_full, 0, 0);
		sem_init(&_empty, 0, _size);
		_size = size;
	}

	void push(T data) {
		/* acquire the empty lock */
		sem_wait(&_empty);
		pthread_mutex_lock(&_mutex);
		_queue.push(data);
		pthread_mutex_unlock(&_mutex);
		/* signal full */
		sem_post(&_full);
	}

	T pop() {
		 /* aquire the full lock */
		sem_wait(&_full);
		pthread_mutex_lock(&_mutex);
		T ret = _queue.front();
		_queue.pop();
		pthread_mutex_unlock(&_mutex);
		/* signal empty */
		sem_post(&_empty);
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

#endif /* GENDERDETECTIONANALYTIC_SRC_ANALYTIC_CONCURRENTQUEUE_HPP_ */
