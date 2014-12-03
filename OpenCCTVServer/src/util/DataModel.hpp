/*
 * DataModel.hpp
 *
 *  Created on: Oct 28, 2014
 *      Author: anjana
 */

#ifndef DATAMODEL_HPP_
#define DATAMODEL_HPP_

#include <map>

#include <boost/thread/thread.hpp>

#include "TypeDef.hpp"
#include "../Image.hpp"
#include "../ThreadSafeQueue.hpp"

namespace util {

using namespace std;

class DataModel {

private:
	//TODO : Do these maps need to be thread safe?
	map<int,AnalyticQueues> _mAlyticInstanceMap;
	map<int,ThreadSafeQueue<Image>*> _mImageQueueMap;

	//TODO : Need to determine whether to add the thread object or the boost thread object
	map<int,boost::thread*> _mProducerMap;
	map<int,boost::thread*> _mConsumerMap;
	map<int,boost::thread*> _mResultsRouterMap;


	static DataModel *_pdataModel;

	DataModel();
	DataModel(DataModel const&);
	void operator=(DataModel const&);

public:
	virtual ~DataModel();
	static DataModel* getInstance();
	map<int,AnalyticQueues>& getAlyticInstanceMap();
	map<int,ThreadSafeQueue<Image>*>& getImageQueueMap();
	map<int,boost::thread*>& getProducerMap();
	map<int,boost::thread*>& getConsumerMap();
	map<int,boost::thread*>& getResultsRouterMap();

};

} /* namespace util */

#endif /* DATAMODEL_HPP_ */
