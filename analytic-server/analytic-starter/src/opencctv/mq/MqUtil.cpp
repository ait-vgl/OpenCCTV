
#include "MqUtil.hpp"

namespace opencctv {
namespace mq {

zmq::context_t* MqUtil::pContext = new zmq::context_t(1);

/**
 * Alert! Please remember to close() and delete returned zmq::socket_t
 * when you are done with it. It was created on the heap using new operator.
 */
zmq::socket_t* MqUtil::createNewMq(const std::string& serverPortStr, int zmqServerSocketType) {
	zmq::socket_t* pSocket = NULL;
	try {
		pSocket = new zmq::socket_t(*pContext, zmqServerSocketType);
		if (pSocket) {
			pSocket->bind(getZmqTcpServerBindUrl(serverPortStr).data());
		}
	} catch (error_t &e) {
		std::string sErrMsg = "Failed to create new MQ at port: ";
		sErrMsg.append(serverPortStr);
		throw std::runtime_error(sErrMsg);
	}
	return pSocket;
}

/**
 * Alert! Please remember to close() and delete returned zmq::socket_t
 * when you are done with it. It was created on the heap using new operator.
 */
zmq::socket_t* MqUtil::connectToMq(const std::string& serverName, const std::string& serverPortStr, int zmqClientSocketType) {
	zmq::socket_t* pSocket = NULL;
	try {
		pSocket = new zmq::socket_t(*pContext, zmqClientSocketType);
		if (pSocket) {
			pSocket->connect(
					getZmqServerConnectUrl(serverName, serverPortStr).data());
		}
	} catch (error_t &e) {
		std::string sErrMsg = "Failed to connect to MQ at: ";
		sErrMsg.append(serverName);
		sErrMsg.append(":");
		sErrMsg.append(serverPortStr);
		throw std::runtime_error(sErrMsg);
	}
	return pSocket;
}

/**
 * Alert! Please remember to delete the inputed std::string
 * if it was created on the heap using new operator.
 * Use this function when writing very large messages to the Socket.
 */
bool MqUtil::writeToSocket(zmq::socket_t* pSocket, const std::string* pSMessage) {
	bool sent = false;
	if(pSocket)
	{
		zmq::message_t* pMessageToServer = NULL;
		try {
			pMessageToServer = new zmq::message_t(pSMessage->size());
			memcpy((void *) pMessageToServer->data(), pSMessage->data(),
					pSMessage->size());
			sent = pSocket->send(*pMessageToServer);
		} catch (error_t &e) {
			if (pMessageToServer) delete pMessageToServer;
			throw std::runtime_error("Unable to send to the Socket");
		}
		if (pMessageToServer) delete pMessageToServer;
	}
	return sent;
}

bool MqUtil::writeToSocket(zmq::socket_t* pSocket, const std::string& sMessage) {
	bool sent = false;
	if(pSocket)
	{
		try {
			zmq::message_t messageToServer(sMessage.size());
			memcpy((void *) messageToServer.data(), sMessage.data(),
					sMessage.size());
			sent = pSocket->send(messageToServer);
		} catch (error_t &e) {
			throw std::runtime_error("Unable to send to the Socket");
		}
	}
	return sent;
}

/**
 * Alert! Please remember to delete the returned std::string after using.
 * Because it has been created on the heap using new operator.
 * Use this function when reading very large messages from the Socket.
 */
std::string* MqUtil::readFromSocket(zmq::socket_t* pSocket) {
	std::string* pSMessage = NULL;
	if(pSocket)
	{
		zmq::message_t* pMessageFromServer = new zmq::message_t();
		bool received = false;
		try {
			received = pSocket->recv(pMessageFromServer);
		} catch (error_t &e) {
			if (pMessageFromServer) delete pMessageFromServer;
			throw std::runtime_error("Unable to receive from the Socket");
		}
		if (!received) {
			if (pMessageFromServer) delete pMessageFromServer;
			throw std::runtime_error("Unable to receive from the Socket");
		}
		pSMessage = new std::string(static_cast<char*>(pMessageFromServer->data()),
				pMessageFromServer->size());
		if (pMessageFromServer) delete pMessageFromServer;
	}
	return pSMessage;
}

void MqUtil::readFromSocket(zmq::socket_t* pSocket, std::string& sMessage) {
	zmq::message_t messageFromServer;
	bool received = false;
	try {
		received = pSocket->recv(&messageFromServer);
	} catch (error_t &e) {
		throw std::runtime_error("Unable to receive from the Socket");
	}
	if (!received) {
		throw std::runtime_error("Unable to receive from the Socket");
	}
	sMessage = std::string(static_cast<char*>(messageFromServer.data()), messageFromServer.size());
}

std::string MqUtil::getZmqTcpServerBindUrl(const std::string& serverPortStr) {
	std::string url = "tcp://*:";
	url.append(serverPortStr);
	return url;
}

std::string MqUtil::getZmqServerConnectUrl(const std::string& serverName,
		const std::string& serverPortStr) {
	std::string url = "tcp://";
	url.append(serverName);
	url.append(":");
	url.append(serverPortStr);
	return url;
}

} /* namespace mq */
} /* namespace opencctv */
