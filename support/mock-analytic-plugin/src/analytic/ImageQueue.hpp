/*
 * ImageQueue.hpp
 *
 *  Created on: Jun 18, 2015
 *      Author: anjana
 */

#ifndef IMAGEQUEUE_HPP_
#define IMAGEQUEUE_HPP_

#include <queue>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

namespace analytic {

template<typename T>
class ImageQueue {
private:
	std::queue<T> _queue;
	boost::mutex _mutex;
	boost::condition_variable _conditionVar;
	size_t size;

public:
	ImageQueue(size_t size) {
		this->size = size;
	};

	void push(T const& data)
	{
		boost::mutex::scoped_lock lock(_mutex);

		while(_queue.size() >= size)
		{
			_conditionVar.wait(lock);
		}

		_queue.push(data);
		lock.unlock();
		_conditionVar.notify_one();
	}

	T pop()
	{
		boost::mutex::scoped_lock lock(_mutex);

		while(_queue.empty())
		{
			_conditionVar.wait(lock);
		}

		T result =_queue.front();
		_queue.pop();

		lock.unlock();
		_conditionVar.notify_one();

		return result;
	}

	bool empty() const
	{
		boost::mutex::scoped_lock lock(_mutex);
		return _queue.empty();
	}

	bool full() const
	{
		boost::mutex::scoped_lock lock(_mutex);
		return (_queue.size() >= size);

	}

	virtual ~ImageQueue()
	{
	}
};

} /* namespace analytic */

#endif /* IMAGEQUEUE_HPP_ */
