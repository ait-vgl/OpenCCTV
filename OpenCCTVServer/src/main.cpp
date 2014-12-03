#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <string>

#include "db/StreamGateway.hpp"
#include "db/CameraGateway.hpp"
#include "VmsDefinitions.hpp"
#include "VmsClient.hpp"
#include "milestone/MilestoneClient.hpp"
#include "Image.hpp"
#include "ProducerThread.hpp"
#include "util/DataModel.hpp"
#include "ConsumerThread.hpp"

using namespace std;
using namespace db;
using namespace milestone;
using namespace util;

int main(int argc, char* argv[])
{
	unsigned int _iImageCount = 1;
	if (argc > 1)
	{
		_iImageCount = atoi(argv[1]);
	}

	string _sAnalyticServerName = "192.41.170.213";
	//string _sAnalyticServerName = "localhost";
	unsigned int _iAnalyticServerPort = 5555;

	//Create an instance of DataModel class.
	//DataModel is a registry holding ref. to all the threads and queues created on the OpenCCTV server
	//It implements the singleton pattern.
	//TODO : The singleton pattern used in DataModel is not thread safe at the moment, is it necessary???
	DataModel *_pDataModel = DataModel::getInstance();

	map<int, ThreadSafeQueue<Image>*>& _mImageQueueMap = _pDataModel->getImageQueueMap();
	map<int, boost::thread*>& _mProducerMap = _pDataModel->getProducerMap();
	map<int, boost::thread*>& _mConsumerMap = _pDataModel->getConsumerMap();

	boost::thread_group _producerThreadGroup;
	boost::thread_group _consumerThreadGroup;
	boost::thread_group _resultsRouterThreadGroup;

	StreamGateway _streamGateway;
	vector<Stream> vStreams = _streamGateway.findAll();
	CameraGateway cameraGateway;
	for(size_t i = 0; i < vStreams.size(); i++)
	{
		Camera camera =  cameraGateway.find(vStreams[i].getCameraId());
		VmsClient* vmsPtr = NULL;
		if (camera.getVmsType().compare(VMS_TYPE_MILESTONE_XPROTECT) == 0)
		{
			//TODO : This needs to be deleted when a stream is stopped
			vmsPtr = new MilestoneClient(vStreams[i], camera);
		} else
		{
			//not yet implemented
			cerr << "OpenCCTVServer:main: Error: Invalid VMS type received from the database."
					<< std::endl;
		}
		if (vmsPtr != NULL && (*vmsPtr).init())
		{
			int _iStreamId = vStreams[i].getId();
			ThreadSafeQueue<Image>* queuePtr = new ThreadSafeQueue<Image>(_iStreamId);
			_mImageQueueMap[_iStreamId] = queuePtr;

			ProducerThread producer(_iStreamId, vmsPtr, queuePtr, vStreams[i], _iImageCount);
			boost::thread* pProducerThread = new boost::thread(producer);
			_mProducerMap[_iStreamId] = pProducerThread;

			if ((*pProducerThread).joinable())
			{
				ConsumerThread consumer(_iStreamId, queuePtr, _sAnalyticServerName, _iAnalyticServerPort, &_resultsRouterThreadGroup,  _iImageCount);
				boost::thread* pConsumerThread = new boost::thread(consumer);
				_mConsumerMap[_iStreamId] = pConsumerThread;

				if((*pConsumerThread).joinable())
				{
					_producerThreadGroup.add_thread(pProducerThread);
					_consumerThreadGroup.add_thread(pConsumerThread);
				}
			}
		}
		else
		{}
	}
	_producerThreadGroup.join_all();
	_consumerThreadGroup.join_all();
	_resultsRouterThreadGroup.join_all();
}
