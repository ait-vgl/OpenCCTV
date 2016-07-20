/*
 * ZoneMinderConnector.hpp
 *
 *  Created on: Jun 3, 2015
 *      Author: anjana
 */

#ifndef ZONEMINDERCONNECTOR_HPP_
#define ZONEMINDERCONNECTOR_HPP_

#include "opencctv/api/VmsConnector.hpp"
#include "util/Config.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "zoneminder/MpegDecoder.hpp"

class ZoneMinderConnector: public opencctv::api::VmsConnector {
private:
	/*unsigned long long _lCount;
	std::vector<unsigned char>* _pVImageData;
	util::Config* _pConfig;*/

	//========
	/*unsigned int iImageHeight;
	unsigned int iImageWidth;*/
	std::string sConnectorDirPath;
	zoneminder::MpegDecoder* _pDecoderPtr;
	bool getZMStreamURL(const std::string& sServerName, const int iServerPort, const std::string& sUsername, const std::string& sPassword, const std::string& sCameraId, const std::string sConnectorPath, std::string& sUrl);

public:
	ZoneMinderConnector();
	bool init(const opencctv::api::VmsConnectInfo& info, const std::string sPathToVmsConnectorDir);
	void produceImageObjects(opencctv::ConcurrentQueue<opencctv::Image>* pQueue);
	bool isStillProducingImages();
	virtual ~ZoneMinderConnector();
};

// the class factories
extern "C" ZoneMinderConnector* create() {
	return new ZoneMinderConnector;
}

extern "C" void destroy(ZoneMinderConnector* _pConnector) {
	if (_pConnector) {
		delete _pConnector;
	}
}

#endif /* ZONEMINDERCONNECTOR_HPP_ */
