/*
 * TcpMqClient.hpp
 *
 *  Created on: Oct 13, 2014
 *      Author: lakindu
 */

#ifndef TCPMQCLIENT_HPP_
#define TCPMQCLIENT_HPP_

#include <zmq.hpp>
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;

namespace mq
{
class TcpMq
{
private:
	zmq::context_t* contextPtr;
	zmq::socket_t* socketPtr;
	const string getZmqServerConnectUrl(const string& serverName, unsigned int serverPort);
	const string getZmqServerConnectUrl(const string& serverName, const string& serverPortStr);
	const string getZmqTcpServerBindUrl(const unsigned int serverPort);
	const string getZmqTcpServerBindUrl(const string& serverPortStr);
public:
	TcpMq(unsigned int numbOfIOThreads = 1);
	TcpMq(zmq::context_t* contextPtr);
	bool createNew(unsigned int serverPort, int zmqSocketType);
	bool createNew(const string& serverPortStr, int zmqServerSocketType);
	bool connectTo(const string& serverName, unsigned int serverPort, int zmqSocketType);
	bool connectTo(const string& serverName, const string& serverPortStr, int zmqClientSocketType);
	void write(const string& message);
	string read();
	virtual ~TcpMq();
};
} /* namespace mq */

#endif /* TCPMQCLIENT_HPP_ */
