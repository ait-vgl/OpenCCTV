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
	if (_mAnalyticServers.count(iAnalyticServerId) > 0) {
		return _mAnalyticServers[iAnalyticServerId];
	} else {
		return NULL;
	}
}
/*
AnalyticServer* AnalyticServerManager::getAnalyticServerByAIId(unsigned int iAnalyticInstanceId)
{
	std::map<unsigned int, analytic::AnalyticServer *>::iterator it;

	for (it = _mAnalyticServers.begin(); it != _mAnalyticServers.end(); ++it)
	{
		analytic::AnalyticServer *pAnalyticServer = it->second;

		if (pAnalyticServer->startAllAnalyticInstanceAction())
		{
            if(pAnalyticServer->isAnalyticInstance(iAnalyticInstanceId)) return pAnalyticServer;
		}
	}
	
	return NULL;
}
*/

const map<unsigned int, AnalyticServer*>& AnalyticServerManager::getAllAnalyticServer() {
	return _mAnalyticServers;
}

void AnalyticServerManager::setAnalyticServer(int iAnalyticServerId, AnalyticServer* pAnalyticServer) {
	if (!_mAnalyticServers.count(iAnalyticServerId) > 0) {
		_mAnalyticServers[iAnalyticServerId] = pAnalyticServer;
	}
}

AnalyticServerManager::~AnalyticServerManager() {
	if(_pASM)
		delete _pASM;
}

} /* namespace analytic */
