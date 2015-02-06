/*
 * FileTransferServer.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: anjana
 */

#ifndef FILETRANSFERSERVER_HPP_
#define FILETRANSFERSERVER_HPP_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

using boost::asio::ip::tcp;
using namespace std;

namespace filetransfer {

class FileTransferServer {
private:
	unsigned short _usTcpPort;

	int receiveFile(boost::asio::ip::tcp::socket &socket, bool &isLastFileReceived);

public:
	FileTransferServer(unsigned short usTcpPort);
	virtual ~FileTransferServer();
	int receiveAllFiles();
};

} /* namespace filetransfer */

#endif /* FILETRANSFERSERVER_HPP_ */
