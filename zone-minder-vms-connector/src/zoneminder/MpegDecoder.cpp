/*
 * MpegDecoder.cpp
 *
 *  Created on: Jun 13, 2015
 *      Author: anjana
 */

#include "MpegDecoder.hpp"

namespace zoneminder {

const string MpegDecoder::CONT_LEN_HEADER = "Content-Length: ";
const string MpegDecoder::CUR_TIME_HEADER = "Timestamp: ";

MpegDecoder::MpegDecoder() {
	_iCurrentContentLength = 0;
	_sCurrentImageTimeStamp = "";
	_pHttpClientPtr = NULL;
	_bDecodeEnable = false;
	_bStillProcessing = false;
	_iDecodedImageCount = 0;
}

bool MpegDecoder::init(string sServerName, int iServerPort, string sUrl)
{
	bool bResult = true;

	string sServerPort =  boost::lexical_cast<string>(iServerPort);

	try
	{
		_pHttpClientPtr = new HttpClient(sServerName, sServerPort);
		if((*_pHttpClientPtr).sendGetRequest(sServerName, sUrl))
		{
			map<string,string> mapHttpHeaderDetails = (*_pHttpClientPtr).processHttpHeader();
			map<string,string>::iterator it = mapHttpHeaderDetails.find("Status");

			if(mapHttpHeaderDetails.size() != 0 && it != mapHttpHeaderDetails.end()){
				bResult = ZoneminderMessage::isConnectionSuccessful(mapHttpHeaderDetails["Status"]);
			}else{
				bResult = false;
			}
		}else
		{
			bResult = false;
		}
	}catch(opencctv::Exception &e)
	{
		bResult = false;
	}

	if(!bResult)
	{
		delete _pHttpClientPtr; _pHttpClientPtr = NULL;
	}

	return bResult;
}

void MpegDecoder::startDecoding(opencctv::ConcurrentQueue<opencctv::Image>* pQueue)
{
	unsigned char* prevBytePtr = NULL;
	unsigned char* curBytePtr = NULL;
	//char readBytes[2048];
	unsigned char readBytes[2048];
	size_t length = 0;
	_bStillProcessing = true;

	if(_pHttpClientPtr)
	{
		_bDecodeEnable = true;
	}

	while(_bDecodeEnable)
	{
		try
		{
			length = (*_pHttpClientPtr).read(readBytes);

		}catch(opencctv::Exception &e)
		{
			_bStillProcessing = false;
			_bDecodeEnable = false;
			throw e;
		}

		ostringstream strOtherThanImageData;
		size_t i = 0;
		while(i < length)
		{
			//curBytePtr = ((unsigned char*) &readBytes[0]) + i;
			curBytePtr = readBytes + i;
			if (prevBytePtr != NULL && (*prevBytePtr) == 255 && (*curBytePtr) == 216) //start of jpeg 0xFF 0xD8
			{
				string metaDataStr = strOtherThanImageData.str();

				if(extractStreamHeaderData(metaDataStr)) //Copy the data only if the image headers are properly extracted
				{
					if(_iCurrentContentLength > (length - i))
					{
						_vCurrentImageData.insert(_vCurrentImageData.end(), prevBytePtr, (prevBytePtr + length - i + 1));
						i = length;
					}
					else
					{
						_vCurrentImageData.insert(_vCurrentImageData.end(), prevBytePtr, (prevBytePtr + _iCurrentContentLength));
						i += _iCurrentContentLength - 1;
					}
				}

				strOtherThanImageData.clear();
			}
			else if (_vCurrentImageData.size() > 0)
			{
				if(_vCurrentImageData.size() < _iCurrentContentLength)
				{
					if((length - i) <= (_iCurrentContentLength - _vCurrentImageData.size()))
					{
						_vCurrentImageData.insert(_vCurrentImageData.end(), curBytePtr, (curBytePtr + (length - i - 1 + 1)));
						i = length;
					}
					else
					{
						_vCurrentImageData.insert(_vCurrentImageData.end(), curBytePtr, (curBytePtr + (_iCurrentContentLength - _vCurrentImageData.size() - 1 + 1)));
						i += (_iCurrentContentLength - _vCurrentImageData.size());
					}
				}
				else if(_vCurrentImageData.size() == _iCurrentContentLength)
				{
					if(_vCurrentImageData[_iCurrentContentLength - 2] == 255 && _vCurrentImageData[_iCurrentContentLength - 1] == 217) //end of jpeg 0xFF 0xD9
					{
						//got jpeg
						_iDecodedImageCount++;
						opencctv::Image* pImage = new opencctv::Image();
						pImage->setImageData(_vCurrentImageData);
						pImage->setTimestamp(_sCurrentImageTimeStamp);
						pQueue->push(pImage);
						//cout << "Decoded image no : " << _iDecodedImageCount << endl;
					}
					else //error
					{
						cerr << "ZoneMinder:MpegDecoder:startDecoding: Error - in decoding Mpeg stream." << std::endl;
					}
					_vCurrentImageData.clear();
				}
			}
			else if(_vCurrentImageData.size() == 0)
			{
				strOtherThanImageData << *curBytePtr;
				i++;
			}
			else
			{
				cerr << "ZoneMinder:MpegDecoder:startDecoding: Error - cannot reach this part of the program." << std::endl;
			}
			prevBytePtr = curBytePtr;
		}
	}
}

bool MpegDecoder::extractStreamHeaderData(const string& streamHeaders)
{
	bool extractedContentLength = false;
	bool extractedTimestamp = false;
	vector<string> subStrs;
	size_t strPos;
	_iCurrentContentLength = 0;
	_sCurrentImageTimeStamp = "";

	boost::split(subStrs, streamHeaders, boost::is_any_of("\n\r"));
	//std::cout << "============Begin======================" << std::endl;
	//cout << streamHeaders << endl;
	for (std::vector<std::string>::iterator subStrPtr = subStrs.begin(); subStrPtr != subStrs.end(); ++subStrPtr)
	{
		//cout << (*subStrPtr) << endl;
		//TODO : substr can be simplified???
		if((strPos = (*subStrPtr).find(CONT_LEN_HEADER)) != string::npos)
		{
			string dataStr;
			dataStr = (*subStrPtr).substr((strPos + CONT_LEN_HEADER.length()), ((*subStrPtr).length() - CONT_LEN_HEADER.length()));
			_iCurrentContentLength = boost::lexical_cast<unsigned int>(dataStr);
			extractedContentLength = true;
		}

		if((strPos = (*subStrPtr).find(CUR_TIME_HEADER)) != string::npos)
		{
			_sCurrentImageTimeStamp = (*subStrPtr).substr((strPos + CUR_TIME_HEADER.length()), ((*subStrPtr).length() - CUR_TIME_HEADER.length()));
			extractedTimestamp = true;
		}
	}
	//std::cout << "============End========================" << std::endl;

	if(!extractedTimestamp)  //if the timestamp is not in the image header, set it to the image decoded time stamp
	{
		ostringstream oss;
		oss << time(0);
		_sCurrentImageTimeStamp = oss.str();
		extractedTimestamp = true;
	}

	if(!(extractedContentLength && extractedTimestamp))
	{
		std::cerr << "ZoneMinder:MpegDecoder:extractStreamHeaderData: Error - cannot reach this part of the program." << std::endl;
	}

	return (extractedContentLength && extractedTimestamp);
}

bool MpegDecoder::isStillProcessing()
{
	return _bStillProcessing;
}

MpegDecoder::~MpegDecoder()
{
	delete _pHttpClientPtr; _pHttpClientPtr = NULL;
}

} /* namespace zoneminder */
