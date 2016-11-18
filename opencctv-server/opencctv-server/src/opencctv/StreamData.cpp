/*
 * StreamData.cpp
 *
 *  Created on: Sep 26, 2016
 *      Author: nhutumdai
 */

#include "StreamData.hpp"

namespace opencctv {

StreamData::StreamData(int iId) {
	this->_iId = iId;
	this->_pQueue = NULL;
	this->_pConsumerThread = NULL;
	this->_pProducerThread = NULL;
	this->_iVMSConnectorId = 0;
}

bool StreamData::isInternalQueue(){

	if(_pQueue)
		return true;
	else
		return false;

}

StreamData::~StreamData() {
	// TODO Auto-generated destructor stub
}

} /* namespace opencctv */
