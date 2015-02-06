/*
 * HttpClient.hpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#ifndef HTTPCLIENT_HPP_
#define HTTPCLIENT_HPP_

#include <boost/asio.hpp>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "SocketTypeDef.hpp"

using namespace std;

namespace tcpsocket {

class HttpClient {
private:
	//boost::asio::io_service* ioServicePtr;
	//boost::asio::ip::tcp::resolver* resolverPtr;
	//boost::asio::ip::tcp::resolver::iterator endpoint;
	boost::asio::ip::tcp::socket *_socket;
	boost::asio::streambuf _streambufTemp;
	boost::array<char, 2048> _buffer;
	ReceivedDataStruct *_pCurrentDataStruct;
	map<string, string> _mapHttpHeaderLines;
	bool isResidualAvailable;

public:
	HttpClient(string sServerName, int iServerPort);
	int sendGetRequest(const string &host, const string &url);
	map<string,string> processHttpHeader();
	void write(string message);
	ReceivedDataStruct* readBytes();
	virtual ~HttpClient();
};

} /* namespace tcpsocket */

#endif /* HTTPCLIENT_HPP_ */
