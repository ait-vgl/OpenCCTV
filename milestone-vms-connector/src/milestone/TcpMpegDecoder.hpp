/*
 * MpegDecoder.hpp
 *
 *  Created on: Sep 28, 2014
 *      Author: lakindu
 */

#ifndef MPEGDECODER_HPP_
#define MPEGDECODER_HPP_

#include <vector>
//#include "../tcpsocket/TcpClient.hpp"
#include "../tcpsocket/Tcpstream.hpp"
#include "../opencctv/ConcurrentQueue.hpp"
#include "../opencctv/Image.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace opencctv;

namespace milestone
{
class TcpMpegDecoder
{
private:
	vector<unsigned char> currentImageData;
	unsigned int currentContentLength;
	string currentImageTimeStamp;
	TCPStream* tcpClientPtr;
	bool decodeEnable;
	bool stillProcessing;
	string CONT_LEN_HEADER;
	string CUR_TIME_HEADER;
	unsigned int decodedImageCount;
	void extractStreamHeaderData(const string& streamHeaders);
public:
	TcpMpegDecoder(TCPStream* tcpClientPtr);
	void startDecoding(ConcurrentQueue<Image>* pQueue);
	void stopDecoding();
	~TcpMpegDecoder();
	bool isStillProcessing();
};
} /* namespace milestone */

#endif /* MPEGDECODER_HPP_ */
