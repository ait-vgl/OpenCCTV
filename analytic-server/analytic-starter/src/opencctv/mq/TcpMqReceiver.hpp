
#ifndef OPENCCTV_TCPMQRECEIVER_HPP_
#define OPENCCTV_TCPMQRECEIVER_HPP_

#include "MqUtil.hpp"
#include "Mq.hpp"
#include "../Exception.hpp"

namespace opencctv {
namespace mq {

class TcpMqReceiver: public Receiver {
private:
	zmq::socket_t* _pSocket;
public:
	TcpMqReceiver();
	bool createMq(const std::string& serverPortStr);
	bool connectToMq(const std::string& serverName, const std::string& serverPortStr);
	std::string* receive();
	virtual ~TcpMqReceiver();
};

} /* namespace mq */
} /* namespace opencctv */

#endif /* OPENCCTV_TCPMQRECEIVER_HPP_ */
