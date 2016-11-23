#include "AnalyticServer.hpp"

namespace analytic
{

AnalyticServer::AnalyticServer(const int &analyticServerId, const std::string &sAnalyticServerIp, const std::string &sAnalyticServerPort)
{
	try
	{
		_iServerId = analyticServerId;
		_pSocket = opencctv::mq::MqUtil::connectToMq(sAnalyticServerIp, sAnalyticServerPort, ZMQ_REQ);
	}
	catch (std::runtime_error &e)
	{
		std::string sErrMsg = "Failed to create ZMQ Socket to connect to Analytic Starter. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}

bool AnalyticServer::startAllAnalyticInstanceAction()
{
	opencctv::util::Config *pConfig;
	opencctv::db::StreamGateway *pStreamGateway = NULL;

	try
	{
		pStreamGateway = new opencctv::db::StreamGateway();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create StreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	pConfig = opencctv::util::Config::getInstance();

	//size_t internalQueueSize = boost::lexical_cast<size_t>(pConfig->get(util::PROPERTY_INTERNAL_QUEUE_SIZE));
	//size_t remoteQueueSize = boost::lexical_cast<size_t>(pConfig->get(opencctv::util::PROPERTY_REMOTE_QUEUE_SIZE));

	opencctv::util::log::Loggers::getDefaultLogger()->info("Initializing variables done.");

	std::vector<opencctv::dto::Stream> vStreams;

	try
	{
		pStreamGateway->findAll(vStreams);
		opencctv::util::log::Loggers::getDefaultLogger()->info("Streams loaded.");
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to find all Streams. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	opencctv::util::log::Loggers::getDefaultLogger()->debug("Analytic instance number will be run: " + boost::lexical_cast<std::string>(vStreams.size()));

	//test::gateway::TestAnalyticInstanceStreamGateway analyticInstanceGateway;
	opencctv::db::AnalyticInstanceStreamGateway *pAnalyticInstanceGateway = NULL;

	try
	{
		pAnalyticInstanceGateway = new opencctv::db::AnalyticInstanceStreamGateway();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	for (size_t i = 0; i < vStreams.size(); ++i)
	{
		opencctv::dto::Stream stream = vStreams[i];

		//ImageMulticaster* pMulticaster = new ImageMulticaster(stream.getId());
		std::vector<opencctv::dto::AnalyticInstanceStream> vAnalyticInstances;

		/* Even use analyticInstanceStream, data inside is work like analytic instance
		 * , because AnalyticInstanceStream is used for multiple relationship Analytic instance and Stream
		 */
		try
		{
			// Get all analytic instance
			pAnalyticInstanceGateway->findAllForStream(stream.getId(), vAnalyticInstances);
			opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic Instances Streams loaded.");
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			return false;
		}

		// Starting Analytic Instances
		for (size_t j = 0; j < vAnalyticInstances.size(); ++j)
		{
			opencctv::dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];

			// Check duplicate analytic running instance
			//	if (!pModel->isAnalyticRunningByStreamId(vAnalyticInstances[j].getId(), vAnalyticInstances[j].getAnalyticInstanceId())) {

			// if the Analytic Instance has not been started yet
			AnalyticData *pAD = getAnalyticData(analyticInstance.getAnalyticInstanceId());

			if (!(pAD) or !pAD->isAnalyticQueueOutAddress()) // check dupplicate running, in case one analytic have multiple streams
			{
				bool bAIStarted = false;
				std::string sAnalyticQueueInAddress, sAnalyticQueueOutAddress;

				try
				{
					// start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
					bAIStarted = startAnalyticInstance(analyticInstance.getAnalyticInstanceId(), analyticInstance.getAnalyticDirLocation(), analyticInstance.getAnalyticFilename(), sAnalyticQueueInAddress, sAnalyticQueueOutAddress);

				}
				catch (opencctv::Exception &e)
				{
					std::stringstream ssErrMsg;
					ssErrMsg << "Failed to start Analytic Instance ";
					ssErrMsg << analyticInstance.getAnalyticInstanceId() << ". ";
					ssErrMsg << e.what();
					opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}

				if (bAIStarted)
				{
					// store analytic input queue address and output queue address in model

					AnalyticData *pAnalyticInstance = new AnalyticData(analyticInstance.getAnalyticInstanceId());

					opencctv::ResultRouterThread *pResultRouter = new opencctv::ResultRouterThread(pAnalyticInstance);
					boost::thread *pResultRouterThread = new boost::thread(*pResultRouter);

					if (pResultRouterThread->joinable())
					{
						//Loggers::getDefaultLogger()->debug("MainController: Starting pResulter");
						//Loggers::getDefaultLogger()->debug("MainController: update Analytic Data: " + boost::lexical_cast<std::string>(analyticInstance.getAnalyticInstanceId()));
						pAnalyticInstance->setResultRouterThread(pResultRouterThread);
						//pAnalyticInstance->setStatus(true);
						//pAnalyticData->setStreamId(stream.getId());

						//pResultRouterThread->join();
					}

					// Place to set up analytic id
					setAnalyticData(analyticInstance.getAnalyticInstanceId(), pAnalyticInstance);

					//pAD =  setAnalyticInstance(analyticInstance.getAnalyticInstanceId(), pAnalyticInstance);

					std::stringstream ssMsg;
					ssMsg << "Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssMsg << " started.";
					opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
				}
				else
				{
					std::stringstream ssErrMsg;
					ssErrMsg << "Starting Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssErrMsg << " failed.";
					opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}
			}
		}

		//opencctv::util::log::Loggers::getDefaultLogger()->info("Starting  All Analytic Instances done.");
	}

	return true;
}

bool AnalyticServer::startAnalyticInstanceAction(unsigned int iAnalyticInstanceId)
{

//test::gateway::TestStreamGateway streamGateway;
	opencctv::db::StreamGateway *pStreamGateway = NULL;

	try
	{
		pStreamGateway = new opencctv::db::StreamGateway();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create StreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	//opencctv::util::Config *pConfig = opencctv::util::Config::getInstance();

	//size_t internalQueueSize = boost::lexical_cast<size_t>(pConfig->get(util::PROPERTY_INTERNAL_QUEUE_SIZE));
	//size_t remoteQueueSize = boost::lexical_cast<size_t>(pConfig->get(util::PROPERTY_REMOTE_QUEUE_SIZE));

	opencctv::util::log::Loggers::getDefaultLogger()->info("Initializing variables done.");

	std::vector<opencctv::dto::Stream> vStreams;

	try
	{
		pStreamGateway->findAllByAnalyticInstanceId(vStreams, iAnalyticInstanceId);
		opencctv::util::log::Loggers::getDefaultLogger()->info("Streams loaded by Analytic instance id: " + boost::lexical_cast<std::string>(iAnalyticInstanceId));
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to find all Streams. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	opencctv::util::log::Loggers::getDefaultLogger()->debug("Analytic instance number will be run: " + boost::lexical_cast<std::string>(vStreams.size()));

	//test::gateway::TestAnalyticInstanceStreamGateway analyticInstanceGateway;
	opencctv::db::AnalyticInstanceStreamGateway *pAnalyticInstanceGateway = NULL;

	try
	{
		pAnalyticInstanceGateway = new opencctv::db::AnalyticInstanceStreamGateway();
	}
	catch (opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to create AnalyticInstanceStreamGateway -  ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return false;
	}

	for (size_t i = 0; i < vStreams.size(); ++i) // Many stream per an anaytic insance
	{
		opencctv::dto::Stream stream = vStreams[i];

		//ImageMulticaster* pMulticaster = new ImageMulticaster(stream.getId());
		std::vector<opencctv::dto::AnalyticInstanceStream> vAnalyticInstances;

		/* Even use analyticInstanceStream, data inside is work like analytic instance
		 * , because AnalyticInstanceStream is used for multiple relationship Analytic instance and Stream
		 */

		if (isAnalyticInstance(iAnalyticInstanceId))
		{
			// break for an analytic instance have multiple stream, we run only one time. This cannot apply in start all, because in statring all alaytic instace, we do not specific analytic instance id, so it may break incase one stream have multiple analytic
			break;
		}

		try
		{
			// Get all analytic instance
			pAnalyticInstanceGateway->findAllForStreamByAnalyticInstanceId(stream.getId(), vAnalyticInstances, iAnalyticInstanceId);
			opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic Instances Streams loaded.");
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to find all AnalyticInstanceStream. ";
			sErrMsg.append(e.what());
			opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			return false;
		}

		// Starting Analytic Instances
		for (size_t j = 0; j < vAnalyticInstances.size(); ++j) // one stream have many analytic instance, but in this case we use anlaytic id, so it will be only one analytic
		{
			opencctv::dto::AnalyticInstanceStream analyticInstance = vAnalyticInstances[j];

			// Check duplicate analytic running instance
			//	if (!pModel->isAnalyticRunningByStreamId(vAnalyticInstances[j].getId(), vAnalyticInstances[j].getAnalyticInstanceId())) {

			// if the Analytic Instance has not been started yet
			AnalyticData *pAD = getAnalyticData(analyticInstance.getAnalyticInstanceId());

			if (!(pAD) or !pAD->isAnalyticQueueOutAddress()) // check dupplicate running, in case one analytic have multiple streams
			{
				bool bAIStarted = false;
				std::string sAnalyticQueueInAddress, sAnalyticQueueOutAddress;

				try
				{
					// start Analytic Instance, store Analytic Input, Output queue addresses into the Application Model.
					bAIStarted = startAnalyticInstance(analyticInstance.getAnalyticInstanceId(), analyticInstance.getAnalyticDirLocation(), analyticInstance.getAnalyticFilename(), sAnalyticQueueInAddress, sAnalyticQueueOutAddress);

				}
				catch (opencctv::Exception &e)
				{
					std::stringstream ssErrMsg;
					ssErrMsg << "Failed to start Analytic Instance ";
					ssErrMsg << analyticInstance.getAnalyticInstanceId() << ". ";
					ssErrMsg << e.what();
					opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}

				if (bAIStarted)
				{
					// store analytic input queue address and output queue address in model

					AnalyticData *pAnalyticInstance = new AnalyticData(analyticInstance.getAnalyticInstanceId());

					opencctv::ResultRouterThread *pResultRouter = new opencctv::ResultRouterThread(pAnalyticInstance);
					boost::thread *pResultRouterThread = new boost::thread(*pResultRouter);

					if (pResultRouterThread->joinable())
					{
						pAnalyticInstance->setResultRouterThread(pResultRouterThread);
					}

					// Place to set up analytic id
					setAnalyticData(analyticInstance.getAnalyticInstanceId(), pAnalyticInstance);

					//pAD =  setAnalyticInstance(analyticInstance.getAnalyticInstanceId(), pAnalyticInstance);

					std::stringstream ssMsg;
					ssMsg << "Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssMsg << " started.";
					opencctv::util::log::Loggers::getDefaultLogger()->info(ssMsg.str());
				}
				else
				{
					std::stringstream ssErrMsg;
					ssErrMsg << "Starting Analytic Instance " << analyticInstance.getAnalyticInstanceId();
					ssErrMsg << " failed.";
					opencctv::util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}
			}
		}

		//opencctv::util::log::Loggers::getDefaultLogger()->info("Starting  All Analytic Instances done.");
	}

	return true;
}

bool AnalyticServer::startAnalyticInstance(unsigned int iAnalyticInstanceId, const std::string &sAnalyticPluginDirLocation, const std::string &sAnalyticPluginFilename, std::string &sAnalyticQueueInAddress, std::string &sAnalyticQueueOutAddress)
{
	bool bRet = false;

	if (_pSocket)
	{
		std::string sRequest, sReply;

		try
		{
			sRequest = xml::AnalyticMessage::getAnalyticStartRequest(iAnalyticInstanceId, sAnalyticPluginDirLocation, sAnalyticPluginFilename);
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to generate Analytic Start request. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		bool bRequestSent = false;

		try
		{
			bRequestSent = opencctv::mq::MqUtil::writeToSocket(_pSocket, sRequest);
		}
		catch (std::runtime_error &e)
		{
			std::string sErrMsg = "Failed to send Analytic Start request. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		if (bRequestSent)
		{
			try
			{
				opencctv::mq::MqUtil::readFromSocket(_pSocket, sReply);
			}
			catch (std::runtime_error &e)
			{
				std::string sErrMsg = "Failed to read Analytic Start reply. ";
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}

			if (!sReply.empty() && sReply.compare("Error") != 0)
			{
				try
				{
					analytic::xml::AnalyticMessage::extractAnalyticStartReplyData(sReply, sAnalyticQueueInAddress, sAnalyticQueueOutAddress);
					bRet = true;
				}
				catch (opencctv::Exception &e)
				{
					std::string sErrMsg = "Failed to extract data from Analytic Start reply. ";
					sErrMsg.append(e.what());
					throw opencctv::Exception(sErrMsg);
				}
			}
			else
			{
				throw opencctv::Exception("Error in Analytic Start reply.");
			}
		}
	}

	return bRet;
}

bool AnalyticServer::stopAnalyticInstanceAction(unsigned int iAnalyticInstanceId)
{
    
    return removeAnalyticData(iAnalyticInstanceId);
}

bool AnalyticServer::stopAllAnalyticInstanceAction()
{
	bool bDone = false;

	if (_pSocket)
	{
		std::string sMsg, sReply;

		try
		{
			sMsg = xml::AnalyticMessage::getKillAllAnalyticProcessesRequest();
		}
		catch (opencctv::Exception &e)
		{
			std::string sErrMsg = "Failed to generate xml message. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		bool bMsgSent = false;

		try
		{
			bMsgSent = opencctv::mq::MqUtil::writeToSocket(_pSocket, sMsg);
		}
		catch (std::runtime_error &e)
		{
			std::string sErrMsg = "Failed to send Kill All Analytic Instances Message. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}

		if (bMsgSent)
		{
			try
			{
				opencctv::mq::MqUtil::readFromSocket(_pSocket, sReply);
			}
			catch (std::runtime_error &e)
			{
				std::string sErrMsg = "Failed to read Kill all analytic processes reply. ";
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}

			xml::AnalyticMessage::parseKillAllAnalyticProcessesReply(sReply, bDone);
		}
	}

	return bDone;
}
/*
const std::map<unsigned int, AnalyticData *> &AnalyticServer::getAllAnalyticData()
{
	return _mAnalyticDatas;
}
*/

void AnalyticServer::setAnalyticData(unsigned int iAnalyticInstanceId, AnalyticData *&pAnalyticData)
{

	if (!isAnalyticInstance(iAnalyticInstanceId))
	{
		_mAnalyticDatas[iAnalyticInstanceId] = pAnalyticData; // bool for status, it may be full of analytic data class

		opencctv::util::log::Loggers::getDefaultLogger()->debug("AnalyticServer: Set up AnalyticData: " + boost::lexical_cast<std::string> (iAnalyticInstanceId));
	}
	else
	{
		opencctv::util::log::Loggers::getDefaultLogger()->debug("AnalyticServer: Skip set up AnalyticData: " + boost::lexical_cast<std::string> (iAnalyticInstanceId));
	}
}

AnalyticData *AnalyticServer::getAnalyticData(unsigned int iAnalyticInstanceId)
{

	if (isAnalyticInstance(iAnalyticInstanceId))
	{
		return _mAnalyticDatas[iAnalyticInstanceId];
	}
	else
		return NULL;
}

/*bool AnalyticServer::isMultipleStreamPerAnalytic(const unsigned int& iAnalyticInstanceId) {

 std::map<unsigned int, AnalyticInstance*>::iterator it = _mAnalyticInstances.find(iAnalyticInstanceId);
 if (it != _mAnalyticInstances.end()) {

 if (!(it->second->vStreamIds.empty()) && it->second->vStreamIds.size() > 1) {
 return true;
 }
 }
 return false;
 }*/

/*bool AnalyticServer::isImageInputQueueAddress(const int& iAnalyticInstanceId) {
	if (isAnalyticInstance(iAnalyticInstanceId)) {
		(_mAnalyticInstances[iAnalyticInstanceId]->sImageInputQueueAddress != "") ? true : false;
	} else
		return false;
}*/

bool AnalyticServer::isAnalyticInstance(int iAnalyticInstanceId)
{
	if (_mAnalyticDatas.count(iAnalyticInstanceId) > 0 && _mAnalyticDatas[iAnalyticInstanceId])
		return true;
	else
		return false;
}

bool AnalyticServer::removeAnalyticData(unsigned int iAnalyticInstanceId)
{
	if (isAnalyticInstance(iAnalyticInstanceId))
	{
		// Delete from pointer
		if (_mAnalyticDatas[iAnalyticInstanceId])
		{
			delete _mAnalyticDatas[iAnalyticInstanceId];

			//Delete from map
			_mAnalyticDatas.erase(_mAnalyticDatas.find(iAnalyticInstanceId));
			return true;
		}
	}
	else
	{
		return false;
	}
	
	return true;
}

AnalyticServer::~AnalyticServer()
{
	if (_pSocket)
	{
		_pSocket->close();
		delete _pSocket;
	}

}

} /* namespace analytic */
