/*
 * FileTransferClient.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: anjana
 */

#include "FileTransferClient.hpp"

namespace filetransfer {

FileTransferClient::FileTransferClient(string sFilePath, string sHost,int iPort)
{
	_sFilePath = sFilePath;
	_sHost = sHost;
	_iPort = iPort;
}

FileTransferClient::~FileTransferClient()
{
}

int FileTransferClient::transferAllFiles()
{
	int iResult = 1;

	if (getFileNames()) {
		 vector<string>::iterator it;
		 for (size_t i = 0; i < _vFileList.size(); i++)
		 {
			 transferFile(_vFileList[i]);
		 }
	} else {
		iResult = -1;
	}

	iResult = transferEmptyFile();

	return iResult;
}

int FileTransferClient::getFileNames() {
	int iResult = 1;

	if (fs::is_directory(_sFilePath)) {
		fs::directory_iterator end_iter;

		for (fs::directory_iterator dir_itr(_sFilePath); dir_itr != end_iter;
				++dir_itr) {
			if (fs::is_regular_file(dir_itr->status())) {
				/*cout << dir_itr->path().filename() << ' '; // display filename only
				 cout << " [" << file_size(dir_itr->path()) << ']';
				 cout << '\n';*/
				_vFileList.push_back(dir_itr->path().filename().string());
			}
		}
	} else {
		cout << (fs::exists(_sFilePath) ? "Found: " : "Not found: ")
				<< _sFilePath << '\n';
		iResult = -1;
	}

	return iResult;
}

int FileTransferClient::transferFile(const string &sFilename) {
	int iResult = 1;

	try
	{
		
		string sFullFilePath = _sFilePath + sFilename;

		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		string sPort = boost::lexical_cast<string>(_iPort);
		tcp::resolver::query query(_sHost,sPort);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;
		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			return -1;

		std::cout << "connected to " << _sHost << ":" << _iPort << std::endl;
		boost::array<char, 1024> buf;
		std::ifstream source_file(sFullFilePath.c_str(),std::ios_base::binary | std::ios_base::ate);
		if (!source_file)
		{
			std::cout << "failed to open " << sFullFilePath << std::endl;
			return -1;
		}

		size_t file_size = source_file.tellg();
		source_file.seekg(0);

		//First send file name and file size to server
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << sFullFilePath << "\n" << file_size << "\n\n";
		boost::asio::write(socket, request);

		//Then send the content of the file
		std::cout << "start sending file content.\n";
		for (;;)
		{
			if (source_file.eof() == false)
			{
				source_file.read(buf.c_array(), (std::streamsize) buf.size());
				if (source_file.gcount() <= 0)
				{
					std::cout << "read file error " << std::endl;
					return 1;
				}
				boost::asio::write(socket, boost::asio::buffer(buf.c_array(), source_file.gcount()), boost::asio::transfer_all(), error);
				if (error)
				{
					std::cout << "send error:" << error << std::endl;
					return -1;
				}
			} else
			{
				break;
			}
		}
		socket.close();
		std::cout << "send file " << sFullFilePath << " completed successfully.\n";
	} catch (std::exception& e) {
		//socket.close();
		std::cerr << e.what() << std::endl;
	}

	cout << endl;

	return iResult;
}

int FileTransferClient::transferEmptyFile()
{
	int iResult = 1;

	try
	{
		string sFullFilePath = "end";

		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		string sPort = boost::lexical_cast<string>(_iPort);
		tcp::resolver::query query(_sHost,sPort);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;
		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			return -1;

		std::cout << "connected to " << _sHost << ":" << _iPort << std::endl;

		size_t file_size = 0;

		//Send file name and file size to server
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << sFullFilePath << "\n" << file_size << "\n\n";
		boost::asio::write(socket, request);
		socket.close();
		std::cout << "send file " << sFullFilePath << " completed successfully.\n";

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return iResult;
}

} /* namespace filetransfer */
