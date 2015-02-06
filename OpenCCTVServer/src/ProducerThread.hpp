/*
 * ProducerThread.hpp
 *
 *  Created on: Oct 6, 2014
 *      Author: lakindu
 */

#ifndef PRODUCERTHREAD_HPP_
#define PRODUCERTHREAD_HPP_

#include "VmsClient.hpp"
#include "ThreadSafeQueue.hpp"
#include "Image.hpp"
#include <iostream>
#include "db/Db.hpp"
using namespace std;
using namespace db;

class ProducerThread
{
private:
	unsigned int _iProducerId;
	Stream _stream;
	VmsClient* vmsClientPtr;
	ThreadSafeQueue<Image>* queuePtr;
	unsigned int imageCount;

public:
	ProducerThread(unsigned int producerId, VmsClient* vmsClientPtr, ThreadSafeQueue<Image>* queuePtr, Stream &stream, unsigned int imageCount);
	~ProducerThread();
	void operator ()();
	bool isStillProcessing();
};

#endif /* PRODUCERTHREAD_HPP_ */
