/*
 * ProducerThread.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: lakindu
 */

#include "ProducerThread.hpp"

ProducerThread::ProducerThread(unsigned int producerId,VmsClient* vmsClientPtr, ThreadSafeQueue<Image>* queuePtr, Stream &stream, unsigned int imageCount)
{
	_iProducerId = producerId;
	this->vmsClientPtr = vmsClientPtr;
	this->imageCount = imageCount;
	this->queuePtr = queuePtr;
	_stream = stream;
}

ProducerThread::~ProducerThread()
{
//	delete vmsClientPtr;
}

void ProducerThread::operator ()()
{
	//cout << "ProducerThread::operator: "<<"Producer thread for stream " << _stream.getId() << " started." << endl;
	//cout << "Stream reader and decoder thread for stream " << _stream.getId() << " started." << endl;
	cout << "Decoder thread : Start decoding the images from the stream  : " << _stream.getId() << endl << endl;
	(*vmsClientPtr).produceImageObjects((*queuePtr), _stream, imageCount);
}

bool ProducerThread::isStillProcessing()
{
	return (*vmsClientPtr).isStillProducingImages();
}
