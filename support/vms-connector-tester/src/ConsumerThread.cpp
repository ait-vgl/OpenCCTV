
#include "ConsumerThread.hpp"

#include "analytic/util/ImageUtil.hpp"

#include <iostream>
using namespace std;

ConsumerThread::ConsumerThread(opencctv::ConcurrentQueue<opencctv::Image>* pQueue) {
	_pQueue = pQueue;
}

void ConsumerThread::operator ()()
{
	while(_pQueue)
	{
		opencctv::Image* pImage;
		unsigned long long lProducedTime = _pQueue->waitAndGetFrontElement(pImage);
		if(pImage)
		{
			cv::Mat matImage;
			analytic::util::ImageUtil::toOpenCvMat(pImage, matImage);
			cout << "Timestamp: " << pImage->getTimestamp() << endl;
			cv::imshow("Display", matImage); // display image
			cv::waitKey(10);
		}
		_pQueue->tryRemoveFrontElement();
	}
}

ConsumerThread::~ConsumerThread() {
}

