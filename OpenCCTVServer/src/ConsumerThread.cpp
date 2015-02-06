/*
 * ConsumerThread.cpp
 *
 *  Created on: Oct 9, 2014
 *      Author: lakindu
 */

#include "ConsumerThread.hpp"

ConsumerThread::ConsumerThread(unsigned int consumerId, ThreadSafeQueue<Image>* queuePtr,string& analyticServerName, unsigned int analyticServerPort, boost::thread_group* resultsRouterThreadGroup, unsigned int imageCount)
{

	this->consumerId = consumerId; //This is the id of the associated stream
	this->queuePtr = queuePtr;
	this->analyticServerName = analyticServerName;
	this->analyticServerPort = analyticServerPort;
	mqPtr = new TcpMq();
	_resultsRouterThreadGroup = resultsRouterThreadGroup;
	this->imageCount = imageCount;
}

ConsumerThread::~ConsumerThread()
{
//	delete mqPtr;
//	mqPtr = NULL;
}

void ConsumerThread::operator ()()
{
	//Find all the analytic instances registered to the current stream consumer
	AnalyticInstStreamGateway _analyticInstStreamGateway;
	vector<AnalyticInstStream> _analyticInstStreams = _analyticInstStreamGateway.findAnalyticInstances(consumerId);

	//For each analytic instances registered to this stream consumer:

	//Case 1 :  The analytic instances has already started. Then obtain the input queue details
	//          of that instance from the AlyticInstanceMap of DataModel. Add the queue details
	//          to the recipient list of this consumer.

	//Case 2 : The analytic instance has not yet started. Then send a request to the analytic server
	//         to start the analytic instances and obtain the queue details for that instance.
	//         Add the instance details to the AlyticInstanceMap of DataModel. Also add the
	//         queue details to the recipient list of this consumer.
	//         Finally start a thread to route the analytic output results.

	DataModel *_pDataModel = DataModel::getInstance();

	map<int,AnalyticQueues>& _mAlyticInstanceMap = _pDataModel->getAlyticInstanceMap();

	//cout << "ConsumerThread::operator: "<<"Consumer thread for stream " << consumerId << " started." << endl;
	//cout << "Image router thread for stream " << consumerId << " started." << endl;
	cout << "Image router thread : Start sending images to analytic processes from the stream  : " << consumerId << endl << endl;
	(*mqPtr).connectTo(analyticServerName, analyticServerPort, ZMQ_REQ);

	for (size_t i = 0; i < _analyticInstStreams.size(); i++) {
		int instanceId = _analyticInstStreams[i].getAnalyticInstanceId();

		map<int, AnalyticQueues>::iterator it = _mAlyticInstanceMap.find(instanceId);

		if (it != _mAlyticInstanceMap.end()) {	//---------Case 1-----------
			AnalyticQueues analyticQueues = it->second;
			analyticQueueAddresses.push_back(analyticQueues.inputQueue);

		} else {	//----------Case 2---------------
			string sInputQueue = "";
			string sOutputQueue = "";
			string replyStr = sendAnalyticStartRequest(_analyticInstStreams[i].getLocation(), _analyticInstStreams[i].getAnalyticInstanceId(), _analyticInstStreams[i].getAnalyticId(), consumerId);

			//cout << "ConsumerThread::operator : replyStr = " << replyStr << endl;

			AnalyticServerMessage::getAnalyticQueueAddress(replyStr, sInputQueue, sOutputQueue);

			if (!sInputQueue.empty() && !sOutputQueue.empty()) {

				AnalyticQueues analyticQueues;
				analyticQueues.inputQueue = sInputQueue;
				analyticQueues.outputQueue = sOutputQueue;
				_mAlyticInstanceMap[instanceId] = analyticQueues;

				analyticQueueAddresses.push_back(analyticQueues.inputQueue);

				ResultsRouterThread resultsRouterThread(sOutputQueue, imageCount, _analyticInstStreams[i].getAnalyticInstanceId());
				boost::thread* pResultsRouterThread = new boost::thread(resultsRouterThread);
				map<int, boost::thread*>& _mResultsRouterMap = _pDataModel->getResultsRouterMap();
				_mResultsRouterMap[instanceId] = pResultsRouterThread;

				if ((*pResultsRouterThread).joinable()){
					_resultsRouterThreadGroup->add_thread(pResultsRouterThread);
				}

			} else {
				cerr << "ConsumerThread::operator: Error : Failed to extract valid image queue addresses for analytic " << _analyticInstStreams[i].getAnalyticId() << endl;
			}
		}

	}

	ImageMulticaster imageMulticaster(queuePtr,analyticQueueAddresses);
	imageMulticaster.connectToImageQueues();
	imageMulticaster.start();

	//_resultsRouterThreadGroup->join_all();


}

string ConsumerThread::sendAnalyticStartRequest(const string& sAnalyticLocation, const unsigned int iAnalyticId, const unsigned int iAnalyticInstId, const unsigned int iCameraId)
{
	string replyStr = "";

	string request = AnalyticServerMessage::getStartAnalyticRequest(sAnalyticLocation, iAnalyticId, iAnalyticInstId, iCameraId);
	(*mqPtr).write(request);
	replyStr = (*mqPtr).read();

	return replyStr;
}
