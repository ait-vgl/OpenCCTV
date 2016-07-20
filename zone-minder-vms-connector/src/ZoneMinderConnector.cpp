/*
 * ZoneMinderConnector.cpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#include "ZoneMinderConnector.hpp"


ZoneMinderConnector::ZoneMinderConnector()  : opencctv::api::VmsConnector() {
	_pDecoderPtr = NULL; //Initialized in the init() function
	sConnectorDirPath = "";
}

bool ZoneMinderConnector::init(const opencctv::api::VmsConnectInfo& info, const std::string sPathToVmsConnectorDir)
{
	sConnectorDirPath = sPathToVmsConnectorDir;

	std::string sUrl;
	if(!getZMStreamURL(info.sServerIp, info.iServerPort, info.sUsername, info.sPassword, info.sCameraId, sPathToVmsConnectorDir,sUrl))
	{
		_bEnable = false;
		return false;
	}

	_pDecoderPtr = new zoneminder::MpegDecoder();

	if (!_pDecoderPtr->init(info.sServerIp,info.iServerPort,sUrl))
	{
		_bEnable = false;
		delete _pDecoderPtr; _pDecoderPtr = NULL;
		return false;
	}

	//Initialization successful
	_bEnable = true;

	return true;
}

void ZoneMinderConnector::produceImageObjects(opencctv::ConcurrentQueue<opencctv::Image>* pQueue)
{
	if(_pDecoderPtr && _bEnable)
	{
		try
		{
			(*_pDecoderPtr).startDecoding(pQueue);

		}catch(opencctv::Exception &e)
		{
			_bEnable = false;
			throw e;
		}

	}else
	{
		_bEnable = false;
		std::string errorMsg = "Unable to decode images. ZoneMinder connector not initialized properly";
		throw opencctv::Exception(errorMsg);
	}
}

bool ZoneMinderConnector::isStillProducingImages()
{
	if(_pDecoderPtr)
	{
		return (*_pDecoderPtr).isStillProcessing();
	}else
	{
		return false;
	}
}

bool ZoneMinderConnector::getZMStreamURL(const std::string& sServerName, const int iServerPort, const std::string& sUsername, const std::string& sPassword, const std::string& sCameraId, const std::string sConnectorPath, std::string& sUrl)
{
	FILE *in;
	char buff[512];
	std::string sScriptPath = sConnectorPath;
	sScriptPath.append("/");
	sScriptPath.append("zoneminder_url_extractor.rb");

	std::string sUser = sUsername;
	std::string sPwd = sPassword;

	if(sUser.empty()){
		sUser = "guest";
	}

	if(sPwd.empty()){
		sPwd = "guest";
	}

	std::stringstream ssCommand;
	ssCommand << "ruby "<< sScriptPath << " ";
	ssCommand << sServerName << " ";
	ssCommand << iServerPort << " ";
	ssCommand << sUser << " ";
	ssCommand << sPwd << " ";
	ssCommand << sCameraId;

	std::string sCommand = ssCommand.str();

	if(!(in = popen(sCommand.c_str(), "r"))){
			return false;
	}

	std::string  sResult = "";

	while(fgets(buff, sizeof(buff), in) != NULL){
		sResult += buff;
	}
	pclose(in);

	size_t error_pos = sResult.find("Error");
	if (error_pos != std::string::npos)
	{
		sUrl = "";
		return false;
	}

	size_t url_start_pos = sResult.find("/cgi-bin");
	if(url_start_pos == std::string::npos)
	{
		sUrl = "";
		return false;
	}

	sUrl = sResult.substr (url_start_pos);

	//std::cout << sUrl << std::endl << std::endl;
	//std::cout << sResult << std::endl << std::endl;

	return true;
}

ZoneMinderConnector::~ZoneMinderConnector() {
	delete _pDecoderPtr; _pDecoderPtr = NULL;
}


