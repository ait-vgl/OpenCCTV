
#include "TcpMqSender.hpp"

namespace opencctv {
namespace mq {

TcpMqSender::TcpMqSender() {
	_pSocket = NULL;
}

bool TcpMqSender::createMq(const std::string& serverPortStr) {
	try
	{
		_pSocket = MqUtil::createNewMq(serverPortStr, ZMQ_PUSH);
	}
	catch(std::runtime_error &e)
	{
		throw Exception(std::string(e.what()));
	}
	if (_pSocket) {
		return true;
	}
	return false;
}

bool TcpMqSender::connectToMq(const std::string& serverName, const std::string& serverPortStr) {
	try
	{
		_pSocket = MqUtil::connectToMq(serverName, serverPortStr, ZMQ_PUSH);
	}
	catch(std::runtime_error &e)
	{
		throw Exception(e.what());
	}
	if (_pSocket) {
		return true;
	}
	return false;
}

/**
 * Alert! Please remember to delete the inputed std::string
 * if it was created on the heap using new operator.
 * Use this function when sending very large serialized images to the MQ.
 */
int TcpMqSender::send(const std::string* pSMessage) {
	bool sent = false;
	try
	{
		sent = MqUtil::writeToSocket(_pSocket, pSMessage);
	}
	catch(std::runtime_error &e)
	{
		throw Exception(e.what());
	}
	return sent;
}

TcpMqSender::~TcpMqSender() {
	if(_pSocket)
	{
		_pSocket->close();
		delete _pSocket;
	}
}

} /* namespace mq */
} /* namespace opencctv */
