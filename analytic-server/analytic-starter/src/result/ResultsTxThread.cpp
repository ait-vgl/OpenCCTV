/*
 * ResultsTxThread.cpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#include "ResultsTxThread.hpp"

namespace result
{

ResultsTxThread::ResultsTxThread(unsigned int iResultsAppInstId)
{
	_iResultsAppInstId = iResultsAppInstId;

	//Find the ResultAppInstController instance created for this results app instance
	analytic::ApplicationModel* pModel = analytic::ApplicationModel::getInstance();
	_pResultAppInstController = NULL;
	if(pModel->containsResultAppInstController(_iResultsAppInstId))
	{
		_pResultAppInstController = pModel->getResultAppInstControllers()[_iResultsAppInstId];
	}
}

void ResultsTxThread::operator ()()
{
	std::stringstream ssMsg;
	bool bReslt = true;

	//ResultsTxThread uses an object of ResultAppInstController to call functions
	//in the connector plugin of this results app instance.
	if(_pResultAppInstController)
	{
		//Initialize
		bReslt = _pResultAppInstController->init();
		if(bReslt)
		{
			//Authenticate
			bReslt = _pResultAppInstController->authenticate();
			if(bReslt)
			{
				//Send analytic instance info
				bReslt = _pResultAppInstController->sendAllAnalyticInstInfo();
				//Send analytic results
				_pResultAppInstController->sendAnalyticResults();
			}
		}
	}else
	{
		bReslt = false;
		ssMsg << "Failed to find the results app controller. ";
	}

	ssMsg << "Results app instance id: ";
	ssMsg << _iResultsAppInstId;
	ssMsg << ". Exiting results routing thread.";
	(bReslt) ? opencctv::util::log::Loggers::getDefaultLogger()->debug(ssMsg.str()) :
			   opencctv::util::log::Loggers::getDefaultLogger()->error(ssMsg.str());
}

ResultsTxThread::~ResultsTxThread()
{
}

} /* namespace result */
