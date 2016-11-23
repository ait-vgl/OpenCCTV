
/*
 * AnalyticServerManager.h
 *
 *  Created on: Sep 26, 2016
 *      Author: nhutumdai
 */

#ifndef ANALYTICSERVERMANAGER_H_
#define ANALYTICSERVERMANAGER_H_

#include "AnalyticServer.hpp"

using namespace std;

namespace analytic {

class AnalyticServerManager {

	AnalyticServerManager();
	static AnalyticServerManager* _pASM; // Analytic Server Manager
	map<unsigned int, AnalyticServer*> _mAnalyticServers; // Analytic server id as key

public:
	static AnalyticServerManager* getInstance();

	AnalyticServer* getAnalyticServer(int iAnalyticServerId);
    //AnalyticServer* getAnalyticServerByAIId(unsigned int iAnalyticInstanceId);
	const map<unsigned int, AnalyticServer*>& getAllAnalyticServer();
	void setAnalyticServer(int iAnalyticServerId, AnalyticServer* pAnalyticServer);

	virtual ~AnalyticServerManager();
};

} /* namespace analytic */

#endif /* ANALYTICSERVERMANAGER_H_ */
