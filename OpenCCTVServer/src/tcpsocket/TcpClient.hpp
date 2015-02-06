/*
 * TcpClient.hpp
 *
 *  Created on: Sep 28, 2014
 *      Author: lakindu
 */

#ifndef TCPCLIENT_HPP_
#define TCPCLIENT_HPP_

#include <boost/asio.hpp>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "SocketTypeDef.hpp"

using namespace std;

namespace tcpsocket
{

/*struct ReceivedDataStruct
{
	char* dataPtr;
	size_t length;
};*/

class TcpClient
{
private:
	boost::asio::io_service* ioServicePtr;
	boost::asio::ip::tcp::resolver* resolverPtr;
	boost::asio::ip::tcp::resolver::iterator endpoint;
	boost::asio::ip::tcp::socket* socketPtr;
	boost::array<char, 2048>* bufferPtr;
	ReceivedDataStruct* currentDataStruct;
public:
	TcpClient(string tcpServerName, int tcpServerPort);
	string read();
	ReceivedDataStruct* readBytes();
	int readBytes(char *pdata);
	void write(string message);
	virtual ~TcpClient();
};
}

#endif /* TCPCLIENT_HPP_ */
