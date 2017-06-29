/*
 * AnalyticInstController.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: anjana
 */

#include "AnalyticInstController.hpp"

namespace result
{

AnalyticInstController::AnalyticInstController()
{
}

/*AnalyticInstController::AnalyticInstController(unsigned int iAnalyticInstId)
{
	_iAnalyticInstId = iAnalyticInstId;

}*/

void AnalyticInstController::startResultsRouting(unsigned int iAnalyticInstId, std::string& sOutputMsg)
{
	std::stringstream ssErrMsg;
	analytic::ApplicationModel* pModel = analytic::ApplicationModel::getInstance();

	//Select the results_app_instance_ids this analytic instance is registered
	result::db::ResultsAppInstanceGateway resultsAppInstanceGateway;
	std::vector<result::db::dto::ResultsAppInstance> vResultsAppInstance;
	resultsAppInstanceGateway.findRAppInstancesForAnalyticInst(iAnalyticInstId, vResultsAppInstance);

	//For each results_app_instance
	for(size_t i = 0; i < vResultsAppInstance.size(); ++i)
	{
		result::db::dto::ResultsAppInstance rAppInst = vResultsAppInstance[i];
		unsigned int iResltsAppInstId = rAppInst.getResultsAppInstanceId();

		result::ResultAppInstController* pResultAppInstController = NULL;
		//If a results app instant controller already exist use it
		if(pModel->containsResultAppInstController(iResltsAppInstId))
		{
			pResultAppInstController = pModel->getResultAppInstControllers()[iResltsAppInstId];

		}else //Otherwise create a new results app instant controller
		{
			pResultAppInstController = new result::ResultAppInstController(iResltsAppInstId);
			//Add this results app instant controller to app model
			if(pResultAppInstController)
			{
				pModel->getResultAppInstControllers().insert(std::pair<unsigned int,
									result::ResultAppInstController*>(iResltsAppInstId,pResultAppInstController));
			}else
			{
				ssErrMsg << "Failed to create a controller for results application instant ";
				ssErrMsg << iResltsAppInstId << ".\n";
				opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				continue; //Skip the remaining steps in for loop
			}
		}

		pResultAppInstController->addAnalyticInstance(iAnalyticInstId);

		//If a thread does not exist for this results app instant create a one
		if(!pModel->containsResultsTxThread(iResltsAppInstId))
		{
			result::ResultsTxThread resultsTxThread(iResltsAppInstId);
			boost::thread* pThread = NULL;
			pThread = new boost::thread(resultsTxThread);
			if(pThread && pThread->joinable())
			{
				//Add this thread to the app model and the thread group
				pModel->getResultsTxThreads()[iResltsAppInstId] = pThread;
				pModel->getResultsTxThreadGroup()->add_thread(pThread);
				//pModel->getResultsTxThreadGroup()->join_all();
			}else
			{
				//Delete the thread if it is created
				delete pThread; pThread = NULL;
				//Remove the ResultAppInstController created for this thread
				if(pModel->containsResultAppInstController(iResltsAppInstId))
				{
					pResultAppInstController = pModel->getResultAppInstControllers()[iResltsAppInstId];
					delete pResultAppInstController; pResultAppInstController = NULL;
					pModel->getResultAppInstControllers().erase(iResltsAppInstId);
				}
				ssErrMsg << "Failed to initialize results transmission thread for results application ";
				ssErrMsg << iResltsAppInstId << ".\n";
				opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				continue; //Skip the remaining steps in for loop
			}
		}
	}

	sOutputMsg = ssErrMsg.str();
}

AnalyticInstController::~AnalyticInstController()
{
	// TODO Auto-generated destructor stub
}

} /* namespace result */
