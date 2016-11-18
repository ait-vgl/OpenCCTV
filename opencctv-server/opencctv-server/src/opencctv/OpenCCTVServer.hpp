/*
 * OpenCCTVServer.h
 *
 *  Created on: Sep 26, 2016
 *      Author: nhutumdai
 */

#ifndef OPENCCTVSERVER_H_
#define OPENCCTVSERVER_H_

#include "StreamData.hpp"

#include "PluginLoader.hpp"
#include "api/VmsConnector.hpp"

#include "../opencctv/util/log/Loggers.hpp"

namespace opencctv {

class OpenCCTVServer {
	int _iServerId;
	std::map<unsigned int, StreamData*> _mStreams; // StreamId as key, Stream as value
	std::map<unsigned int, PluginLoader<api::VmsConnector>*> _mVMSConnectors; // VMS type id as key

public:
	StreamData* getStreamData(unsigned int iStreamId);
	bool isStream(unsigned int iStreamId);
	void setStream(unsigned int iStreamId, StreamData* pSD);

	bool isVMSConnector(unsigned int iVmsTypeId);
	void setVMSConnector(unsigned int iVMSTypeId, PluginLoader<api::VmsConnector>*);
	PluginLoader<api::VmsConnector>* getVMSConnector(unsigned int iVMSTypeId);

	OpenCCTVServer(int iOpenCCTVServerId);
	virtual ~OpenCCTVServer();

	int getServerId(){
		return _iServerId;
	}
};

} /* namespace opencctv */

#endif /* OPENCCTVSERVER_H_ */
