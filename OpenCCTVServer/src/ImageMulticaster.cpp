/*
 * ImageMulticaster.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: lakindu
 */

#include "ImageMulticaster.hpp"
#include <boost/lexical_cast.hpp>

ImageMulticaster::ImageMulticaster(ThreadSafeQueue<Image>* queuePtr, vector<string> analyticImageQueueAddresses)
{
	this->queuePtr = queuePtr;
	this->analyticImageQueueAddresses = analyticImageQueueAddresses;
}

void ImageMulticaster::connectToImageQueues(const string& analyticServerName)
{
	zmq::context_t* contextPtr = new zmq::context_t(1);
	for (size_t i = 0; i < analyticImageQueueAddresses.size(); i++)
	{
		TcpMq* mqPtr = new TcpMq(contextPtr);
		(*mqPtr).connectTo(analyticServerName, boost::lexical_cast<unsigned int>(analyticImageQueueAddresses[i]), ZMQ_PUSH);
		analyticInQueues.push_back(mqPtr);
	}
}

void ImageMulticaster::connectToImageQueues()
{
	zmq::context_t* contextPtr = new zmq::context_t(1);
	for (size_t i = 0; i < analyticImageQueueAddresses.size(); i++)
	{
		TcpMq* mqPtr = new TcpMq(contextPtr);
		(*mqPtr).connectTo(analyticImageQueueAddresses[i], ZMQ_PUSH);
		analyticInQueues.push_back(mqPtr);
	}
}

void ImageMulticaster::start()
{
	while(true)
	{
	Image image = (*queuePtr).waitAndPop();
	//Prepare the output buffer
	std::ostringstream obuffer;
	//Initialize the archive
	boost::archive::text_oarchive oarchive(obuffer);
	//Serialize the image object
	oarchive & (image);
	//Get the serialization data (a string) from the buffer
	std::string outStr(obuffer.str());

	for (size_t i = 0; i < analyticInQueues.size(); i++)
	{
		(*analyticInQueues[i]).write(outStr);
		//cout << "Image sent - "<< "\tStream ID: " << image.getStreamId() << "\tTimestamp: " << image.getTimestamp() << endl;

		//cout << "\tStream ID: " << image.getStreamId() << endl;
		//cout << "\tTimestamp: " << image.getTimestamp() << endl;
		//cout << "\tImage data size: " << (*imagePtr).getImageData().size() << endl;
		//cout << "\tData: " << endl;
		//for(int i = 0; i < 5; i++)
		//{
		//	int chr = (*imagePtr).getImageData()[i];
		//	cout << "\t" << chr;
		//}
		//cout << endl;
	}
	}
}

ImageMulticaster::~ImageMulticaster()
{

}

