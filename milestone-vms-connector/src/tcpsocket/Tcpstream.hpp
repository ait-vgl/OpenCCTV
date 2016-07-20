/*
 * TCPStream.hpp
 *
 *  Created on: Sep 2, 2014
 *      Author: anjana
 */

#ifndef __tcpstream_h__
#define __tcpstream_h__

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

using namespace std;

class TCPStream
{
    int     m_sd;
    string  m_peerIP;
    int     m_peerPort;

  public:
    friend class TCPAcceptor;
    friend class TCPConnector;

    ~TCPStream();

    ssize_t send(const char* buffer, size_t len);
    ssize_t receive(char* buffer, size_t len, int timeout=0);

    string getPeerIP();
    int    getPeerPort();

    enum {
        connectionClosed = 0,
        connectionReset = -1,
        connectionTimedOut = -2
    };

  private:
    bool waitForReadEvent(int timeout);
    
    TCPStream(int sd, struct sockaddr_in* address);
    TCPStream();
    TCPStream(const TCPStream& stream);
};

#endif
