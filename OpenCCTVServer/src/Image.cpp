/*
 * Image.cpp
 *
 *  Created on: Oct 9, 2014
 *      Author: lakindu
 */

#include "Image.hpp"

Image::Image()
{
	this->_iWidth = 0;
	this->_iHeight = 0;
	this->_iStreamId = 0;
	this->_sInputName = "";
	this->_sTimestamp = "";
	this->_sResult = "";
}

Image::Image(unsigned int iWidth, unsigned int iHeight, vector<char>& vImageData, unsigned int iStreamId, const string& sTimestamp, const string& sResult)
{
	this->_iWidth = iWidth;
	this->_iHeight = iHeight;
	this->_vImageData = vImageData;
	this->_iStreamId = iStreamId;
	this->_sInputName = "";
	this->_sTimestamp = sTimestamp;
	this->_sResult = sResult;
}

Image::~Image()
{
}

// == Getters and Setters ==

unsigned int Image::getHeight() const {
	return _iHeight;
}

void Image::setHeight(unsigned int height) {
	_iHeight = height;
}

unsigned int Image::getStreamId() const {
	return _iStreamId;
}

void Image::setStreamId(unsigned int streamId) {
	_iStreamId = streamId;
}

unsigned int Image::getWidth() const {
	return _iWidth;
}

void Image::setWidth(unsigned int width) {
	_iWidth = width;
}

const string& Image::getInputName() const {
	return _sInputName;
}

void Image::setInputName(const string& inputName) {
	_sInputName = inputName;
}

const string& Image::getTimestamp() const {
	return _sTimestamp;
}

void Image::setTimestamp(const string& timestamp) {
	_sTimestamp = timestamp;
}

const vector<char>& Image::getImageData() const {
	return _vImageData;
}

void Image::setImageData(const vector<char>& imageData) {
	_vImageData = imageData;
}

const string& Image::getResult() const {
	return _sResult;
}

void Image::setResult(const string& result) {
	_sResult = result;
}




