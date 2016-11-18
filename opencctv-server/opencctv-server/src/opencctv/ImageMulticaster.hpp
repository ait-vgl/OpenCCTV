
#ifndef OPENCCTV_IMAGEMULTICASTER_HPP_
#define OPENCCTV_IMAGEMULTICASTER_HPP_

#include <map>
#include <string>
#include <sstream>
#include "mq/TcpMqSender.hpp"
#include "util/Config.hpp"
#include "Exception.hpp"
#include "util/log/Loggers.hpp"

#include "dto/AnalyticInstanceStream.hpp"
#include "util/flow/FlowController.hpp"
#include "util/serialization/Serializers.hpp"
#include "util/Util.hpp"

#include "../analytic/AnalyticServerManager.hpp"
#include "../analytic/AnalyticServer.hpp"

#include "OpenCCTVServerManager.hpp"
#include "OpenCCTVServer.hpp"

namespace opencctv {

class ImageMulticaster {
private:

	typedef struct {
		mq::Sender* pSender;
		std::string sInputName;
	} MulticastData;

	analytic::AnalyticServer* _pAS;
	OpenCCTVServer* _pOS;

	unsigned int _iStreamId;
	std::map<unsigned int, MulticastData> _mMulticastDests; // Analytic instance id as key for multicast destination

	bool _bEnable;
	util::serialization::Serializable* _pSerializer;
	bool send(mq::Sender* pMqSender, Image* pImage);

public:
	ImageMulticaster(unsigned int iStreamId);
	void addDestination(dto::AnalyticInstanceStream& analyticInstance);
	size_t getNumberOfDestinations();
	void start();
	void stop();
	const bool& isStart();
	void removeElement(unsigned int iAnalyticInstanceStreamId);
	virtual ~ImageMulticaster();
};

} /* namespace opencctv */

#endif /* OPENCCTV_IMAGEMULTICASTER_HPP_ */
