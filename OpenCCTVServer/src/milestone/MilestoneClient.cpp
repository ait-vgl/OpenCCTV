/*
 * MilestoneClient.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: lakindu
 */

#include "MilestoneClient.hpp"

namespace milestone
{
const string MilestoneClient::SELECT_CONNECT_INFO_SQL = "SELECT v.username, v.password, v.server_name, v.server_port FROM vmses AS v, cameras AS c WHERE c.vms_id = v.id AND c.id = ?";

MilestoneClient::MilestoneClient(Stream &stream, Camera &camera) : VmsClient(stream, camera)
{
	decoderPtr = NULL;
	username = "";
	password = "";
	serverName = "";
	serverPort = -1;
}

bool MilestoneClient::init()
{
	extractConnectInformation();
	TcpClient* tcpClientPtr = new TcpClient(serverName, serverPort);
	decoderPtr = new TcpMpegDecoder(tcpClientPtr);
	string connectMsg = MilestoneMessage::getConnectMessage(0, username, password, _camera.getCameraId(), _stream.getWidth(), _stream.getHeight(), _stream.getKeepAspectRatio(), _stream.getAllowupSizing());
	(*tcpClientPtr).write(connectMsg);
	string connectResponse = (*tcpClientPtr).read();
	//cout << connectResponse;
	if(MilestoneMessage::isConnectionSuccessful(connectResponse))
	{
		string liveMsg = MilestoneMessage::getLiveMessage(1, _stream.getCompressionRate());
		//cout << liveMsg;
		(*tcpClientPtr).write(liveMsg);
		return true;
	}
	return false;
}

void MilestoneClient::produceImageObjects(ThreadSafeQueue<Image>& queue, Stream &stream, const unsigned int imageCount)
{
	(*decoderPtr).startDecoding(queue, stream, imageCount);
}

void MilestoneClient::extractConnectInformation()
{
	Connection* dbConnPtr = NULL;
	PreparedStatement* statementPtr = NULL;
	ResultSet* resultsPtr = NULL;
	try
	{
		dbConnPtr = DbConnector::getConnection();
		statementPtr = (*dbConnPtr).prepareStatement(SELECT_CONNECT_INFO_SQL);
		(*statementPtr).setInt(1, _camera.getId());
		resultsPtr = (*statementPtr).executeQuery();
		if((*resultsPtr).next())
		{
			username = (*resultsPtr).getString("username");
			password = (*resultsPtr).getString("password");
			serverName = (*resultsPtr).getString("server_name");
			serverPort = (*resultsPtr).getInt("server_port");
		}
		else
		{
			std::cerr << "MilestoneClient::extractConnectInformation: Error: " << "No connect information in the database for camera " << _camera.getId() << endl;
		}
	}
	catch(SQLException &e)
	{
		std::cerr << "MilestoneClient::extractConnectInformation: " << "Failed to extract VMS information from the database. " << e.what() << endl;
	}
	(*resultsPtr).close();
	(*statementPtr).close();
	(*dbConnPtr).close();
	delete resultsPtr; resultsPtr = NULL;
	delete statementPtr; resultsPtr = NULL;
	delete dbConnPtr; dbConnPtr = NULL;
}

bool MilestoneClient::isStillProducingImages()
{
	return (*decoderPtr).isStillProcessing();
}

MilestoneClient::~MilestoneClient()
{
	//TODO : Need to perform some delete operations here
}

} /* namespace milestone */
