/*
 * MilestoneClient.hpp
 *
 *  Created on: Oct 7, 2014
 *      Author: lakindu
 */

#ifndef MILESTONECLIENT_HPP_
#define MILESTONECLIENT_HPP_

#include "../VmsClient.hpp"
#include "TcpMpegDecoder.hpp"
#include "../db/DbConnector.hpp"
#include <cppconn/prepared_statement.h>
#include "../tcpsocket/TcpClient.hpp"
#include "MilestoneMessage.hpp"

using namespace tcpsocket;

namespace milestone
{
class MilestoneClient : public VmsClient
{
private:
	TcpMpegDecoder* decoderPtr;
	string username;
	string password;
	string serverName;
	int serverPort;
	void extractConnectInformation();
	static const string SELECT_CONNECT_INFO_SQL;
public:
	MilestoneClient(Stream &stream, Camera &camera);
	virtual ~MilestoneClient();
	void produceImageObjects(ThreadSafeQueue<Image>& queue, Stream &stream, const unsigned int imageCount);
	bool init();
	bool isStillProducingImages();
};

} /* namespace milestone */

#endif /* MILESTONECLIENT_HPP_ */
