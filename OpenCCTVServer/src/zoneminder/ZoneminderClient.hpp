/*
 * ZoneminderClient.hpp
 *
 *  Created on: Feb 3, 2015
 *      Author: anjana
 */

#ifndef ZONEMINDERCLIENT_HPP_
#define ZONEMINDERCLIENT_HPP_

#include <iostream>
#include <string>
#include "../VmsClient.hpp"
#include "../tcpsocket/HttpClient.hpp"
#include "../ThreadSafeQueue.hpp"
#include "../db/Stream.hpp"
#include "TcpMpegDecoder.hpp"
#include "ZoneminderMessage.hpp"


using namespace tcpsocket;
using namespace std;

namespace zoneminder {

class ZoneminderClient : public VmsClient
{
private:
	TcpMpegDecoder* _pDecoderPtr;
	string _sUsername;
	string _sPassword;
	string _sServerName;
	int _iServerPort;
	HttpClient* _pHttpClientPtr;

public:
	ZoneminderClient(Stream &stream, Camera &camera);
	virtual ~ZoneminderClient();
	void produceImageObjects(ThreadSafeQueue<Image>& queue, Stream &stream, const unsigned int imageCount);
	bool init();
	bool isStillProducingImages();
};

} /* namespace zoneminder */

#endif /* ZONEMINDERCLIENT_HPP_ */
