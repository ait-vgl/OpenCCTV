/*
 * ImageMulticaster.hpp
 *
 *  Created on: Oct 13, 2014
 *      Author: lakindu
 */

#ifndef IMAGEMULTICASTER_HPP_
#define IMAGEMULTICASTER_HPP_

#include "mq/TcpMq.hpp"
#include "ThreadSafeQueue.hpp"
#include "Image.hpp"

using namespace mq;

class ImageMulticaster
{
private:
	ThreadSafeQueue<Image>* queuePtr;
	vector<string> analyticImageQueueAddresses;
	vector<TcpMq*> analyticInQueues;
public:
	ImageMulticaster(ThreadSafeQueue<Image>* queuePtr, vector<string> analyticImageQueueAddresses);
	void connectToImageQueues(const string& analyticServerName);
	void connectToImageQueues();
	void start();
	virtual ~ImageMulticaster();
};

#endif /* IMAGEMULTICASTER_HPP_ */
