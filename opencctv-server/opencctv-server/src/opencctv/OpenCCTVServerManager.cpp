/*
 * OpenCCTVServerManager.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: nhutumdai
 */

#include "OpenCCTVServerManager.hpp"

namespace opencctv {

OpenCCTVServerManager* OpenCCTVServerManager::_pOSM = NULL;

OpenCCTVServerManager* OpenCCTVServerManager::getInstance() {
	if (!_pOSM) {
		_pOSM = new OpenCCTVServerManager();
	}
	return _pOSM;
}

OpenCCTVServerManager::OpenCCTVServerManager() {
	//_mOpenCCTVServers.clear();

}

OpenCCTVServer* OpenCCTVServerManager::getOpenCCTVServer(int OpenCCTVServerId) {
	if (_mOpenCCTVServers.count(OpenCCTVServerId) > 0) {
		return _mOpenCCTVServers[OpenCCTVServerId];
	} else {
		return NULL;
	}
}

const map<unsigned int, OpenCCTVServer*>& OpenCCTVServerManager::getAllOpenCCTVServer() {
	return _mOpenCCTVServers;
}

void OpenCCTVServerManager::setOpenCCTVServer(int OpenCCTVServerId, OpenCCTVServer* pOpenCCTVServer) {
	if (!(_mOpenCCTVServers.count(OpenCCTVServerId) > 0)) {
		_mOpenCCTVServers[OpenCCTVServerId] = pOpenCCTVServer;
	}
}

OpenCCTVServerManager::~OpenCCTVServerManager() {
	if(_pOSM)
		delete _pOSM;
}

} /* namespace opencctv */
