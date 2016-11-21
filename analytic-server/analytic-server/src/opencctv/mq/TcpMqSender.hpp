
#ifndef OPENCCTV_TCPMQSENDER_HPP_
#define OPENCCTV_TCPMQSENDER_HPP_

#include "MqUtil.hpp"
#include "Mq.hpp"
#include "../Exception.hpp"

namespace opencctv {
namespace mq {

class TcpMqSender : public Sender {
private:
	zmq::socket_t* _pSocket;
public:
	TcpMqSender();
	bool createMq(const std::string& serverPortStr);
	bool connectToMq(const std::string& serverName, const std::string& serverPortStr);
	int send(const std::string* pSMessage);
	virtual ~TcpMqSender();
};

} /* namespace mq */
} /* namespace opencctv */

#endif /* OPENCCTV_TCPMQSENDER_HPP_ */
