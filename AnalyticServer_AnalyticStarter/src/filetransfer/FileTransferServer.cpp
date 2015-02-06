/*
 * FileTransferServer.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: anjana
 */

#include "FileTransferServer.hpp"

namespace filetransfer {

FileTransferServer::FileTransferServer(unsigned short usTcpPort)
{
	_usTcpPort = usTcpPort;
}

FileTransferServer::~FileTransferServer()
{
}

int FileTransferServer::receiveFile(boost::asio::ip::tcp::socket &socket, bool &isLastFileReceived)
{
	int iResut = 1;

	size_t file_size = 0;
	string file_path;
	boost::array<char, 1024> buf;
	boost::system::error_code error;

	try
	{
		std::cout << "get client connection." << std::endl;

		boost::asio::streambuf request_buf;
		boost::asio::read_until(socket, request_buf, "\n\n");
		std::cout << "request size:" << request_buf.size() << "\n";
		std::istream request_stream(&request_buf);
		std::string file_path;
		request_stream >> file_path;
		request_stream >> file_size;

		if ((file_path.compare("end") == 0) && (file_size == 0) )
		{
			isLastFileReceived = true;

		}else
		{
			isLastFileReceived = false;

			request_stream.read(buf.c_array(), 2); // eat the "\n\n"

			std::cout << file_path << " size is " << file_size << std::endl;

			/*size_t pos = file_path.find_last_of('/');
			if (pos != std::string::npos)
				file_path = file_path.substr(pos + 1);*/

			std::ofstream output_file(file_path.c_str(), std::ios_base::binary);
			if (!output_file) {
				std::cout << "failed to open " << file_path << std::endl;
				return -1;
			}

			// write extra bytes to file
			do {
				request_stream.read(buf.c_array(), (std::streamsize) buf.size());
				//std::cout << __FUNCTION__ << " write " << request_stream.gcount()<< " bytes.\n";
				output_file.write(buf.c_array(), request_stream.gcount());
			} while (request_stream.gcount() > 0);

			for (;;) {
				size_t len = socket.read_some(boost::asio::buffer(buf), error);
				if (len > 0)
					output_file.write(buf.c_array(), (std::streamsize) len);

				if (output_file.tellp() == (std::fstream::pos_type) (std::streamsize) file_size)
					break; // file was received

				if (error) {
					iResut = -1;
					std::cout << error << std::endl;
					break;
				}
			}
			std::cout << "received " << output_file.tellp() << " bytes.\n";
		}
	}catch (std::exception& e)
	{
		iResut = -1;
		std::cerr << e.what() << std::endl;
	}

	cout << endl;

	return iResut;
}

int FileTransferServer::receiveAllFiles()
{
	int iResut = 1;

	try
	{
		std::cout << " Listen on file transfer port " << _usTcpPort << std::endl;

		boost::asio::io_service io_service;
		boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _usTcpPort));
		bool bLastFileRec = false;

		while(true){
			tcp::socket socket(io_service);
			acceptor.accept(socket);

			iResut = receiveFile(socket, bLastFileRec);

			socket.close();

			if(bLastFileRec)
			{
				break;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		iResut = -1;
	}

	return iResut;
}

} /* namespace filetransfer */
