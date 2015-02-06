/*
 * ConsumerThread.hpp
 *
 *  Created on: Oct 9, 2014
 *      Author: lakindu
 */

#ifndef CONSUMERTHREAD_HPP_
#define CONSUMERTHREAD_HPP_

#include <iostream>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "ThreadSafeQueue.hpp"
#include "Image.hpp"
#include "db/Db.hpp"
#include "tcpsocket/TcpClient.hpp"
#include "AnalyticServerMessage.hpp"
#include "mq/TcpMq.hpp"
#include "ImageMulticaster.hpp"
#include "util/Util.hpp"
#include "ResultsRouterThread.hpp"

using namespace std;
using namespace db;
using namespace tcpsocket;
using namespace mq;
using namespace util;

class ConsumerThread
{
private:
	unsigned int consumerId;
	ThreadSafeQueue<Image>* queuePtr;
	string analyticServerName;
	unsigned int analyticServerPort;
	vector<string> analyticQueueAddresses;
	TcpMq* mqPtr;
	boost::thread_group* _resultsRouterThreadGroup;
	unsigned int imageCount;

	string sendAnalyticStartRequest(const string& sAnalyticLocation, const unsigned int iAnalyticId, const unsigned int iAnalyticInstId, const unsigned int iCameraId);


public:
	ConsumerThread(unsigned int consumerId, ThreadSafeQueue<Image>* queuePtr,string& analyticServerName, unsigned int analyticServerPort, boost::thread_group* resultsRouterThreadGroup, unsigned int imageCount);
	~ConsumerThread();
	void operator ()();
};

#endif /* CONSUMERTHREAD_HPP_ */
