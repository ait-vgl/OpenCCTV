/*
 * ResultsRouterThread.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: anjana
 */

#include "ResultsRouterThread.hpp"

ResultsRouterThread::ResultsRouterThread(string analyticOutQueueAddress, unsigned int imageCount)
{
	mqPtr = new TcpMq(1);
	_analyticOutQueueAddress = analyticOutQueueAddress;
	_iImageCount = imageCount;

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

		cout << "ResultsRouterThread::operator: "<<"Results router thread for queue " << _analyticOutQueueAddress << " started." << endl;

		int iStreamId = 0;
		int iAnalyticInstId = 0 ;
		int iAnalyticId = 0;
		string sImgTimestamp = "";
		string sResult = "";

		AnalyticResultGateway _analyticResultGateway;

		//Read the output results and store in the OpenCCTV database
		//TODO Later change this to a while loop?
		while(1)
		{
			//Read serialized Analytic Results
			string sResultsStr = mqPtr->read();
			cout << "sResultsStr : "<< sResultsStr << endl;

			AnalyticOutputMessage::getAnalyticResultsData(sResultsStr, iStreamId, iAnalyticInstId, iAnalyticId,sImgTimestamp,sResult);

			_analyticResultGateway.insertResults(iStreamId,iAnalyticInstId,iAnalyticId, sImgTimestamp,sResult);

		}

	}else{
		cerr << "ResultsRouterThread:operator: Error connecting to the analytic output queue......." << endl;
	}
}

