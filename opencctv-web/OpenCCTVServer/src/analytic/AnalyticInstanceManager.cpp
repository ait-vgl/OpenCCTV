
#include "AnalyticInstanceManager.hpp"

namespace analytic {

AnalyticInstanceManager::AnalyticInstanceManager(const std::string& sAnalyticServerIp, const std::string& sAnalyticServerPort) {
	try
	{
		_pSocket = opencctv::mq::MqUtil::connectToMq(sAnalyticServerIp, sAnalyticServerPort, ZMQ_REQ);
	}
	catch(std::runtime_error &e)
	{
		std::string sErrMsg = "Failed to create ZMQ Socket to connect to Analytic Starter. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}

bool AnalyticInstanceManager::startAnalyticInstance(unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename, std::string& sAnalyticQueueInAddress, std::string& sAnalyticQueueOutAddress)
{
	bool bRet = false;
	if(_pSocket)
	{
		std::string sRequest, sReply;
		try {
			sRequest = xml::AnalyticMessage::getAnalyticStartRequest(
					iAnalyticInstanceId, sAnalyticPluginDirLocation,
					sAnalyticPluginFilename);
		} catch (opencctv::Exception &e) {
			std::string sErrMsg = "Failed to generate Analytic Start request. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}
		bool bRequestSent = false;
		try {
			bRequestSent = opencctv::mq::MqUtil::writeToSocket(_pSocket, sRequest);
		} catch (std::runtime_error &e) {
			std::string sErrMsg = "Failed to send Analytic Start request. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}
		if (bRequestSent) {
			try {
				opencctv::mq::MqUtil::readFromSocket(_pSocket, sReply);
			} catch (std::runtime_error &e) {
				std::string sErrMsg = "Failed to read Analytic Start reply. ";
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}
			if (!sReply.empty() && sReply.compare("Error") != 0) {
				try {
					analytic::xml::AnalyticMessage::extractAnalyticStartReplyData(
							sReply, sAnalyticQueueInAddress,
							sAnalyticQueueOutAddress);
					bRet = true;
				} catch (opencctv::Exception &e) {
					std::string sErrMsg =
							"Failed to extract data from Analytic Start reply. ";
					sErrMsg.append(e.what());
					throw opencctv::Exception(sErrMsg);
				}
			} else {
				throw opencctv::Exception("Error in Analytic Start reply.");
			}
		}
	}
	return bRet;
}

bool AnalyticInstanceManager::killAllAnalyticInstances()
{
	bool bDone = false;
	if(_pSocket)
	{
		std::string sMsg, sReply;
		try {
			sMsg = xml::AnalyticMessage::getKillAllAnalyticProcessesRequest();
		} catch (opencctv::Exception &e) {
			std::string sErrMsg = "Failed to generate xml message. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}
		bool bMsgSent = false;
		try {
			bMsgSent = opencctv::mq::MqUtil::writeToSocket(_pSocket, sMsg);
		} catch (std::runtime_error &e) {
			std::string sErrMsg = "Failed to send Kill All Analytic Instances Message. ";
			sErrMsg.append(e.what());
			throw opencctv::Exception(sErrMsg);
		}
		if (bMsgSent) {
			try {
				opencctv::mq::MqUtil::readFromSocket(_pSocket, sReply);
			} catch (std::runtime_error &e) {
				std::string sErrMsg = "Failed to read Kill all analytic processes reply. ";
				sErrMsg.append(e.what());
				throw opencctv::Exception(sErrMsg);
			}
			xml::AnalyticMessage::parseKillAllAnalyticProcessesReply(sReply, bDone);
		}
	}
	return bDone;
}

AnalyticInstanceManager::~AnalyticInstanceManager() {
	if(_pSocket)
	{
		_pSocket->close();
		delete _pSocket;
	}
}

} /* namespace analytic */
