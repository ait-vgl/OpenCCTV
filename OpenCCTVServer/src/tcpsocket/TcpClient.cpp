/*
 * TcpClient.cpp
 *
 *  Created on: Sep 28, 2014
 *      Author: lakindu
 */

#include "TcpClient.hpp"

namespace tcpsocket
{
TcpClient::TcpClient(string tcpServerName, int tcpServerPort)
{
	try
	{
		ioServicePtr = new boost::asio::io_service();
		resolverPtr = new boost::asio::ip::tcp::resolver((*ioServicePtr));
		endpoint = (*resolverPtr).resolve(boost::asio::ip::tcp::resolver::query(tcpServerName, boost::lexical_cast<std::string>(tcpServerPort)));
		socketPtr = new boost::asio::ip::tcp::socket((*ioServicePtr));
		boost::asio::connect((*socketPtr), endpoint);
		bufferPtr = new boost::array<char, 2048>();
		currentDataStruct = new ReceivedDataStruct;
	}
	catch(exception &e)
	{
		cerr << "TcpClient:TcpClient: Exception: " << e.what() << endl;
	}
}

//string TcpClient::readLine()
//{
//	string line;
//	try
//	{
//		boost::asio::streambuf buf;
//		boost::asio::read_until((*socketPtr), buf, '\n');
//		istream is(&buf);
//		getline(is, line);
//	}
//	catch(exception &e)
//	{
//		std::cerr << "Exception: " << e.what() << endl;
//	}
//	return line;
//}

//string TcpClient::readUntil(string endingRegex)
//{
//	string line;
//	try
//	{
//		boost::asio::streambuf buf;
//		boost::asio::read_until((*socketPtr), buf, endingRegex);
//		istream is(&buf);
//		is >> line;
//	}
//	catch(exception &e)
//	{
//		std::cerr << "Exception: " << e.what() << endl;
//	}
//	return line;
//}

void TcpClient::write(string message)
{
	try
	{
		boost::asio::write((*socketPtr), boost::asio::buffer(message));
	}
	catch(exception &e)
	{
		cerr << "TcpClient:write: Exception: " << e.what() << endl;
	}
}

ReceivedDataStruct* TcpClient::readBytes()
{
	size_t len = 0;
	boost::system::error_code error;
	try
	{
//		cout << "OK!\n";
//		cout << len << endl;
		len = (*socketPtr).read_some(boost::asio::buffer(*bufferPtr), error);
//		cout << len << endl;
	}
	catch(exception &e)
	{
		cerr << "TcpClient:readBytes: Exception: " << e.what() << endl;
	}
	if(error)
	{
		cerr << "TcpClient:readBytes: Error: " << error.message() << endl;
	}
	(*currentDataStruct).length = len;
	(*currentDataStruct).dataPtr = (*bufferPtr).c_array();
	return currentDataStruct;
}

int TcpClient::readBytes(char* pdata)
{
	size_t len = 0;
	boost::system::error_code error;
	try
	{
		cout << "OK!\n";
		cout << len << endl;
		len = (*socketPtr).read_some(boost::asio::buffer(*bufferPtr), error);
		cout << len << endl;
	}
	catch(exception &e)
	{
		cerr << "TcpClient:readBytes: Exception: " << e.what() << endl;
	}
	if(error)
	{
		cerr << "TcpClient:readBytes: Error: " << error.message() << endl;
	}
	pdata = (*bufferPtr).c_array();
	return len;
}

string TcpClient::read()
{
	size_t len = 0;
	boost::system::error_code error;
	try
	{
		len = (*socketPtr).read_some(boost::asio::buffer(*bufferPtr), error);
	} catch (exception &e)
	{
		cerr << "TcpClient:read: Exception: " << e.what() << endl;
	}
	if (error)
	{
		cerr << "TcpClient:read: Error: " << error.message() << endl;
	}
	string receivedStr((*bufferPtr).c_array(), len);
	return receivedStr;
}

TcpClient::~TcpClient()
{
	(*socketPtr).close();
	(*resolverPtr).cancel();
	(*ioServicePtr).stop();
	delete socketPtr;
	delete resolverPtr;
	delete ioServicePtr;
	delete bufferPtr;
}
}
