/*
 * AnalyticServerController.cpp
 *
 *  Created on: May 8, 2017
 *      Author: anjana
 */

#include "AnalyticServerController.hpp"

namespace analytic {

AnalyticServerController* AnalyticServerController::_pAnalyticServerController = NULL;

AnalyticServerController* AnalyticServerController::getInstance()
{
	if (!_pAnalyticServerController)
	{
		try
		{
			_pAnalyticServerController = new AnalyticServerController();
		}catch(opencctv::Exception& e)
		{
			throw e;
		}
	}
	return _pAnalyticServerController;
}

AnalyticServerController::AnalyticServerController()
{
	// Loading Configuration file
	_pConfig = NULL;
	try
	{
		_pConfig = analytic::util::Config::getInstance();
	}catch(opencctv::Exception& e)
	{
		throw e;
	}

	opencctv::util::log::Loggers::getDefaultLogger()->info("Loading Configuration file done.");

	// Initializing values
	//Read the starting port
	std::string sStartingPort = _pConfig->get(analytic::util::PROPERTY_STARTER_PORT);
	if(sStartingPort.empty())
	{
		throw opencctv::Exception("Failed to retrieve Analytic Starter port from Configuration file.");
	}
	//STARTING_PORT = boost::lexical_cast<unsigned int>(sStartingPort);
	iPort = boost::lexical_cast<unsigned int>(sStartingPort);

	//Read the number of analytics
	std::string sNoAnalytics = _pConfig->get(analytic::util::PROPERTY_NUM_OF_ANALYTICS);
	if(sNoAnalytics.empty())
	{
		throw opencctv::Exception("Failed to retrieve Number of Analytics from Configuration file.");
	}

	iNumOfAnalytics = boost::lexical_cast<unsigned int>(sNoAnalytics);

	//fillIOPorts();

	opencctv::util::log::Loggers::getDefaultLogger()->info("Initializing configuration details done.");

	// Creating Analytic Server's request-reply MQ
	try
	{
		_pSocket = opencctv::mq::MqUtil::createNewMq(sStartingPort, ZMQ_REP);
	}
	catch(std::runtime_error &e)
	{
		std::string sErrMsg = "Failed to create Analytic Server's request-reply MQ. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	opencctv::util::log::Loggers::getDefaultLogger()->info("Creating Analytic Server's request-reply MQ done.");
}

void AnalyticServerController::executeOperation()
{
	std::string sRequest;
	std::string sOperation;
	std::string sMessage;

	//Read the request
	try
	{
		opencctv::mq::MqUtil::readFromSocket(_pSocket, sRequest);
		sOperation = analytic::xml::AnalyticMessage::extractAnalyticRequestOperation(sRequest);
	}
	catch(opencctv::Exception &e)
	{
		sOperation = analytic::xml::OPERATION_UNKNOWN;
		sMessage = e.what();
	}

	//Execute the operation according to the operation type
	std::string sReply;
	if(sOperation.compare(analytic::xml::OPERATION_START_ANALYTIC) == 0)
	{
		sReply = startAnalytic(sRequest);
		sendReply(sReply);
	}
	else if(sOperation.compare(analytic::xml::OPERATION_STOP_ANALYTIC) == 0)
	{
		sReply = stopAnalytic(sRequest);
		sendReply(sReply);

	}
	else if(sOperation.compare(analytic::xml::OPERATION_KILL_ALL_ANALYTICS) == 0)
	{
		sReply = killAllAnalytics(sRequest);
		sendReply(sReply);
	}
	else
	{
		sendReply(reportError(analytic::xml::OPERATION_UNKNOWN, false,
				"Request with an unknown Operation",sMessage));
	}
}

std::string AnalyticServerController::startAnalytic(const std::string& sRequest)
{
	// Request data
	unsigned int iAnalyticInstanceId = 0;
	std::string sAnalyticDirPath;
	std::string sAnalyticFilename;

	//Reply data
	std::string sReply;
	bool bAIStarted = false;
	std::stringstream ssErrMsg;
	std::string sErrMsg;
	std::string sAnalyticInputQueueAddress;
	std::string sAnalyticOutputQueueAddress;

	// TODO Check maximum num. of analytic instances???

	//Step 1 - Extract data from the request message
	try
	{
		analytic::xml::AnalyticMessage::extractAnalyticStartRequestData(sRequest, iAnalyticInstanceId, sAnalyticDirPath, sAnalyticFilename);
	}
	catch (opencctv::Exception &e)
	{

		return reportError(analytic::xml::OPERATION_START_ANALYTIC,
				false, "Failed to extract data from analytic start request",e.what());
	}

	if((iAnalyticInstanceId <= 0) || sAnalyticDirPath.empty() || sAnalyticFilename.empty())
	{

		return reportError(analytic::xml::OPERATION_START_ANALYTIC,
						false, "Analytic start request with invalid data","");
	}

	//Step 2 - Start the analytic instance

	// TODO Check if a process already exist with the same analytic instance id
	//If so stop the analytic instance and start the analytic instance again

	// Creating Analytic process
	analytic::AnalyticProcess *pAnalyticProcess = new analytic::AnalyticProcess();
	if(!pAnalyticProcess)
	{
		return reportError(analytic::xml::OPERATION_START_ANALYTIC,
								false, "Out of memory - failed to create new analytic instance","");
	}

	// TODO Remove later=========
	sAnalyticInputQueueAddress = "9999"; //= boost::lexical_cast<std::string>(++iPort);
	sAnalyticOutputQueueAddress = "9999";//= boost::lexical_cast<std::string>(++iPort);
	// ==========================

	try
	{
		std::string sAnalyticRunnerPath;
		sAnalyticRunnerPath.append(_pConfig->get(analytic::util::PROPERTY_ANALYTIC_RUNNER_DIR));
		sAnalyticRunnerPath.append("/");
		sAnalyticRunnerPath.append(_pConfig->get(analytic::util::PROPERTY_ANALYTIC_RUNNER_FILENAME));
		//std::cout << "From Starter: Instance id " + boost::lexical_cast<std::string>(iAnalyticInstanceId) + " plugin location" + sAnalyticDirPath + " plugin file anem" + sAnalyticFilename + " Input Queue port" + sAnalyticInputQueueAddress + " Output queue port" + sAnalyticOutputQueueAddress << std::endl;
		bAIStarted = pAnalyticProcess->startAnalytic(sAnalyticRunnerPath, iAnalyticInstanceId,
				sAnalyticDirPath, sAnalyticFilename, sAnalyticInputQueueAddress, sAnalyticOutputQueueAddress);
	}catch(opencctv::Exception &e)
	{
		sErrMsg = e.what();
		bAIStarted = false;
	}

	if(!bAIStarted)
	{
		ssErrMsg << "Failed to start analytic instance " << iAnalyticInstanceId;
		return reportError(analytic::xml::OPERATION_START_ANALYTIC,
										false, ssErrMsg.str(),sErrMsg);
	}

	analytic::ApplicationModel* pModel = analytic::ApplicationModel::getInstance();

	//Record the details of the analytic process in ApplicationModel
	pModel->getAnalyticProcesses()[iAnalyticInstanceId] = pAnalyticProcess;

	//Step 3 - Start the results routing threads for each results app instance
	//         registered for this analytic instance

	//Select the results_app_instance_ids this analytic instance is registered
	result::db::ResultsAppInstanceGateway resultsAppInstanceGateway;
	std::vector<result::db::dto::ResultsAppInstance> vResultsAppInstance;
	resultsAppInstanceGateway.findRAppInstancesForAnalyticInst(iAnalyticInstanceId, vResultsAppInstance);

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

		pResultAppInstController->addAnalyticInstance(iAnalyticInstanceId);

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
				ssErrMsg << "Failed to initialize results transmission thread for results application instant ";
				ssErrMsg << iResltsAppInstId << ".\n";
				opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				continue; //Skip the remaining steps in for loop
			}
		}
	}

	//Step 4 - Return the reply XML message
	std::stringstream ssMsg;
	ssMsg << "Analytic instance " << iAnalyticInstanceId << " started.";
	opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
	if(!ssErrMsg.str().empty())
	{
		ssMsg << " But, few errors occurred during the operation. Check the analytic server log for more details.";
	}

	try
	{
		sReply = analytic::xml::AnalyticMessage::getAnalyticStartReply(true, ssMsg.str(), sAnalyticInputQueueAddress, sAnalyticOutputQueueAddress);
	}
	catch (opencctv::Exception &e)
	{
		sReply = reportError(analytic::xml::OPERATION_START_ANALYTIC,
				bAIStarted, "Failed to create analytic start reply XML message",e.what());
	}

	return sReply;
}

std::string AnalyticServerController::stopAnalytic(const std::string& sRequest)
{
	// Request data
	unsigned int iAnalyticInstanceId;

	//Reply data
	std::string sReply;
	bool bDone = false;
	std::string sErrMsg;

	// Extracting request data
	try
	{
		analytic::xml::AnalyticMessage::extractAnalyticStopRequestData(sRequest, iAnalyticInstanceId);
	}
	catch (opencctv::Exception &e)
	{
		sErrMsg = "Failed to extract data from analytic stop request. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		sReply = analytic::xml::AnalyticMessage::getErrorReply(analytic::xml::OPERATION_STOP_ANALYTIC,false,sErrMsg);
		return sReply;
	}

	//Find the process for the requested analytic instance
	std::map<unsigned int, analytic::AnalyticProcess *> mAnalyticProcesses = analytic::ApplicationModel::getInstance()->getAnalyticProcesses();
	std::map<unsigned int, analytic::AnalyticProcess *>::iterator it =  mAnalyticProcesses.find(iAnalyticInstanceId);

	if(it == mAnalyticProcesses.end())
	{
		std::stringstream ssMsg;
		ssMsg << "Failed to stop the analytic instance. Analytic instance id. ";
		ssMsg << iAnalyticInstanceId << " is invalid.";
		sReply = analytic::xml::AnalyticMessage::getErrorReply(analytic::xml::OPERATION_STOP_ANALYTIC,bDone,ssMsg.str());
		return sReply;
	}

	// Stopping the analytic process
	analytic::AnalyticProcess *pAnalyticProcess = it->second;
	bDone = pAnalyticProcess->stopAnalytic();

	if(!bDone)
	{
		std::stringstream ssMsg;
		ssMsg << "Error occurred while stopping the analytic instance " << iAnalyticInstanceId << ".";
		sReply = analytic::xml::AnalyticMessage::getErrorReply(analytic::xml::OPERATION_STOP_ANALYTIC,bDone,ssMsg.str());
		return sReply;
	}

	//Remove the analytic process from the ApplicationModel
	//mAnalyticProcesses.erase(it);

	//Free the memory taken by the analytic process
	/*if (pAnalyticProcess)
	{
		delete pAnalyticProcess; pAnalyticProcess = NULL;
	}*/

	// TODO Remove the analytic process and its results threads when all results are transmitted
	pAnalyticProcess->setIsActive(false);

	bDone = true;

	//Return the reply XML message
	try
	{
		std::stringstream ssMsg;
		ssMsg << "Analytic instance " << iAnalyticInstanceId << " stopped successfully.";
		sReply = analytic::xml::AnalyticMessage::getAnalyticStopReply(bDone,ssMsg.str());
		opencctv::util::log::Loggers::getDefaultLogger()->debug("Finished stopping analytic instance, replied");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create analytic stop reply. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		sReply = analytic::xml::AnalyticMessage::getErrorReply(analytic::xml::OPERATION_STOP_ANALYTIC, bDone, sErrMsg);
	}

	return sReply;

}

std::string AnalyticServerController::killAllAnalytics(const std::string& sRequest)
{
	//bool bDone = false;
	std::string sReply;
	std::string sErrMsg;
	std::stringstream ssNotStoppedAnalytics;
	std::string separator = "";

	std::map<unsigned int, analytic::AnalyticProcess *> mAnalyticProcesses = analytic::ApplicationModel::getInstance()->getAnalyticProcesses();
	std::map<unsigned int, analytic::AnalyticProcess *>::iterator it;

	for (it = mAnalyticProcesses.begin(); it != mAnalyticProcesses.end(); ++it)
	{
		analytic::AnalyticProcess *pAnalyticProcess = it->second;

		if (pAnalyticProcess->stopAnalytic())
		{
			/*//Remove the analytic process from the ApplicationModel
			mAnalyticProcesses.erase(it);

			//Free the memory taken by the analytic process
			if (pAnalyticProcess)
			{
				delete pAnalyticProcess; pAnalyticProcess = NULL;
			}*/

			// TODO Remove the analytic process and its results threads when all results are transmitted
			pAnalyticProcess->setIsActive(false);
		}
		else
		{
			ssNotStoppedAnalytics << separator << it->first;
			separator = ", ";
		}
	}

	if (mAnalyticProcesses.empty())
	{
		try
		{
			sReply = analytic::xml::AnalyticMessage::getKillAllAnalyticProcessesReply(true,"Finished stopping all analytic instance");
			opencctv::util::log::Loggers::getDefaultLogger()->debug("Finished stopping all analytic instances");
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to create kill all analytics reply. ";
			sReply = analytic::xml::AnalyticMessage::getErrorReply(analytic::xml::OPERATION_KILL_ALL_ANALYTICS, true, sErrMsg);
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}
	}else
	{
		sErrMsg = "Failed to stop following analytic instances : ";
		sErrMsg.append(ssNotStoppedAnalytics.str());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		sReply = analytic::xml::AnalyticMessage::getErrorReply(analytic::xml::OPERATION_KILL_ALL_ANALYTICS, false, sErrMsg);
	}

	return sReply;
}

void AnalyticServerController::sendReply(const std::string& sMessage)
{
	if(_pSocket)
	{
		try
		{
			opencctv::mq::MqUtil::writeToSocket(_pSocket, sMessage);
		} catch (opencctv::Exception &e)
		{
			opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
		}
	}else
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Analytic Server Request-Reply Queue is not Initialized");
	}
}

std::string AnalyticServerController::reportError(const std::string& sOperation,
		const bool bDone, const std::string& sErrorMsg,const std::string& sExceptionMsg)
{
	std::string sMsg = sErrorMsg;
	sMsg = sMsg.append(". ");
	sMsg = sMsg.append(sExceptionMsg);
	opencctv::util::log::Loggers::getDefaultLogger()->error(sMsg);
	return analytic::xml::AnalyticMessage::getErrorReply(sOperation, bDone, sMsg);
}


AnalyticServerController::~AnalyticServerController()
{
	_pSocket->close();
	delete _pSocket; _pSocket = NULL;
}

} /* namespace analytic */

