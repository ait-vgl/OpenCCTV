/*
 * AnalyticData.cpp
 *
 *  Created on: Sep 16, 2016
 *      Author: nhutumdai
 */

#include "AnalyticData.hpp"

namespace analytic
{
AnalyticData::AnalyticData(int iId)
{
	//this->_iId = iId;
	//this->_bStatus = true;
	//this->_sAnalyticQueueInAddress = "";
	this->_sAnalyticQueueOutAddress = "";
	//this->_pFlowController = NULL;
	this->_pResultRouterThread = NULL;
}
/*
bool AnalyticData::isAnalyticQueueInAddress() {

	if (_sAnalyticQueueOutAddress != "") {
		return true;
	} else
		return false;
}
*/
bool AnalyticData::isAnalyticQueueOutAddress()
{

	if (_sAnalyticQueueOutAddress != "")
	{
		return true;
	}
	else
		return false;
}
/*
bool AnalyticData::isMultipleStream() {
	if (!(this->_vStreamIds.empty()) && this->_vStreamIds.size() > 1) {
		return true;
	} else
		return false;
}

bool AnalyticData::isFlowController(){
	if(_pFlowController)
		return true;
	else
		return false;
}
*/

AnalyticData::~AnalyticData()
{
	pthread_t id;

	if (_pResultRouterThread)
	{

		id = _pResultRouterThread->native_handle();
		_pResultRouterThread->detach();

		pthread_cancel(id);

		if (_pResultRouterThread)
		{
			delete _pResultRouterThread;
			_pResultRouterThread = NULL;
		}

		opencctv::util::log::Loggers::getDefaultLogger()->debug("Clear Result router thread at AnalaticData.");
	}
}



} // end namespace analytic
