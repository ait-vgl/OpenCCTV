/*
 * StreamData.h
 *
 *  Created on: Sep 26, 2016
 *      Author: nhutumdai
 */

#ifndef STREAMDATA_H_
#define STREAMDATA_H_

#include "ConcurrentQueue.hpp"
#include "Image.hpp"

#include "mq/TcpMqSender.hpp"

namespace opencctv {

class StreamData {
private:
	int _iId;
	boost::thread* _pProducerThread;
	boost::thread* _pConsumerThread;
	std::vector<unsigned int> _vAnalyticIds;
	ConcurrentQueue<Image>* _pQueue;
	unsigned int _iVMSConnectorId;

	std::map<unsigned int, mq::Sender*> _mMulticastDestinations; // Analytic instance id as key

public:

	bool isInternalQueue();


	StreamData(int _iId);
	virtual ~StreamData();

	const std::map<unsigned int, mq::Sender*>& getMulticastDestinations() {
		return _mMulticastDestinations;
	}

	void setMulticastDestinations(const std::map<unsigned int, mq::Sender*>& multicastDestinations) {
		_mMulticastDestinations = multicastDestinations;
	}

	boost::thread* getConsumerThread() {
		return _pConsumerThread;
	}

	void setConsumerThread(boost::thread* consumerThread) {
		_pConsumerThread = consumerThread;
	}

	boost::thread* getProducerThread(){
		return _pProducerThread;
	}

	void setProducerThread(boost::thread* producerThread) {
		_pProducerThread = producerThread;
	}

	ConcurrentQueue<Image>* getInternalQueue() {
		return _pQueue;
	}

	void setInternalQueue(ConcurrentQueue<Image>* queue) {
		_pQueue = queue;
	}
};

} /* namespace opencctv */

#endif /* STREAMDATA_H_ */
