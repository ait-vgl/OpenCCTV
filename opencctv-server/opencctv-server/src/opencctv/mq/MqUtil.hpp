
#ifndef OPENCCTV_MQUTIL_HPP_
#define OPENCCTV_MQUTIL_HPP_

/* zero mq includes (/usr/local/include/) */
/* zero mq library links (/usr/local/lib/) -lzmq */
#include <zmq.hpp>
#include <string>
/* boost includes (/usr/local/include/) */
/* boost library links (/usr/local/lib/) -lboost_system */
#include <boost/lexical_cast.hpp> // to cast types
#include <exception> // to throw runtime exceptions

namespace opencctv {
namespace mq {

class MqUtil {
private:
	static zmq::context_t* pContext;
	static std::string getZmqTcpServerBindUrl(const std::string& serverPortStr);
	static std::string getZmqServerConnectUrl(const std::string& serverName, const std::string& serverPortStr);
public:
	static zmq::socket_t* createNewMq(const std::string& serverPortStr, int zmqServerSocketType);
	static zmq::socket_t* connectToMq(const std::string& serverName, const std::string& serverPortStr, int zmqClientSocketType);
	static bool writeToSocket(zmq::socket_t* pSocket, const std::string* pSMessage);
	static std::string* readFromSocket(zmq::socket_t* pSocket);
	static bool writeToSocket(zmq::socket_t* pSocket, const std::string& sMessage);
	static void readFromSocket(zmq::socket_t* pSocket, std::string& sMessage);
};

} /* namespace mq */
} /* namespace opencctv */

#endif /* OPENCCTV_MQUTIL_HPP_ */
