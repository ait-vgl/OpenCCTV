
#include "MilestoneVmsConnector.hpp"

MilestoneVmsConnector::MilestoneVmsConnector() : opencctv::api::VmsConnector() {
	_pDecoder = NULL;
}

bool MilestoneVmsConnector::init(const opencctv::api::VmsConnectInfo& info, const std::string sPathToAnalyticDir)
{
	TCPConnector* connector = new TCPConnector();
	TCPStream* stream = connector->connect(info.sServerIp.c_str(), info.iServerPort);
	_pDecoder = new TcpMpegDecoder(stream);
	string connectMsg = MilestoneMessage::getConnectMessage(0, info.sUsername,
				info.sPassword, info.sCameraId, info.iWidth, info.iHeight,
				info.bKeepAspectRatio, info.bAllowUpsizing);
	stream->send(connectMsg.c_str(), connectMsg.size());
	char line[2048];
	int len = stream->receive(line, sizeof(line));
	line[len] = '\0';
	string connectResponse(line);
	if (MilestoneMessage::isConnectionSuccessful(connectResponse)) {
		string liveMsg = MilestoneMessage::getLiveMessage(1, info.iCompressionRate);
		stream->send(liveMsg.c_str(), liveMsg.size());
		return true;
	}
	return false;
}

void MilestoneVmsConnector::produceImageObjects(opencctv::ConcurrentQueue<opencctv::Image>* pQueue)
{
	_pDecoder->startDecoding(pQueue);
}

bool MilestoneVmsConnector::isStillProducingImages()
{
	return _pDecoder->isStillProcessing();
}

MilestoneVmsConnector::~MilestoneVmsConnector() {
}

