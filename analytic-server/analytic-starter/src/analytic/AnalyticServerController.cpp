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
	_iPid = getpid();
	_iServerId = 0;
	_sHost = "";
	_iPort = 0;
	try
	{
		_pConfig = analytic::util::Config::getInstance();
	}catch(opencctv::Exception& e)
	{
		throw e;
	}

	opencctv::util::log::Loggers::getDefaultLogger()->info("Loading configuration file done.");

	// Initializing values
	//Read the server's ip/host address
	_sHost = _pConfig->get(analytic::util::PROPERTY_STARTER_HOST);
	boost::algorithm::trim(_sHost);
	if(_sHost.empty())
	{
		throw opencctv::Exception("Failed to retrieve analytic server host address from configuration file.");
	}

	//Read the starting port
	std::string sStartingPort =_pConfig->get(analytic::util::PROPERTY_STARTER_PORT);
	boost::trim(sStartingPort);
	if(sStartingPort.empty())
	{
		throw opencctv::Exception("Failed to retrieve analytic server port from configuration file.");
	}

	try
	{
		_iPort = boost::lexical_cast<unsigned int>(sStartingPort);
	}
	catch (const boost::bad_lexical_cast &e)
	{
		throw opencctv::Exception("Failed to retrieve analytic server port from configuration file.");
	}

	std::stringstream ssMsg;
	ssMsg << "Analytic server details - Host: " << _sHost;
	ssMsg << " Port: " << _iPort;
	opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());

	//Read the number of analytics
	//std::string sNoAnalytics = _pConfig->get(analytic::util::PROPERTY_NUM_OF_ANALYTICS);
	//if(sNoAnalytics.empty())
	//{
		//throw opencctv::Exception("Failed to retrieve Number of Analytics from configuration file.");
	//}

	//_iNumOfAnalytics = boost::lexical_cast<unsigned int>(sNoAnalytics);

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
	std::string sOperation = "";
	unsigned int iServerId = 0;
	std::string sMessage;

	//Read the request
	try
	{
		opencctv::mq::MqUtil::readFromSocket(_pSocket, sRequest);
		std::cout << "sRequest : " << sRequest << "\n" << std::endl;
		//sOperation = analytic::xml::AnalyticMessage::extractAnalyticRequestOperation(sRequest);
		analytic::xml::AnalyticMessage::extractInitialDetails(sRequest, iServerId, sOperation);
		//std::cout << "sReply : " << sReply << "\n" << std::endl;
	}
	catch(opencctv::Exception &e)
	{
		sOperation = analytic::xml::OPERATION_UNKNOWN;
		sMessage = e.what();
	}

	//Update the server id
	if(iServerId >0)
	{
		_iServerId = iServerId;
	}

	//If Initial details are invalid reply with an error message
	//Otherwise, execute the operation according to the operation type
	std::string sReply;
	if(iServerId == 0)
	{
		sReply = reportError("Request with invalid analytic server Id.");
	}else if(sOperation.empty())
	{
		sReply = reportError("Request with an unknown operation.");
	}
	else if(sOperation.compare(analytic::xml::OPERATION_START_ANALYTIC) == 0)
	{
		sReply = startAnalytic(sRequest);
	}
	else if(sOperation.compare(analytic::xml::OPERATION_STOP_ANALYTIC) == 0)
	{
		sReply = stopAnalytic(sRequest);
	}
	else if(sOperation.compare(analytic::xml::OPERATION_KILL_ALL_ANALYTICS) == 0)
	{
		sReply = killAllAnalytics();
	}
	else if(sOperation.compare(analytic::xml::OPERATION_ANALYTIC_SERVER_STATUS) == 0)
	{
		sReply = getServerStatus();
	}else if(sOperation.compare(analytic::xml::OPERATION_ANALYTIC_INST_STATUS) == 0)
	{
		sReply = getAnalyticInstStatus();
	}
	else
	{
		sReply = reportError("Request with an unknown operation.");
	}
	std::cout << "sReply : " << sReply << "\n" << std::endl;
	sendReply(sReply);
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
	std::string sAnalyticInputQueueAddress = "in";
	std::string sAnalyticOutputQueueAddress = "out";

	// TODO Check maximum num. of analytic instances???

	// TODO The analytic start request XML message has to be updated

	//Step 0 - Check for config details
	if(!_pConfig)
	{
		return reportError("Analytic-starter module not properly configured");
	}

	//Step 1 - Extract data from the request message
	try
	{
		analytic::xml::AnalyticMessage::extractAnalyticStartRequestData(sRequest, iAnalyticInstanceId,sAnalyticFilename);
	}
	catch (opencctv::Exception &e)
	{
		return reportError("Failed to extract data from analytic start request");
	}

	//Step 2 - Set the analytic results path
	std::string sAnalyticResultsDir = "";
	sAnalyticResultsDir = _pConfig->get(analytic::util::PROPERTY_ANALYTIC_RESULTS_DIR);
	if(sAnalyticResultsDir.empty())
	{
		return reportError("Failed to locate the path to save analytic results");
	}
	if (*sAnalyticResultsDir.rbegin() != '/')     // check last char
	{
		sAnalyticResultsDir.append("/");
	}
	std::ostringstream sResultsDir;
	sResultsDir << sAnalyticResultsDir << iAnalyticInstanceId;
	sAnalyticResultsDir = sResultsDir.str();

	if(!opencctv::util::Util::createDir(sAnalyticResultsDir))
	{
		return reportError("Failed to create a directory to save analytic results.");
	}


	//Step 3 - Start the analytic instance

	// TODO Check if a process already exist with the same analytic instance id
	//If so stop the analytic instance and start the analytic instance again

	// TODO Code not modified. Need updating to use the current changes.

	// Creating Analytic process
	analytic::AnalyticProcess *pAnalyticProcess = new analytic::AnalyticProcess(iAnalyticInstanceId);
	if(!pAnalyticProcess)
	{
		return reportError("Out of memory - failed to create new analytic instance");
	}

	try
	{
		std::string sAnalyticRunnerPath;
		sAnalyticRunnerPath.append(_pConfig->get(analytic::util::PROPERTY_ANALYTIC_RUNNER_DIR));
		if (*sAnalyticRunnerPath.rbegin() != '/')     // check last char of path
		{
			sAnalyticRunnerPath.append("/");
		}
		sAnalyticRunnerPath.append(_pConfig->get(analytic::util::PROPERTY_ANALYTIC_RUNNER_FILENAME));
		//std::cout << "From Starter: Instance id " + boost::lexical_cast<std::string>(iAnalyticInstanceId) + " plugin location" + sAnalyticDirPath + " plugin file anem" + sAnalyticFilename + " Input Queue port" + sAnalyticInputQueueAddress + " Output queue port" + sAnalyticOutputQueueAddress << std::endl;
		bAIStarted = pAnalyticProcess->startAnalytic(sAnalyticRunnerPath, sAnalyticFilename, sAnalyticResultsDir);
	}catch(opencctv::Exception &e)
	{
		sErrMsg = e.what();
		bAIStarted = false;
	}

	if(!bAIStarted)
	{
		return reportError("Failed to start analytic instance.");
	}

	//Step 4 - Record the details of the analytic process in ApplicationModel and
	//         update the database
	analytic::ApplicationModel* pModel = analytic::ApplicationModel::getInstance();
	pModel->getAnalyticProcesses()[iAnalyticInstanceId] = pAnalyticProcess;

	std::string sOutputMsg = "";
	try
	{
		result::db::AnalyticInstanceGateway analyticInstanceGateway;
		analyticInstanceGateway.updateStatus(iAnalyticInstanceId, analytic::ANALYTIC_STATUS_STARTED);
	}
	catch(opencctv::Exception &e)
	{
		sOutputMsg = "Failed to update the status of analytic instances in the database.";
		//return reportError("Database error. Failed to update the status of analytic instances.");
	}

	if (!sOutputMsg.empty()){ ssErrMsg << sOutputMsg; }

	//Step 5 - Start the results routing threads for each results app instance
	//         registered by this analytic instance
	//result::AnalyticInstController analyticInstController;

	//analyticInstController.startResultsRouting(iAnalyticInstanceId,sOutputMsg);
	//if (!sOutputMsg.empty()){ ssErrMsg << " " << sOutputMsg; }

	//Step 6 - Return the reply XML message
	std::stringstream ssMsg;
	ssMsg << "Analytic instance " << iAnalyticInstanceId << " started.";
	opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
	if(!(ssErrMsg.str().empty()))
	{
		ssMsg << " But, few errors occurred while starting the analytic instance. ";
		ssMsg << "Errors : " << ssErrMsg.str() << " Check the analytic server log for more details.";
	}

	try
	{
		sReply = analytic::xml::AnalyticMessage::getAnalyticStartReply(ssMsg.str(), _sStatus, _iPid);
	}
	catch (opencctv::Exception &e)
	{
		sReply = reportError("Failed to create analytic start reply XML message");
	}

	return sReply;
}

std::string AnalyticServerController::stopAnalytic(const std::string& sRequest)
{
	// Request data
	unsigned int iAnalyticInstanceId = 0;

	//Reply data
	std::string sReply;
	bool bDone = false;
	std::string sErrMsg;

	// Extracting request data
	try
	{
		analytic::xml::AnalyticMessage::extractAnalyticStopRequestData(sRequest, iAnalyticInstanceId);
		if(iAnalyticInstanceId == 0)
		{
			throw opencctv::Exception("Invalid analytic instance Id.");
		}
	}
	catch (opencctv::Exception &e)
	{
		sErrMsg = "Failed to extract data from analytic stop request. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		sReply = analytic::xml::AnalyticMessage::getErrorReply(sErrMsg, _sStatus, _iPid);
		return sReply;
	}

	//Find the process for the requested analytic instance
	std::map<unsigned int, analytic::AnalyticProcess *> mAnalyticProcesses = analytic::ApplicationModel::getInstance()->getAnalyticProcesses();
	std::map<unsigned int, analytic::AnalyticProcess *>::iterator it =  mAnalyticProcesses.find(iAnalyticInstanceId);

	if(it == mAnalyticProcesses.end())
	{
		std::stringstream ssMsg;
		ssMsg << "Failed to stop the analytic instance. Cannot find analytic instance with ID : ";
		ssMsg << iAnalyticInstanceId << ".";
		return analytic::xml::AnalyticMessage::getErrorReply(ssMsg.str(), _sStatus, _iPid);
		return sReply;
	}

	// Stopping the analytic process
	analytic::AnalyticProcess *pAnalyticProcess = it->second;
	bDone = pAnalyticProcess->stopAnalytic();

	if(!bDone)
	{
		std::stringstream ssMsg;
		ssMsg << "Error occurred while stopping the analytic instance " << iAnalyticInstanceId << ".";
		return analytic::xml::AnalyticMessage::getErrorReply(ssMsg.str(), _sStatus, _iPid);
		return sReply;
	}

	//Free the memory taken by the analytic process
	if (pAnalyticProcess)
	{
		delete pAnalyticProcess; pAnalyticProcess = NULL;
	}
	//Remove the analytic process from the ApplicationModel
	mAnalyticProcesses.erase(it);


	//Update the database status
	std::string sOutputMsg = "";
	try
	{
		result::db::AnalyticInstanceGateway analyticInstanceGateway;
		analyticInstanceGateway.updateStatus(iAnalyticInstanceId, analytic::ANALYTIC_STATUS_STOPPED);
	}
	catch(opencctv::Exception &e)
	{
		sOutputMsg = "Failed to update the status of analytic instances in the database.";
	}

	// TODO Remove results routing threads when all results are transmitted
	//pAnalyticProcess->setIsActive(false);

	//Return the reply XML message
	try
	{
		std::stringstream ssMsg;
		ssMsg << "Analytic instance " << iAnalyticInstanceId << " stopped.";
		if(!sOutputMsg.empty())
		{
			ssMsg << " But, few errors occurred while stopping the analytic instance. ";
			ssMsg << "Errors : " << sOutputMsg << " Check the analytic server log for more details.";
		}
		sReply = analytic::xml::AnalyticMessage::getAnalyticStopReply(ssMsg.str(), _sStatus, _iPid);
		opencctv::util::log::Loggers::getDefaultLogger()->debug("Finished stopping analytic instance, replied");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "XML parser error. Failed to create analytic stop reply. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return analytic::xml::AnalyticMessage::getErrorReply(sErrMsg, _sStatus, _iPid);
	}

	return sReply;

}

/*std::string AnalyticServerController::killAllAnalytics(const std::string& sRequest)
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
			//Remove the analytic process from the ApplicationModel
			mAnalyticProcesses.erase(it);

			//Free the memory taken by the analytic process
			if (pAnalyticProcess)
			{
				delete pAnalyticProcess; pAnalyticProcess = NULL;
			}

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
			//sReply = analytic::xml::AnalyticMessage::getErrorReply(analytic::xml::OPERATION_KILL_ALL_ANALYTICS, true, sErrMsg);
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}
	}else
	{
		sErrMsg = "Failed to stop following analytic instances : ";
		sErrMsg.append(ssNotStoppedAnalytics.str());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		//sReply = analytic::xml::AnalyticMessage::getErrorReply(analytic::xml::OPERATION_KILL_ALL_ANALYTICS, false, sErrMsg);
	}

	return sReply;
}*/

std::string AnalyticServerController::killAllAnalytics()
{
	std::string sReply;
	std::stringstream ssErrMsg;
	std::stringstream ssNotStoppedAnalytics;
	std::stringstream ssNotStatusUpdatedAnalytics;
	std::string sSeparator1 = "";
	std::string sSeparator2 = "";
	unsigned int iAnalyticInstId = 0;

	std::map<unsigned int, analytic::AnalyticProcess *> mAnalyticProcesses = analytic::ApplicationModel::getInstance()->getAnalyticProcesses();
	std::map<unsigned int, analytic::AnalyticProcess *>::iterator it;

	for (it = mAnalyticProcesses.begin(); it != mAnalyticProcesses.end(); /*++it*/)//Iterator incremented below
	{
		std::map<unsigned int, analytic::AnalyticProcess *>::iterator itErase = it;
		++it;

		analytic::AnalyticProcess *pAnalyticProcess = itErase->second;

		if(!pAnalyticProcess)//If analytic process is null skip the loop
		{
			continue;
		}

		iAnalyticInstId = pAnalyticProcess ->getAnalyticInstanceId();

		if (pAnalyticProcess->stopAnalytic())
		{
			delete pAnalyticProcess; pAnalyticProcess = NULL;
			//Remove the analytic process from the ApplicationModel
			mAnalyticProcesses.erase(itErase);

			//Update the database status
			try
			{
				result::db::AnalyticInstanceGateway analyticInstanceGateway;
				analyticInstanceGateway.updateStatus(iAnalyticInstId, analytic::ANALYTIC_STATUS_STOPPED);
			}
			catch(opencctv::Exception &e)
			{

				ssNotStatusUpdatedAnalytics << sSeparator2 << iAnalyticInstId;
				sSeparator2 = ", ";
			}

			// TODO Remove results routing threads when all results are transmitted
		}
		else
		{
			ssNotStoppedAnalytics << sSeparator1 << it->first;
			sSeparator1 = ", ";
		}
	}

	//Return the reply XML message
	std::string sMsg = "";

	if(!(ssNotStoppedAnalytics.str().empty()))
	{
		ssErrMsg << "Failed to stop following analytic instances properly: ";
		ssErrMsg << ssNotStoppedAnalytics.str() << ".";

	}

	if(!(ssNotStatusUpdatedAnalytics.str().empty()))
	{
		ssErrMsg << " Failed to update the status of following analytic instances in the database : ";
		ssErrMsg << ssNotStatusUpdatedAnalytics.str() << ".";

	}

	if(!(ssErrMsg.str().empty()))
	{
		sMsg = "Few errors occurred while stopping analytic instances. Errors : ";
		sMsg.append(ssErrMsg.str());
	}else
	{
		sMsg = "All analytic instances successfully stopped.";
	}

	//opencctv::util::log::Loggers::getDefaultLogger()->debug(sMsg);

	try
	{
		sReply = analytic::xml::AnalyticMessage::getKillAllAnalyticsReply(sMsg, _sStatus, _iPid);

	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "XML parser error. Failed to create kill all analytics reply. ";
		sErrMsg.append(e.what());
		return analytic::xml::AnalyticMessage::getErrorReply(sErrMsg, _sStatus, _iPid);
	}

	return sReply;
}

std::string AnalyticServerController::AnalyticServerController::getServerStatus()
{
	std::string sReply;
	std::string sMessage = "Analytic server status retrieved successfully.";

	//Return the reply XML message
	try
	{
		sReply = analytic::xml::AnalyticMessage::getServerStatusReply(sMessage,_sStatus,_iPid);
	}
	catch (opencctv::Exception &e)
	{
		std::stringstream ossReply;
		ossReply << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
		ossReply << "<analyticreply>";
		ossReply << "<type>";
		ossReply <<	analytic::xml::OPERATION_ANALYTIC_SERVER_STATUS;
		ossReply << "</type>";
		ossReply << "<content>";
		ossReply <<	sMessage;
		ossReply << "</content>";
		ossReply << "<serverstatus>";
		ossReply <<	_sStatus;
		ossReply << "</serverstatus>";
		ossReply << "<serverpid>";
		ossReply <<	_iPid;
		ossReply << "</serverpid>";
		ossReply << "</analyticreply>";

		sReply = ossReply.str();
	}

	return sReply;
}

std::string AnalyticServerController::getAnalyticInstStatus()
{
	std::string sReply = "";
	std::stringstream ssFailedAnalytics;
	ssFailedAnalytics << "";
	const char* cSeparator = "";
	/*Retrieve the list of analytic instances assigned to this analytic server*/
	std::vector<unsigned int > vAnlyticInstIds;
	try
	{
		result::db::AnalyticServerGateway analyticServerGateway;
		analyticServerGateway.findAllAnalyticInstances(_iServerId, vAnlyticInstIds);
	}catch(opencctv::Exception &e)
	{
		return reportError("Failed to retrieve analytic instance details.");
	}

	if(vAnlyticInstIds.empty())
	{
		return reportError("No analytic instances are assigned to this analytic server.");
	}

	try
	{
		result::db::AnalyticInstanceGateway analyticInstanceGateway;
		analytic::ApplicationModel* pModel = analytic::ApplicationModel::getInstance();
		std::map<unsigned int, analytic::AnalyticProcess*> mAnalyticProcesses = pModel->getAnalyticProcesses();
		std::map<unsigned int, analytic::AnalyticProcess*> ::iterator itInst;
		std::vector<unsigned int>::iterator itDBInst;
		unsigned int iInstId = 0;
		unsigned int iInstStatus = 999;



		for (itDBInst = vAnlyticInstIds.begin(); itDBInst != vAnlyticInstIds.end(); ++itDBInst)
		{
			int iInstId = *itDBInst;
			itInst = mAnalyticProcesses.find(iInstId);

			if(itInst == mAnalyticProcesses.end())// The analytic is not currently running on this server
			{
				//Update the status to "stopped"
				iInstStatus = 0;
			}else// The analytic is currently running on this server
			{
				//Check the status of the AnalyticProcess object
				iInstStatus = itInst->second->getAnalyticStatus();
			}

			//Update the DB
			try
			{
				analyticInstanceGateway.updateStatus(iInstId, iInstStatus);
			}catch(opencctv::Exception &e)
			{
				ssFailedAnalytics << cSeparator << iInstId;
			}

			cSeparator = ", ";
		}
	}catch(opencctv::Exception &e)
	{
		return reportError("Database error. Failed to update the status of analytic instances.");
	}

	std::string sMessage = "";

	if (ssFailedAnalytics.str().empty())
	{
		sMessage = "Status of analytic instances retrieved successfully.";
	}else
	{
		sMessage = "Errors occurred while updating the status of analytic instance(s) " + ssFailedAnalytics.str() + ".";
	}

	//Return the reply XML message
	try
	{
		sReply = analytic::xml::AnalyticMessage::getServerStatusReply(sMessage,_sStatus,_iPid);
	}
	catch (opencctv::Exception &e)
	{
		std::stringstream ossReply;
		ossReply << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
		ossReply << "<analyticreply>";
		ossReply << "<type>";
		ossReply <<	analytic::xml::OPERATION_ANALYTIC_INST_STATUS;
		ossReply << "</type>";
		ossReply << "<content>";
		ossReply <<	sMessage;
		ossReply << "</content>";
		ossReply << "<serverstatus>";
		ossReply <<	_sStatus;
		ossReply << "</serverstatus>";
		ossReply << "<serverpid>";
		ossReply <<	_iPid;
		ossReply << "</serverpid>";
		ossReply << "</analyticreply>";

		sReply = ossReply.str();
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

std::string AnalyticServerController::reportError(const std::string& sErrorMsg)
{
	opencctv::util::log::Loggers::getDefaultLogger()->error(sErrorMsg);
	return analytic::xml::AnalyticMessage::getErrorReply(sErrorMsg, _sStatus, _iPid);
}

const std::string& AnalyticServerController::getStatus() const {
	return _sStatus;
}

void AnalyticServerController::updateStatus(const std::string& sStatus) {
	_sStatus = sStatus;
	int iResult = 0;
	try
	{
		result::db::AnalyticServerGateway analyticServerGateway;
		//iResult = analyticServerGateway.updateAnalyticServerStatus(_sHost, _iPort, analytic::ANALYTIC_SERVER_STOPPED);
		iResult = analyticServerGateway.updateAnalyticServerStatus(_sHost, _iPort, sStatus);
	}
	catch(opencctv::Exception &e)
	{

		opencctv::util::log::Loggers::getDefaultLogger()->error(e.what());
	}

	if(iResult < 1)
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Faild to update the analytic server status in the database." );
	}
}

void AnalyticServerController::stopServer()
{
	_sStatus = "Stopped";
	std::string sReply;

	//Stop all the analytics
	sReply = killAllAnalytics(); //This will update the analytic status in DB
	std::string sReplyContent = "";
	analytic::xml::AnalyticMessage::extractKillAllAnalyticsReply(sReply, sReplyContent);
	opencctv::util::log::Loggers::getDefaultLogger()->debug(sReplyContent);

	//Update the server status in the database.
	updateStatus(analytic::ANALYTIC_SERVER_STOPPED);
}

AnalyticServerController::~AnalyticServerController()
{
	_pSocket->close();
	delete _pSocket; _pSocket = NULL;
}

} /* namespace analytic */

