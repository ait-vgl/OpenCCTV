/*
 * OpenCCTVServer.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: nhutumdai
 */

#include "OpenCCTVServer.hpp"

namespace opencctv {

OpenCCTVServer::OpenCCTVServer(int iOpenCCTVServerId) {

	this->_iServerId = iOpenCCTVServerId;
}

StreamData* OpenCCTVServer::getStreamData(unsigned int iStreamId) {
	if (isStream(iStreamId)) {
		return _mStreams[iStreamId];
	} else
		return NULL;
}

bool OpenCCTVServer::isStream(unsigned int iStreamId) {
	if (_mStreams.count(iStreamId) > 0 and _mStreams[iStreamId])
		return true;
	else
		return false;
}

void OpenCCTVServer::setStream(unsigned int iStreamId, StreamData* pSD) {
	if (!isStream(iStreamId)) {
		_mStreams[iStreamId] = pSD;
		opencctv::util::log::Loggers::getDefaultLogger()->debug("OpenCCTVServer: Set up StreamData: " + boost::lexical_cast<std::string>(iStreamId));
	} else {
		opencctv::util::log::Loggers::getDefaultLogger()->debug("OpenCCTVServer: Skip set up StreamData: " + boost::lexical_cast<std::string>(iStreamId));
	}
}

bool OpenCCTVServer::isVMSConnector(unsigned int iVmsTypeId) {
	if (_mVMSConnectors.count(iVmsTypeId) > 0 and _mVMSConnectors[iVmsTypeId])
		return true;
	else
		return false;
}

void OpenCCTVServer::setVMSConnector(unsigned int iVMSTypeId, PluginLoader<api::VmsConnector>* pVMSConnector) {
	if (!isVMSConnector(iVMSTypeId)) {
		_mVMSConnectors[iVMSTypeId] = pVMSConnector;
	}
}

PluginLoader<api::VmsConnector>* OpenCCTVServer::getVMSConnector(unsigned int iVMSTypeId) {
	if (isVMSConnector(iVMSTypeId)) {
		return _mVMSConnectors[iVMSTypeId];
	} else
		return NULL;
}

OpenCCTVServer::~OpenCCTVServer() {
	// TODO Auto-generated destructor stub
}

} /* namespace opencctv */
