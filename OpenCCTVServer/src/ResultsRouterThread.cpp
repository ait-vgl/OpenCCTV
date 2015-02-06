/*
 * ResultsRouterThread.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: anjana
 */

#include "ResultsRouterThread.hpp"

ResultsRouterThread::ResultsRouterThread(string analyticOutQueueAddress, unsigned int imageCount, int iAnalyticInstId)
{
	mqPtr = new TcpMq(1);
	_analyticOutQueueAddress = analyticOutQueueAddress;
	_iImageCount = imageCount;
	_iAnalyticInstId = iAnalyticInstId;
}

ResultsRouterThread::~ResultsRouterThread()
{
	// TODO Auto-generated destructor stub
}

void ResultsRouterThread::operator ()()
{
	//Connect to the analytic output queue
	bool bConnectResult = mqPtr->connectTo(_analyticOutQueueAddress,ZMQ_PULL);

	if(bConnectResult){

		//cout << "ResultsRouterThread::operator: "<<"Results router thread for analytic instance " << _iAnalyticInstId << " started." << endl;
		//cout << "Results router thread for analytic instance " << _iAnalyticInstId << " started." << endl;

		cout << "Results router thread : Start reading the images from the analytic process  : " << _iAnalyticInstId << endl << endl;

		AnalyticResultGateway _analyticResultGateway;
		Image imageResultObj;

		ostringstream filePath;
		filePath << "/usr/local/opencctv/images/";
		filePath << _iAnalyticInstId;
		filePath << "/";

		//TODO : Add the error handling to mkdir
		mkdir(filePath.str().c_str(), 0775);

		int iFrameCount = 1;

		//Read the output results and store in the OpenCCTV database
		while(1)		{

			//Read serialized results image object
			string serializedImageStr = mqPtr->read();

			//Initialize with received serialized string data
			std::istringstream ibuffer(serializedImageStr);

			//De-serialize and create image object
			boost::archive::text_iarchive iarchive(ibuffer);
			iarchive & imageResultObj;

			//TODO Define the image store location in a config file
			//1. Write the images to the path /usr/local/opencctv/images
			Mat matResultObj = JpegImage::toOpenCvMat(imageResultObj);

			ostringstream filename;
			filename << filePath.str();
			filename << imageResultObj.getTimestamp() << "_";
			filename << iFrameCount;
			filename << ".jpg";

			imwrite(filename.str(), matResultObj);

			++iFrameCount;

			//2. Store the image detais in the results DB
			_analyticResultGateway.insertResults(_iAnalyticInstId,imageResultObj.getTimestamp(),imageResultObj.getResult(),filename.str());

		}

	}else{
		cerr << "ResultsRouterThread:operator: Error connecting to the analytic output queue......." << endl;
	}
}

