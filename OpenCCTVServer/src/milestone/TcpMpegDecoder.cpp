/*
 * MpegDecoder.cpp
 *
 *  Created on: Sep 28, 2014
 *      Author: lakindu
 */

#include "TcpMpegDecoder.hpp"

namespace milestone
{

TcpMpegDecoder::TcpMpegDecoder(TcpClient* tcpClientPtr)
{
	currentContentLength = 0;
	currentImageTimeStamp = "";
	this->tcpClientPtr = tcpClientPtr;
	decodeEnable = false;
	stillProcessing = false;
	decodedImageCount = 0;
}

void TcpMpegDecoder::startDecoding(ThreadSafeQueue<Image>& queuePtr, Stream &stream, unsigned int imageCount)
{
	char* prevBytePtr = NULL;
	char* curBytePtr = NULL;
	decodeEnable = true;
	stillProcessing = true;
	while(decodeEnable)
	{
		if(decodedImageCount >= imageCount)
		{
			stillProcessing = false;
			break;
		}
		ReceivedDataStruct* dataStructPtr = (*tcpClientPtr).readBytes();
		ostringstream strOtherThanImageData;
		size_t i = 0;
		while(i < (*dataStructPtr).length)
		{
			curBytePtr = (*dataStructPtr).dataPtr + i;
			if (prevBytePtr != NULL && (*prevBytePtr) == -1 && (*curBytePtr) == -40) //start of jpeg
			{
				string metaDataStr = strOtherThanImageData.str();
				extractStreamHeaderData(metaDataStr);
				if(currentContentLength > ((*dataStructPtr).length - i))
				{
					currentImageData.insert(currentImageData.end(), prevBytePtr, (prevBytePtr + (*dataStructPtr).length - i + 1));
					i = (*dataStructPtr).length;
				}
				else
				{
					currentImageData.insert(currentImageData.end(), prevBytePtr, (prevBytePtr + currentContentLength));
					i += currentContentLength - 1;
				}
			}
			else if (currentImageData.size() > 0)
			{
				if(currentImageData.size() < currentContentLength)
				{
					if(((*dataStructPtr).length - i) <= (currentContentLength - currentImageData.size()))
					{
						currentImageData.insert(currentImageData.end(), curBytePtr, (curBytePtr + ((*dataStructPtr).length - i - 1 + 1)));
						i = (*dataStructPtr).length;
					}
					else
					{
						currentImageData.insert(currentImageData.end(), curBytePtr, (curBytePtr + (currentContentLength - currentImageData.size() - 1 + 1)));
						i += (currentContentLength - currentImageData.size());
					}
				}
				else if(currentImageData.size() == currentContentLength)
				{
					if(currentImageData[currentContentLength - 2] == -1 && currentImageData[currentContentLength - 1] == -39) //end of jpeg
					{
						//got jpeg
						decodedImageCount++;
						Image image(stream.getWidth(), stream.getHeight(), currentImageData, stream.getId(), currentImageTimeStamp, "");
						queuePtr.push(image);
						//cout << "TcpMpegDecoder::startDecoding: Produced image from stream " << stream.getId() << " at " << currentImageTimeStamp << endl;
					}
					else //error
					{
						cerr << "TcpMpegDecoder:startDecoding: Error in decoding Mpeg stream." << std::endl;
					}
					currentImageData.clear();
				}
			}
			else if(currentImageData.size() == 0)
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
	boost::split(subStrs, streamHeaders, boost::is_any_of("\n\r"));
	for (std::vector<std::string>::iterator subStrPtr = subStrs.begin(); subStrPtr != subStrs.end(); ++subStrPtr)
	{
		if((*subStrPtr).length() > CUR_TIME_HEADER.length())
		{
			size_t strPos;
			if((strPos = (*subStrPtr).find(CONT_LEN_HEADER)) != string::npos)
			{
				string dataStr;
				dataStr = (*subStrPtr).substr((strPos + CONT_LEN_HEADER.length()), ((*subStrPtr).length() - CONT_LEN_HEADER.length()));
				currentContentLength= boost::lexical_cast<unsigned int>(dataStr);
				extractedContentLength = true;
			}
			else if((strPos = (*subStrPtr).find(CUR_TIME_HEADER)) != string::npos)
			{
				currentImageTimeStamp = (*subStrPtr).substr((strPos + CUR_TIME_HEADER.length()), ((*subStrPtr).length() - CUR_TIME_HEADER.length()));
				extractedTimestamp = true;
			}
		}
	}
	if(!(extractedContentLength && extractedTimestamp))
	{
		std::cerr << "TcpMpegDecoder:extractStreamHeaderData: Error - cannot reach this part of the program." << std::endl;
	}
}

bool TcpMpegDecoder::isStillProcessing()
{
	return stillProcessing;
}

TcpMpegDecoder::~TcpMpegDecoder()
{
	//delete tcpClientPtr;
}
} /* namespace milestone */
