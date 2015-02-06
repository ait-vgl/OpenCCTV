/*
 * HttpClient.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#include "HttpClient.hpp"

namespace tcpsocket {

HttpClient::HttpClient(string sServerName, int iServerPort)
{
	try
	{
		boost::asio::io_service io_service;

		// Get a list of endpoints corresponding to the server name.
		boost::asio::ip::tcp::resolver resolver(io_service);
		boost::asio::ip::tcp::resolver::query query(sServerName, boost::lexical_cast<string>(iServerPort));
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;

		// Try each endpoint until we successfully establish a connection.
		//boost::asio::ip::tcp::socket socket(io_service);
		_socket = new boost::asio::ip::tcp::socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end) {
			(*_socket).close();
			(*_socket).connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);

		_pCurrentDataStruct = new ReceivedDataStruct;
		isResidualAvailable = false;
	}
	catch(exception &e)
	{
		cerr << "HttpClient:HttpClient: Exception: " << e.what() << endl;
	}
}

int HttpClient::sendGetRequest(const string &host, const string &url)
{
	size_t iResult = 0;

	try
	{
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << url << " HTTP/1.0\r\n";
		request_stream << "Host: " << host << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		iResult = boost::asio::write((*_socket), request);
	}
	catch(exception &e)
	{
		cerr << "HttpClient:sedGetRequest: Exception: " << e.what() << endl;
	}

	return iResult;
}

map<string,string> HttpClient::processHttpHeader()
{
	_mapHttpHeaderLines.clear();

	try {
		boost::asio::read_until((*_socket), _streambufTemp, "\r\n");

		std::istream response_stream(&_streambufTemp);

		if(response_stream)
		{
			string sLine;
			//Read the status line
			getline(response_stream, sLine);
			_mapHttpHeaderLines["Status"] = sLine;

			boost::asio::read_until((*_socket), _streambufTemp, "\r\n\r\n");

			// Process the rest of the lines in the response headers.
			size_t strPos;
			while (std::getline(response_stream, sLine) && sLine != "\r")
			{
				if((strPos = sLine.find(":")) != string::npos)
					_mapHttpHeaderLines[sLine.substr(0,strPos)] = sLine.substr(strPos+1);
				else
					_mapHttpHeaderLines[sLine] = "";
			}

			// Write whatever content remaining in the _streambufTemp to ReceivedDataStruct.
			if (_streambufTemp.size() > 0)
			{
				isResidualAvailable = true;
				(*_pCurrentDataStruct).length = _streambufTemp.size();

				//_buffer << _streambufTemp;
				istream is(&_streambufTemp);
				is.read(_buffer.c_array(),_streambufTemp.size());
				(*_pCurrentDataStruct).dataPtr = _buffer.c_array();
			}
		}
	} catch(exception &e)
	{
		cerr << "HttpClient:processHttpHeader: Exception: " << e.what() << endl;
	}

	return _mapHttpHeaderLines;
}

void HttpClient::write(string message)
{
	try
	{
		boost::asio::write((*_socket), boost::asio::buffer(message));
	}
	catch(exception &e)
	{
		cerr << "HttpClient:write: Exception: " << e.what() << endl;
	}
}

ReceivedDataStruct* HttpClient::readBytes()
{
	size_t len = 0;
	boost::system::error_code error;

	if(isResidualAvailable)
	{
		isResidualAvailable = false;
		return _pCurrentDataStruct;

	}else
	{
		try
		{
			len = (*_socket).read_some(boost::asio::buffer(_buffer), error);
			//cout << len << endl;
		} catch (exception &e) {
			cerr << "HttpClient:readBytes: Exception: " << e.what() << endl;
		}
		if (error) {
			cerr << "HttpClient:readBytes: Error: " << error.message() << endl;
		}
		(*_pCurrentDataStruct).length = len;
		(*_pCurrentDataStruct).dataPtr = _buffer.c_array();
	}

	return _pCurrentDataStruct;
}

HttpClient::~HttpClient() {
	delete _socket; _socket = NULL;
}

} /* namespace tcpsocket */
