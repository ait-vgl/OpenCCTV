
#include "opencctv/mq/MqUtil.hpp"
#include "analytic/xml/AnalyticMessage.hpp"
#include "opencctv/Exception.hpp"
#include "opencctv/util/log/Loggers.hpp"
#include "analytic/AnalyticProcess.hpp"
#include "analytic/util/Config.hpp"
#include <string>
#include <sstream>
#include <map>
#include "analytic/ApplicationModel.hpp"

using namespace opencctv;

int main()
{
	// Loading Configuration file
	analytic::util::Config *pConfig;

	try
	{
		pConfig = analytic::util::Config::getInstance();
	}
	catch (Exception &e)
	{
		std::string sErrMsg = "Failed to load Configuration file. ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	opencctv::util::log::Loggers::getDefaultLogger()->info("Loading Configuration file done.");

	// Initializing values
	std::string sStartingPort = pConfig->get(analytic::util::PROPERTY_STARTER_PORT);

	if (sStartingPort.empty())
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error("Failed to retrieve Analytic Starter port from Configuration file.");
		return -1;
	}

	const unsigned int STARTING_PORT = boost::lexical_cast<unsigned int>(sStartingPort);
	unsigned int iPort = STARTING_PORT;
	opencctv::util::log::Loggers::getDefaultLogger()->info("Initializing values done.");

	// Creating Analytic Server's request-reply MQ
	zmq::socket_t *pSocket = NULL;

	try
	{
		pSocket = mq::MqUtil::createNewMq(sStartingPort, ZMQ_REP);
	}
	catch (std::runtime_error &e)
	{
		std::string sErrMsg = "Failed to create Analytic Server's request-reply MQ. ";
		sErrMsg.append(e.what());
		util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	opencctv::util::log::Loggers::getDefaultLogger()->info("Creating Analytic Server's request-reply MQ done.");

	// Starting listening for requests
	opencctv::util::log::Loggers::getDefaultLogger()->info("Listening for requests started.");

	while (pSocket)
	{
		std::string sRequest;
		std::string sReply = "Error";

		try
		{
			mq::MqUtil::readFromSocket(pSocket, sRequest);
		}
		catch (std::runtime_error &e)
		{
			std::string sErrMsg = "Failed to read request from the request-reply MQ. ";
			sErrMsg.append(e.what());
			util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}

		std::string sOperation;

		try
		{
			sOperation = analytic::xml::AnalyticMessage::extractAnalyticRequestOperation(sRequest);
		}
		catch (Exception &e)
		{
			std::string sErrMsg = "Failed to extract the operation from XML request. ";
			sErrMsg.append(e.what());
			util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}

		// Analytic Start request
		if (sOperation.compare(analytic::xml::OPERATION_START_ANALYTIC) == 0)
		{
			// Extracting request data
			unsigned int iAnalyticInstanceId;
			std::string sAnalyticDirPath;
			std::string sAnalyticFilename;

			try
			{
				analytic::xml::AnalyticMessage::extractAnalyticStartRequestData(sRequest, iAnalyticInstanceId, sAnalyticDirPath, sAnalyticFilename);
			}
			catch (Exception &e)
			{
				std::string sErrMsg = "Failed to extract data from Analytic Start request. ";
				sErrMsg.append(e.what());
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}

			// Creating Analytic process
			if (iAnalyticInstanceId > 0 && !sAnalyticDirPath.empty() && !sAnalyticFilename.empty())
			{
				bool bAIStarted = false;
				analytic::AnalyticProcess *pAnalyticProcess = new analytic::AnalyticProcess();
				std::string sAnalyticInputQueueAddress = boost::lexical_cast<std::string>(++iPort);
				std::string sAnalyticOutputQueueAddress = boost::lexical_cast<std::string>(++iPort);

				try
				{
					std::string sAnalyticRunnerPath;
					sAnalyticRunnerPath.append(pConfig->get(analytic::util::PROPERTY_ANALYTIC_RUNNER_DIR));
					sAnalyticRunnerPath.append("/");
					sAnalyticRunnerPath.append(pConfig->get(analytic::util::PROPERTY_ANALYTIC_RUNNER_FILENAME));

					std::cout << "From Starter: Instance id " + boost::lexical_cast<std::string>(iAnalyticInstanceId) + " plugin location" + sAnalyticDirPath + " plugin file anem" + sAnalyticFilename + " Input Queue port" + sAnalyticInputQueueAddress + " Output queue port" + sAnalyticOutputQueueAddress << std::endl;

					bAIStarted = pAnalyticProcess->startAnalytic(sAnalyticRunnerPath, iAnalyticInstanceId, sAnalyticDirPath, sAnalyticFilename, sAnalyticInputQueueAddress, sAnalyticOutputQueueAddress);
				}
				catch (opencctv::Exception &e)
				{
					std::stringstream ssErrMsg;
					ssErrMsg << "Failed to start Analytic Instance " << iAnalyticInstanceId << ". ";
					ssErrMsg << e.what();
					util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}

				if (bAIStarted)
				{
					analytic::ApplicationModel::getInstance()->getAnalyticProcesses()[iAnalyticInstanceId] = pAnalyticProcess;
					std::stringstream ssMsg;
					ssMsg << "Analytic Instance " << iAnalyticInstanceId << " started. ";
					ssMsg << "Input queue at: " << sAnalyticInputQueueAddress << ", ";
					ssMsg << "Output queue at: " << sAnalyticOutputQueueAddress;
					util::log::Loggers::getDefaultLogger()->info(ssMsg.str());

					try
					{
						sReply = analytic::xml::AnalyticMessage::getAnalyticStartReply(sAnalyticInputQueueAddress, sAnalyticOutputQueueAddress);
					}
					catch (Exception &e)
					{
						std::string sErrMsg = "Failed to create Analytic Start Reply XML message. ";
						sErrMsg.append(e.what());
						util::log::Loggers::getDefaultLogger()->error(sErrMsg);
					}
				}
				else
				{
					std::stringstream ssErrMsg;
					ssErrMsg << "Failed to start Analytic Instance from bAIStarted " << iAnalyticInstanceId << ". ";
					util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				}

				// Sending Analytic Start reply
				bool bASRSent = false;

				try
				{
					bASRSent = opencctv::mq::MqUtil::writeToSocket(pSocket, sReply);
				}
				catch (std::runtime_error &e)
				{
					std::string sErrMsg = "Failed to send Analytic Start Reply. ";
					sErrMsg.append(e.what());
					util::log::Loggers::getDefaultLogger()->error(sErrMsg);
				}

				if (!bASRSent)
				{
					util::log::Loggers::getDefaultLogger()->error("Failed to send Analytic Start Reply.");
				}
			}
			else
			{
				std::string sErrMsg = "Request with invalid data.\nRequest: ";
				sErrMsg.append(sRequest);
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}
		}
		// Stop an analytic instance
		else if (sOperation.compare(analytic::xml::OPERATION_STOP_ANALYTIC) == 0)
		{
			// Extracting request data
			unsigned int iAnalyticInstanceId;

			try
			{
                // Use same as startRequest message
				analytic::xml::AnalyticMessage::extractAnalyticStopRequestData(sRequest, iAnalyticInstanceId);
			}
			catch (Exception &e)
			{
				std::string sErrMsg = "Failed to extract data from Analytic Stop request. ";
				sErrMsg.append(e.what());
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}

			// Creating Analytic process
			bool bDone = false;

			std::map<unsigned int, analytic::AnalyticProcess *> mAnalyticProcesses = analytic::ApplicationModel::getInstance()->getAnalyticProcesses();
			std::map<unsigned int, analytic::AnalyticProcess *>::iterator it =  mAnalyticProcesses.find(iAnalyticInstanceId);

			if( it != mAnalyticProcesses.end())
			{
				analytic::AnalyticProcess *pAnalyticProcess = it->second;

				if (it->first == iAnalyticInstanceId && pAnalyticProcess->stop())
				{
					if (pAnalyticProcess)
					{
						delete pAnalyticProcess;
						pAnalyticProcess = NULL;
					}

					mAnalyticProcesses.erase(it++); //remove analytic process from the model
                    bDone = true; // stopped the analytic
                    break;
				}
			}

			// Sending Kill all analytic processes reply
			try
			{
				//TODO: may update this message
				sReply = analytic::xml::AnalyticMessage::getKillAllAnalyticProcessesReply(bDone);
			}
			catch (Exception &e)
			{
				std::string sErrMsg = "Failed to create Kill All Analytics Reply. ";
				sErrMsg.append(e.what());
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}

			bool bASRSent = false;

			try
			{
				bASRSent = opencctv::mq::MqUtil::writeToSocket(pSocket, sReply);
			}
			catch (std::runtime_error &e)
			{
				std::string sErrMsg = "Failed to send stop an Analytic Processes Reply. ";
				sErrMsg.append(e.what());
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}

			if (!bASRSent)
			{
				util::log::Loggers::getDefaultLogger()->error("Failed to send stop an Analytic Processes Reply.");
			}

			if (bDone)
			{
				//iPort = STARTING_PORT; // reset port
				std::stringstream ssErrMsg;
				ssErrMsg << "stop the Analytic processes: " << iAnalyticInstanceId << " .";
				util::log::Loggers::getDefaultLogger()->info(ssErrMsg.str());
			}
			else
			{
				std::stringstream ssErrMsg;
				ssErrMsg << "Failed to stop the Analytic processes: " << iAnalyticInstanceId << " .";
				util::log::Loggers::getDefaultLogger()->error(ssErrMsg.str());
				//return -1; // exit
			}
		}
		// Kill all analytic processes message
		else if (sOperation.compare(analytic::xml::OPERATION_KILL_ALL_ANALYTICS) == 0)
		{
			bool bDone = false;
			std::map<unsigned int, analytic::AnalyticProcess *> mAnalyticProcesses = analytic::ApplicationModel::getInstance()->getAnalyticProcesses();
			std::map<unsigned int, analytic::AnalyticProcess *>::iterator it;

			for (it = mAnalyticProcesses.begin(); it != mAnalyticProcesses.end(); /*it increment below*/)
			{
				analytic::AnalyticProcess *pAnalyticProcess = it->second;

				if (pAnalyticProcess->stop())
				{
					if (pAnalyticProcess)
					{
						delete pAnalyticProcess;
						pAnalyticProcess = NULL;
					}

					mAnalyticProcesses.erase(it++); //remove analytic process from the model
				}
				else
				{
					++it;
				}
			}

			if (mAnalyticProcesses.empty())
			{
				bDone = true; // killed all analytics
			}

			// Sending Kill all analytic processes reply
			try
			{
				sReply = analytic::xml::AnalyticMessage::getKillAllAnalyticProcessesReply(bDone);
			}
			catch (Exception &e)
			{
				std::string sErrMsg = "Failed to create Kill All Analytics Reply. ";
				sErrMsg.append(e.what());
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}

			bool bASRSent = false;

			try
			{
				bASRSent = opencctv::mq::MqUtil::writeToSocket(pSocket, sReply);
			}
			catch (std::runtime_error &e)
			{
				std::string sErrMsg = "Failed to send Kill All Analytic Processes Reply. ";
				sErrMsg.append(e.what());
				util::log::Loggers::getDefaultLogger()->error(sErrMsg);
			}

			if (!bASRSent)
			{
				util::log::Loggers::getDefaultLogger()->error("Failed to send Kill All Analytic Processes Reply.");
			}

			if (bDone)
			{
				iPort = STARTING_PORT; // reset port
				util::log::Loggers::getDefaultLogger()->info("Killed all the Analytic processes.");
			}
			else
			{
				util::log::Loggers::getDefaultLogger()->error("Failed to kill all the Analytic processes.");
				//return -1; // exit
			}
		}
		else
		{
			std::string sErrMsg = "Request with an unknown Operation.\nRequest: ";
			sErrMsg.append(sRequest);
			util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}
	}

	if (pSocket) delete pSocket;

	return 0;
}
