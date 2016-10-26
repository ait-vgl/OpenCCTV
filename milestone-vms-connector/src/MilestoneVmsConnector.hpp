
#ifndef OPENCCTV_MILESTONEVMSCONNECTOR_HPP_
#define OPENCCTV_MILESTONEVMSCONNECTOR_HPP_

#include "opencctv/api/VmsConnector.hpp"
#include "milestone/TcpMpegDecoder.hpp"
#include "tcpsocket/Tcpconnector.hpp"
#include "milestone/MilestoneMessage.hpp"

using namespace milestone;

class MilestoneVmsConnector: public opencctv::api::VmsConnector {
private:
	TcpMpegDecoder* _pDecoder;
public:
	MilestoneVmsConnector();
	bool init(const opencctv::api::VmsConnectInfo& info, const std::string sPathToAnalyticDir);
	void produceImageObjects(opencctv::ConcurrentQueue<opencctv::Image>* pQueue);
	bool isStillProducingImages();
	virtual ~MilestoneVmsConnector();
};

extern "C" MilestoneVmsConnector* create() {
	return new MilestoneVmsConnector;
}

extern "C" void destroy(MilestoneVmsConnector* p) {
	if (p) {
		delete p;
		p = NULL;
	}
}

#endif /* OPENCCTV_MILESTONEVMSCONNECTOR_HPP_ */
