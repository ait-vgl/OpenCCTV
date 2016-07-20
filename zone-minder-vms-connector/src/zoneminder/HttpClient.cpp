/*
 * HttpClient.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "HttpClient.hpp"

namespace zoneminder {

HttpClient::HttpClient(std::string sServerName, std::string sServerPort) : _sServerName(sServerName), _sServerPort(sServerPort)
{

	try
	{
		boost::asio::io_service io_service;
		// Get a list of endpoints corresponding to the server name.
		boost::asio::ip::tcp::resolver resolver(io_service);

		/*if (sServerPort.compare("80") == 0)
		{
			sServerPort = "http";
		}*/

		boost::asio::ip::tcp::resolver::query query(sServerName, sServerPort);

		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;

		// Try each endpoint until we successfully establish a connection.
		_socket = new boost::asio::ip::tcp::socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end) {
			(*_socket).close();
			(*_socket).connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);
	}
	catch(std::exception &e)
	{
		delete _socket; _socket = NULL;
		std::string sErrorMessage = "Unable to connect to ZoneMinder Server at ";
		sErrorMessage.append(_sServerName);
		sErrorMessage.append(":");
		sErrorMessage.append(_sServerPort);
		sErrorMessage.append(" - ");
		sErrorMessage.append(e.what());
		throw opencctv::Exception(sErrorMessage);
	}

}

int HttpClient::sendGetRequest(const std::string& sHost, const std::string& sUrl)
{
	size_t iResult = 0;

	try
	{
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << sUrl << " HTTP/1.0\r\n";
		request_stream << "Host: " << sHost << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		iResult = boost::asio::write((*_socket), request);
	}
	catch(std::exception &e)
	{
		std::string sErrorMessage = "Faild to send a HTTP GET request to ZoneMinder Server at ";
		sErrorMessage.append(_sServerName);
		sErrorMessage.append(":");
		sErrorMessage.append(_sServerPort);
		sErrorMessage.append(" - ");
		sErrorMessage.append(e.what());
		throw opencctv::Exception(sErrorMessage);
	}

	return iResult;
}

std::map<std::string,std::string> HttpClient::processHttpHeader()
{
	_mapHttpHeaderLines.clear();

	try {
		boost::asio::read_until((*_socket), _streambufTemp, "\r\n");

		std::istream response_stream(&_streambufTemp);

		if(response_stream)
		{
			std::string sLine;
			//Read the status line
			getline(response_stream, sLine);
			_mapHttpHeaderLines["Status"] = sLine;

			boost::asio::read_until((*_socket), _streambufTemp, "\r\n\r\n");

			// Process the rest of the lines in the response headers.
			size_t strPos;
			while (std::getline(response_stream, sLine) && sLine != "\r")
			{
				if((strPos = sLine.find(":")) != std::string::npos)
					_mapHttpHeaderLines[sLine.substr(0,strPos)] = sLine.substr(strPos+1);
				else
					_mapHttpHeaderLines[sLine] = "";
			}
		}
	} catch(std::exception &e)
	{
		std::string sErrorMessage = "Faild to process the HTTP headers sent by the ZoneMinder Server at ";
		sErrorMessage.append(_sServerName);
		sErrorMessage.append(":");
		sErrorMessage.append(_sServerPort);
		sErrorMessage.append(" - ");
		sErrorMessage.append(e.what());
		throw opencctv::Exception(sErrorMessage);
	}

	return _mapHttpHeaderLines;
}

void HttpClient::write(std::string message)
{
	try
	{
		boost::asio::write((*_socket), boost::asio::buffer(message));
	}
	catch(std::exception &e)
	{
		std::string sErrorMessage = "Faild to write data to the ZoneMinder Server socket at ";
		sErrorMessage.append(_sServerName);
		sErrorMessage.append(":");
		sErrorMessage.append(_sServerPort);
		sErrorMessage.append(" - ");
		sErrorMessage.append(e.what());
		throw opencctv::Exception(sErrorMessage);
	}
}

size_t HttpClient::read(unsigned char (&dataBuffer)[2048])
{
	size_t iLength = 0;
	boost::system::error_code error;
	iLength = (*_socket).read_some(boost::asio::buffer(dataBuffer), error);

	if (error)
	{
		iLength = 0;
		std::string sErrorMessage = "Faild to read data from the ZoneMinder Server socket at";
		sErrorMessage.append(_sServerName);
		sErrorMessage.append(":");
		sErrorMessage.append(_sServerPort);
		sErrorMessage.append(" - ");
		sErrorMessage.append(error.message());
		throw opencctv::Exception(sErrorMessage);
	}

	return iLength;

}

HttpClient::~HttpClient() {
	delete _socket; _socket = NULL;
}

} /* namespace zoneminder */
