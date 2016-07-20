/*
 * MpegDecoder.hpp
 *
 *  Created on: Jun 13, 2015
 *      Author: anjana
 */

#ifndef MPEGDECODER_HPP_
#define MPEGDECODER_HPP_

#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include "../opencctv/ConcurrentQueue.hpp"
#include "../opencctv/Image.hpp"
#include "ZoneminderMessage.hpp"
#include "HttpClient.hpp"

namespace zoneminder {

class MpegDecoder {
private:
	vector<unsigned char> _vCurrentImageData;
	unsigned int _iCurrentContentLength;
	string _sCurrentImageTimeStamp;
	HttpClient *_pHttpClientPtr;
	bool _bDecodeEnable;
	bool _bStillProcessing;
	unsigned int _iDecodedImageCount;

	static const string CONT_LEN_HEADER;
	static const string CUR_TIME_HEADER;

	bool extractStreamHeaderData(const string& streamHeaders);
public:
	MpegDecoder();
	bool init(string sServerName, int iServerPort, string sUrl);
	void startDecoding(opencctv::ConcurrentQueue<opencctv::Image>* pQueuePtr);
	bool isStillProcessing();
	virtual ~MpegDecoder();
};

} /* namespace zoneminder */

#endif /* MPEGDECODER_HPP_ */
