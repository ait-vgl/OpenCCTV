/*
 * ThreadSafeQueue.hpp
 *
 *  Created on: Oct 5, 2014
 *      Author: Anjana Tissera
 */

#ifndef THREADSAFEQUEUE_HPP_
#define THREADSAFEQUEUE_HPP_

#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;

template<typename Data>
class ThreadSafeQueue
{
private:
	int _iStreamId;
	queue<Data>* queuePtr;
	mutable boost::mutex* mutexPtr;
	boost::condition_variable* conditionVarPtr;

public:
	ThreadSafeQueue(int iStreamId)
	{
		this->_iStreamId = iStreamId;
		queuePtr = new queue<Data>();
		mutexPtr = new boost::mutex();
		conditionVarPtr = new boost::condition_variable();
	}
	~ThreadSafeQueue()
	{
		delete conditionVarPtr;
		delete mutexPtr;
		delete queuePtr;
	}
	void push(Data dataPtr)
	{
		boost::mutex::scoped_lock lock(*mutexPtr);
		(*queuePtr).push(dataPtr);
		lock.unlock();
		(*conditionVarPtr).notify_all();
	}
	bool empty() const
	{
		bool result;
		boost::mutex::scoped_lock lock(*mutexPtr);
		result = (*queuePtr).empty();
		lock.unlock();
		return result;
	}
	Data waitAndPop()
	{
		Data poppedValuePtr;
		boost::mutex::scoped_lock lock(*mutexPtr);
		while ((*queuePtr).empty())
		{
			(*conditionVarPtr).wait(lock);
			boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		}
		poppedValuePtr = (*queuePtr).front();
		(*queuePtr).pop();
		lock.unlock();
		(*conditionVarPtr).notify_all();
		return poppedValuePtr;
	}

	int getStreamId()
	{
			return this->_iStreamId;

	}
};

#endif /* THREADSAFEQUEUE_HPP_ */
