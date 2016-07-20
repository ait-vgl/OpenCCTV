
#ifndef OPENCCTV_IMAGEMULTICASTER_HPP_
#define OPENCCTV_IMAGEMULTICASTER_HPP_

#include <map>
#include <string>
#include <sstream>
#include "mq/TcpMqSender.hpp"
#include "util/Config.hpp"
#include "Exception.hpp"
#include "util/log/Loggers.hpp"
#include "ApplicationModel.hpp"
#include "dto/AnalyticInstanceStream.hpp"
#include "util/flow/FlowController.hpp"
#include "util/serialization/Serializers.hpp"
#include "util/Util.hpp"

namespace opencctv {

class ImageMulticaster {
private:
	typedef struct {
		unsigned int iAnalyticInstanceId;
		std::string sInputName;
	} Element;
	unsigned int _iStreamId;
	std::map<unsigned int, Element> _mAISInfo; // Analytic Instance Stream ID as key
	bool _bEnable;
	util::serialization::Serializable* _pSerializer;
	bool send(mq::Sender* pMqSender, Image* pImage);
public:
	ImageMulticaster(unsigned int iStreamId);
	void addDestination(const dto::AnalyticInstanceStream& analyticInstance);
	size_t getNumberOfDestinations();
	void start();
	void stop();
	virtual ~ImageMulticaster();
};

} /* namespace opencctv */

#endif /* OPENCCTV_IMAGEMULTICASTER_HPP_ */
