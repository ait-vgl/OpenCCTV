/*
 * TcpMpegDecoder.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#include "TcpMpegDecoder.hpp"

namespace zoneminder {

TcpMpegDecoder::TcpMpegDecoder(HttpClient* pHttpClientPtr)
{
	_iCurrentContentLength = 0;
	_sCurrentImageTimeStamp = "";
	this->_pHttpClientPtr = pHttpClientPtr;
	_bDecodeEnable = false;
	_bStillProcessing = false;
	_iDecodedImageCount = 0;
}

void TcpMpegDecoder::startDecoding(ThreadSafeQueue<Image>& queuePtr, Stream &stream, unsigned int imageCount)
{
	char* prevBytePtr = NULL;
	char* curBytePtr = NULL;
	_bDecodeEnable = true;
	_bStillProcessing = true;

	while(_bDecodeEnable)
	{
		if(_iDecodedImageCount >= imageCount)
		{
			_bStillProcessing = false;
			break;
		}
		ReceivedDataStruct* dataStructPtr = (*_pHttpClientPtr).readBytes();

		ostringstream strOtherThanImageData;
		size_t i = 0;
		while(i < (*dataStructPtr).length)
		{
			curBytePtr = (*dataStructPtr).dataPtr + i;
			if (prevBytePtr != NULL && (*prevBytePtr) == -1 && (*curBytePtr) == -40) //start of jpeg
			{
				string metaDataStr = strOtherThanImageData.str();
				extractStreamHeaderData(metaDataStr);

				if(_iCurrentContentLength > ((*dataStructPtr).length - i))
				{
					_vCurrentImageData.insert(_vCurrentImageData.end(), prevBytePtr, (prevBytePtr + (*dataStructPtr).length - i + 1));
					i = (*dataStructPtr).length;
				}
				else
				{
					_vCurrentImageData.insert(_vCurrentImageData.end(), prevBytePtr, (prevBytePtr + _iCurrentContentLength));
					i += _iCurrentContentLength - 1;
				}
			}
			else if (_vCurrentImageData.size() > 0)
			{
				if(_vCurrentImageData.size() < _iCurrentContentLength)
				{
					if(((*dataStructPtr).length - i) <= (_iCurrentContentLength - _vCurrentImageData.size()))
					{
						_vCurrentImageData.insert(_vCurrentImageData.end(), curBytePtr, (curBytePtr + ((*dataStructPtr).length - i - 1 + 1)));
						i = (*dataStructPtr).length;
					}
					else
					{
						_vCurrentImageData.insert(_vCurrentImageData.end(), curBytePtr, (curBytePtr + (_iCurrentContentLength - _vCurrentImageData.size() - 1 + 1)));
						i += (_iCurrentContentLength - _vCurrentImageData.size());
					}
				}
				else if(_vCurrentImageData.size() == _iCurrentContentLength)
				{
					if(_vCurrentImageData[_iCurrentContentLength - 2] == -1 && _vCurrentImageData[_iCurrentContentLength - 1] == -39) //end of jpeg
					{
						//got jpeg
						_iDecodedImageCount++;
						Image image(stream.getWidth(), stream.getHeight(), _vCurrentImageData, stream.getId(), _sCurrentImageTimeStamp, "");
						queuePtr.push(image);
						//cout << "TcpMpegDecoder::startDecoding: Produced image from stream " << stream.getId() << " at " << _sCurrentImageTimeStamp << endl;
					}
					else //error
					{
						cerr << "TcpMpegDecoder:startDecoding: Error in decoding Mpeg stream." << std::endl;
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
				cerr << "TcpMpegDecoder:startDecoding: Error - cannot reach this part of the program." << std::endl;
			}
			prevBytePtr = curBytePtr;
		}
	}
}

void TcpMpegDecoder::extractStreamHeaderData(const string& streamHeaders)
{
	bool extractedContentLength = false;
	bool extractedTimestamp = false;
	vector<string> subStrs;
	size_t strPos;
	boost::split(subStrs, streamHeaders, boost::is_any_of("\n\r"));
	for (std::vector<std::string>::iterator subStrPtr = subStrs.begin(); subStrPtr != subStrs.end(); ++subStrPtr)
	{
		//TODO : substr can be simplified???
		if((strPos = (*subStrPtr).find(CONT_LEN_HEADER)) != string::npos)
		{
			string dataStr;
			dataStr = (*subStrPtr).substr((strPos + CONT_LEN_HEADER.length()), ((*subStrPtr).length() - CONT_LEN_HEADER.length()));
			_iCurrentContentLength= boost::lexical_cast<unsigned int>(dataStr);
			extractedContentLength = true;
		}

		if((strPos = (*subStrPtr).find(CUR_TIME_HEADER)) != string::npos)
		{
			_sCurrentImageTimeStamp = (*subStrPtr).substr((strPos + CUR_TIME_HEADER.length()), ((*subStrPtr).length() - CUR_TIME_HEADER.length()));
			extractedTimestamp = true;
		}
		else //if the time stamp is not in the image header, set it to the image decoded time stamp
		{
			ostringstream oss;
			oss << time(0);
			_sCurrentImageTimeStamp = oss.str();
			extractedTimestamp = true;
		}
	}
	if(!(extractedContentLength && extractedTimestamp))
	{
		std::cerr << "TcpMpegDecoder:extractStreamHeaderData: Error - cannot reach this part of the program." << std::endl;
	}
}

bool TcpMpegDecoder::isStillProcessing()
{
	return _bStillProcessing;
}

TcpMpegDecoder::~TcpMpegDecoder()
{
}

} /* namespace zoneminder */
