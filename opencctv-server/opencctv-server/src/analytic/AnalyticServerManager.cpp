/*
 * AnalyticServerManager.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: nhutumdai
 */

#include "AnalyticServerManager.hpp"

namespace analytic {

AnalyticServerManager* AnalyticServerManager::_pASM = NULL;

AnalyticServerManager::AnalyticServerManager() {
	// TODO Auto-generated constructor stub

}

AnalyticServerManager* AnalyticServerManager::getInstance() {
	if (!_pASM) {
		_pASM = new AnalyticServerManager();
	}
	return _pASM;
}

AnalyticServer* AnalyticServerManager::getAnalyticServer(int iAnalyticServerId) {
	if (_mAnalayticServers.count(iAnalyticServerId) > 0) {
		return _mAnalayticServers[iAnalyticServerId];
	} else {
		return NULL;
	}
}

const map<unsigned int, AnalyticServer*>& AnalyticServerManager::getAllAnalyticServer() {
	return _mAnalayticServers;
}

void AnalyticServerManager::setAnalyticServer(int iAnalyticServerId, AnalyticServer* pAnalyticServer) {
	if (!_mAnalayticServers.count(iAnalyticServerId) > 0) {
		_mAnalayticServers[iAnalyticServerId] = pAnalyticServer;
	}
}

AnalyticServerManager::~AnalyticServerManager() {
	if(_pASM)
		delete _pASM;
}

} /* namespace analytic */
