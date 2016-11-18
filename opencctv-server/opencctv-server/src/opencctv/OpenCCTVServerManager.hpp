/*
 * OpenCCTVServerManager.h
 *
 *  Created on: Sep 26, 2016
 *      Author: nhutumdai
 */


#ifndef OPENCCTVSERVERMANAGER_H_
#define OPENCCTVSERVERMANAGER_H_

//#define NULL   ((void *) 0)


#include "OpenCCTVServer.hpp"

using namespace std;

namespace opencctv {

class OpenCCTVServerManager {

	OpenCCTVServerManager();
	static OpenCCTVServerManager* _pOSM; // Open CCTV Server Manager
	std::map<unsigned int, OpenCCTVServer*> _mOpenCCTVServers; // OpenCCTV server is as key , OpenCCTV server as value

public:
	static OpenCCTVServerManager* getInstance();

	OpenCCTVServer* getOpenCCTVServer(int iOpenCCTVServerId);
	const map<unsigned int, OpenCCTVServer*>& getAllOpenCCTVServer();
	void setOpenCCTVServer(int, OpenCCTVServer*);

	virtual ~OpenCCTVServerManager();
};

} /* namespace opencctv */

#endif /* OPENCCTVSERVERMANAGER_H_ */
