/*
 * MpegDecoder.cpp
 *
 *  Created on: Sep 28, 2014
 *      Author: lakindu
 */

#include "TcpMpegDecoder.hpp"

namespace milestone
{

TcpMpegDecoder::TcpMpegDecoder(TCPStream* tcpClientPtr)
{
	currentContentLength = 0;
	currentImageTimeStamp = "";
	this->tcpClientPtr = tcpClientPtr;
	decodeEnable = false;
	stillProcessing = false;
	decodedImageCount = 0;
	CONT_LEN_HEADER = "Content-length: ";
	CUR_TIME_HEADER = "Current: ";
}

void TcpMpegDecoder::startDecoding(ConcurrentQueue<Image>* pQueue)
{
	unsigned char* prevBytePtr = NULL;
	unsigned char* curBytePtr = NULL;
	char readBytes[2048];
	decodeEnable = true;
	stillProcessing = true;
	while(decodeEnable)
	{
		size_t length = (*tcpClientPtr).receive(readBytes, sizeof(readBytes));
		ostringstream strOtherThanImageData;
		size_t i = 0;
		while(i < length)
		{
			curBytePtr = ((unsigned char*) &readBytes[0]) + i;
			if (prevBytePtr != NULL && (*prevBytePtr) == 255 && (*curBytePtr) == 216) //start of jpeg 0xFF 0xD8
			{
				string metaDataStr = strOtherThanImageData.str();
				extractStreamHeaderData(metaDataStr);
				if(currentContentLength > (length - i))
				{
					currentImageData.insert(currentImageData.end(), prevBytePtr, (prevBytePtr + length - i + 1));
					i = length;
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
					if((length - i) <= (currentContentLength - currentImageData.size()))
					{
						currentImageData.insert(currentImageData.end(), curBytePtr, (curBytePtr + (length - i - 1 + 1)));
						i = length;
					}
					else
					{
						currentImageData.insert(currentImageData.end(), curBytePtr, (curBytePtr + (currentContentLength - currentImageData.size() - 1 + 1)));
						i += (currentContentLength - currentImageData.size());
					}
				}
				else if(currentImageData.size() == currentContentLength)
				{
					if(currentImageData[currentContentLength - 2] == 255 && currentImageData[currentContentLength - 1] == 217) //end of jpeg 0xFF 0xD9
					{
						//got jpeg
						decodedImageCount++;
						// Image image(stream.getWidth(), stream.getHeight(), currentImageData, stream.getId(), currentImageTimeStamp, "");
						Image* pImage = new Image();
						pImage->setImageData(currentImageData);
						pImage->setTimestamp(currentImageTimeStamp);
						pQueue->push(pImage);
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
				//cerr << "TcpMpegDecoder:startDecoding: Error - cannot reach this part of the program." << std::endl;
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
