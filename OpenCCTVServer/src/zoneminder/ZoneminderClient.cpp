/*
 * ZoneminderClient.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: anjana
 */

#include "ZoneminderClient.hpp"

namespace zoneminder {

ZoneminderClient::ZoneminderClient(Stream &stream, Camera &camera) : VmsClient(stream, camera)
{
	_pDecoderPtr = NULL;	//Initialized in the init() function
	_pHttpClientPtr = NULL;	//Initialized in the init() function
	_sUsername = "";
	_sPassword = "";
	_sServerName = "";
	_iServerPort = -1;
}

bool ZoneminderClient::init()
{
	bool bResult = true;

	//TODO : Remove hard coded server name, port and url
	_sServerName = "sServerName";
	_iServerPort = iServerPort;
	string _sUrl = "the strema url";

	_pHttpClientPtr = new HttpClient(_sServerName, _iServerPort);
	if((*_pHttpClientPtr).sendGetRequest(_sServerName, _sUrl))
	{
		map<string,string> mapHttpHeaderDetails = (*_pHttpClientPtr).processHttpHeader();
		map<string,string>::iterator it = mapHttpHeaderDetails.find("Status");

		if(mapHttpHeaderDetails.size() != 0 && it != mapHttpHeaderDetails.end()){
			bResult = ZoneminderMessage::isConnectionSuccessful(mapHttpHeaderDetails["Status"]);
			if(bResult){
				_pDecoderPtr = new TcpMpegDecoder(_pHttpClientPtr);
			}
		}else{
			bResult = false;
		}
	}else
	{
		bResult = false;
	}

	return bResult;
}

void ZoneminderClient::produceImageObjects(ThreadSafeQueue<Image>& queue, Stream &stream, const unsigned int imageCount)
{
	(*_pDecoderPtr).startDecoding(queue, stream, imageCount);
}

bool ZoneminderClient::isStillProducingImages()
{
	return (*_pDecoderPtr).isStillProcessing();
}

ZoneminderClient::~ZoneminderClient()
{
	delete _pDecoderPtr; _pDecoderPtr = NULL;
	delete _pHttpClientPtr; _pHttpClientPtr = NULL;
}

} /* namespace zoneminder */
