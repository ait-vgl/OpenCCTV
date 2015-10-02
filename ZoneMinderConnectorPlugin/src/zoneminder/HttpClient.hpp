/*
 * HttpClient.hpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#ifndef HTTPCLIENT_HPP_
#define HTTPCLIENT_HPP_

#include <iostream>
#include <map>
#include <exception>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include "../opencctv/Exception.hpp"

namespace zoneminder {

class HttpClient {
private:

	std::string _sServerName;
	std::string _sServerPort;
	boost::asio::ip::tcp::socket *_socket;
	boost::asio::streambuf _streambufTemp;
	std::map<std::string, std::string> _mapHttpHeaderLines;

public:
	HttpClient(std::string sServerName, std::string sServerPort);
	int sendGetRequest(const std::string& sHost, const std::string& sUrl);
	std::map<std::string,std::string> processHttpHeader();
	void write(std::string message);
	size_t read(unsigned char (&dataBuffer)[2048]);
	virtual ~HttpClient();
};

} /* namespace zoneminder */

#endif /* HTTPCLIENT_HPP_ */
