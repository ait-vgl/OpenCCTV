/*
 * DataModel.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: anjana
 */

#include "DataModel.hpp"

namespace util {

DataModel* DataModel::_pdataModel = NULL;

DataModel::DataModel()
{
}

DataModel::~DataModel()
{
}

DataModel* DataModel::getInstance()
{
   if (!_pdataModel){   // Only allow one instance of class to be generated.
	   _pdataModel = new DataModel;
//	   cout << "--------Calling the DataModel constructor !!! --------------------" <<endl;
   }

   return _pdataModel;
}

map<int,AnalyticQueues>& DataModel::getAlyticInstanceMap(){
	return _mAlyticInstanceMap;
}

map<int,ThreadSafeQueue<Image>*>& DataModel::getImageQueueMap(){
	return _mImageQueueMap;
}

map<int,boost::thread*>& DataModel::getProducerMap(){
	return _mProducerMap;
}

map<int,boost::thread*>& DataModel::getConsumerMap(){
	return _mConsumerMap;
}

map<int,boost::thread*>& DataModel::getResultsRouterMap(){
	return _mResultsRouterMap;
}

} /* namespace util */
