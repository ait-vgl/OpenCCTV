/*
 * TcpMqClient.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: lakindu
 */

#include "TcpMq.hpp"

namespace mq
{
TcpMq::TcpMq(unsigned int numbOfIOThreads)
{
	if(numbOfIOThreads < 1)
	{
		// not in-process message passing. So at least 1 IO thread.
		// TODO: throw application exception
	}
	contextPtr = NULL;
	contextPtr = new zmq::context_t(numbOfIOThreads);
	if(!contextPtr)
	{
		cerr << "TcpMq: Zmq Error: " << "Unable to create Zmq context. " << zmq_strerror(EINVAL) << endl;
		// TODO: throw application exception
	}
	socketPtr = NULL;
}

TcpMq::TcpMq(zmq::context_t* contextPtr)
{
	this->contextPtr = contextPtr;
	socketPtr = NULL;
}

bool TcpMq::createNew(const string& serverPortStr, int zmqServerSocketType)
{
	if(contextPtr)
	{
		socketPtr = new zmq::socket_t(*contextPtr, zmqServerSocketType);
		if(!socketPtr)
		{
			cerr << "TcpMq: Zmq Error: " << "Unable to create Zmq server socket." << endl;
			// TODO: throw application exception
		}
		try
		{
			(*socketPtr).bind(getZmqTcpServerBindUrl(serverPortStr).data());
			return true;
		}
		catch(error_t &e)
		{
			cerr << "TcpMq: Zmq Error: " << "Unable to create new MQ at " << "localhost" << ":" << serverPortStr << endl;
			// TODO: throw application exception
		}
	}
	return false;
}

bool TcpMq::createNew(unsigned int serverPort, int zmqServerSocketType)
{
	return createNew(boost::lexical_cast<std::string>(serverPort), zmqServerSocketType);
}

bool TcpMq::connectTo(const string& serverName, const string& serverPortStr, int zmqClientSocketType)
{
	if(contextPtr)
	{
		socketPtr = new zmq::socket_t(*contextPtr, zmqClientSocketType);
		if(!socketPtr)
		{
			cerr << "TcpMq:Zmq Error: " << "Unable to create Zmq client socket." << endl;
			// TODO: throw application exception
		}
		try
		{
			(*socketPtr).connect(getZmqServerConnectUrl(serverName, serverPortStr).data());
			return true;
		}
		catch(error_t &e)
		{
			cerr << "TcpMq: Zmq Error: " << "Unable to connect to MQ at " << serverName << ":" << serverPortStr << endl;
			// TODO: throw application exception
		}
	}
	return false;
}

bool TcpMq::connectTo(const string& analyticQueueAddress, int zmqClientSocketType)
{
	if(contextPtr)
	{
		socketPtr = new zmq::socket_t(*contextPtr, zmqClientSocketType);
		if(!socketPtr)
		{
			cerr << "Zmq Error: " << "Unable to create Zmq client socket." << endl;
			// TODO: throw application exception
		}
		try
		{
			(*socketPtr).connect(analyticQueueAddress.data());
			return true;
		}
		catch(error_t &e)
		{
			cerr << "Zmq Error: " << "Unable to connect to MQ at " << analyticQueueAddress << endl;
			// TODO: throw application exception
		}
	}
	return false;
}

bool TcpMq::connectTo(const string& serverName, unsigned int serverPort, int zmqClientSocketType)
{
	return connectTo(serverName, boost::lexical_cast<std::string>(serverPort), zmqClientSocketType);
}

void TcpMq::write(const string& message)
{
	zmq::message_t messageToServer(message.size());
	memcpy((void *) messageToServer.data(), message.data(), message.size());
	bool isSent = (*socketPtr).send(messageToServer);
	if(!isSent)
	{
		cerr << "Zmq Error: " << "Unable to send message to Zmq server." << endl;
		// TODO: throw application exception
	}
}

string TcpMq::read()
{
	zmq::message_t messageFromServer;
	bool isSent = (*socketPtr).recv(&messageFromServer);
	if(!isSent)
	{
		cerr << "Zmq Error: " << "Unable to read message from Zmq server." << endl;
		// TODO: throw application exception
	}
	string message(static_cast<char*>(messageFromServer.data()), messageFromServer.size());
	return message;
}

const string TcpMq::getZmqTcpServerBindUrl(const unsigned int serverPort)
{
	return getZmqTcpServerBindUrl(boost::lexical_cast<std::string>(serverPort));
}

const string TcpMq::getZmqTcpServerBindUrl(const string& serverPortStr)
{
	string url = "tcp://*:";
	url.append(serverPortStr);
	return url;
}

const string TcpMq::getZmqServerConnectUrl(const string& serverName, unsigned int serverPort)
{
	return getZmqServerConnectUrl(serverName, boost::lexical_cast<std::string>(serverPort));
}

const string TcpMq::getZmqServerConnectUrl(const string& serverName, const string& serverPortStr)
{
	string url = "tcp://";
	url.append(serverName);
	url.append(":");
	url.append(serverPortStr);
	return url;
}

TcpMq::~TcpMq()
{
	(*socketPtr).close();
	delete socketPtr;
	(*contextPtr).close();
	delete contextPtr;
}
} /* namespace mq */
