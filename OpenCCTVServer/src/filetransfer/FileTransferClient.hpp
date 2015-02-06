/*
 * FileTransferClient.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: anjana
 */

#ifndef FILETRANSFERCLIENT_HPP_
#define FILETRANSFERCLIENT_HPP_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

namespace fs = boost::filesystem;
using boost::asio::ip::tcp;
using namespace std;

namespace filetransfer {

class FileTransferClient {
private:
	string _sFilePath;
	string _sHost;
	int _iPort;
	vector<string> _vFileList;
	int getFileNames();
	int transferFile(const string &sFilename);
	int transferEmptyFile();

public:
	FileTransferClient(string sFilePath, string sHost, int iPort);
	virtual ~FileTransferClient();
	int transferAllFiles();
};

} /* namespace filetransfer */

#endif /* FILETRANSFERCLIENT_HPP_ */
