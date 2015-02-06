/*
 * ResultsRouterThread.hpp
 *
 *  Created on: Nov 5, 2014
 *      Author: anjana
 */

#ifndef RESULTSROUTERTHREAD_HPP_
#define RESULTSROUTERTHREAD_HPP_

#include <iostream>
#include <sstream>
#include <sys/stat.h>
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

#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;
using namespace db;
using namespace tcpsocket;
using namespace mq;
using namespace util;

class ResultsRouterThread {
private:
	TcpMq* mqPtr;
	string _analyticOutQueueAddress;
	unsigned int _iImageCount;
	int _iAnalyticInstId;

public:
	ResultsRouterThread(string analyticOutQueueAddress, unsigned int imageCount, int _iAnalyticInstId);
	virtual ~ResultsRouterThread();
	void operator ()();
};

#endif /* RESULTSROUTERTHREAD_HPP_ */
