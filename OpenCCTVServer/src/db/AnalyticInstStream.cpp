/*
 * AnalyticInstStream.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: anjana
 */

#include "AnalyticInstStream.hpp"

namespace db {

AnalyticInstStream::AnalyticInstStream() {
	this->_iId = 0;
	this->_iAnalyticInstanceId = 0;
	this->_iStreamId = 0;
	this->_sInputName = "";
	this->_iAnalyticId = 0;
	this->_sLocation = "";

}

AnalyticInstStream::~AnalyticInstStream() {
	// TODO Auto-generated destructor stub
}

// == Getters and Setters ==

unsigned int AnalyticInstStream::getAnalyticId() const {
	return _iAnalyticId;
}

void AnalyticInstStream::setAnalyticId(unsigned int analyticId) {
	_iAnalyticId = analyticId;
}

unsigned int AnalyticInstStream::getAnalyticInstanceId() const {
	return _iAnalyticInstanceId;
}

void AnalyticInstStream::setAnalyticInstanceId(
		unsigned int analyticInstanceId) {
	_iAnalyticInstanceId = analyticInstanceId;
}

unsigned int AnalyticInstStream::getId() const {
	return _iId;
}

void AnalyticInstStream::setId(unsigned int id) {
	_iId = id;
}

unsigned int AnalyticInstStream::getStreamId() const {
	return _iStreamId;
}

void AnalyticInstStream::setStreamId(unsigned int streamId) {
	_iStreamId = streamId;
}

const string& AnalyticInstStream::getInputName() const {
	return _sInputName;
}

void AnalyticInstStream::setInputName(const string& inputName) {
	_sInputName = inputName;
}

const string& AnalyticInstStream::getLocation() const {
	return _sLocation;
}

void AnalyticInstStream::setLocation(const string& location) {
	_sLocation = location;
}

} /* namespace db */
