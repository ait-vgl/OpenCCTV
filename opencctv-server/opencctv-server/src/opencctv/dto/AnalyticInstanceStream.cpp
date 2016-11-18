#include "AnalyticInstanceStream.hpp"

namespace opencctv {
namespace dto {

AnalyticInstanceStream::AnalyticInstanceStream() {
	_iId = 0;
	_iAnalyticInstanceId = 0;
}

void AnalyticInstanceStream::setId(unsigned int iId) {
	_iId = iId;
}

unsigned int AnalyticInstanceStream::getId() const {
	return _iId;
}

/*
unsigned int AnalyticInstanceStream::getStreamId() const{
	return _iStreamId;
}

void AnalyticInstanceStream::setStreamId(unsigned int iStreamId){
	_iStreamId = iStreamId;
}
*/

void AnalyticInstanceStream::setAnalyticInstanceId(unsigned int iId) {
	_iAnalyticInstanceId = iId;
}

unsigned int AnalyticInstanceStream::getAnalyticInstanceId() const {
	return _iAnalyticInstanceId;
}

void AnalyticInstanceStream::setInputName(const std::string& sInputName) {
	_sInputName = sInputName;
}

std::string AnalyticInstanceStream::getInputName() const {
	return _sInputName;
}

void AnalyticInstanceStream::setAnalyticDirLocation(const std::string& sLocation) {
	_sAnalyticDirLocation = sLocation;
}

const std::string& AnalyticInstanceStream::getAnalyticDirLocation() const {
	return _sAnalyticDirLocation;
}

void AnalyticInstanceStream::setAnalyticFilename(const std::string& sFilename)
{
	_sAnalyticFilename = sFilename;
}

const std::string& AnalyticInstanceStream::getAnalyticFilename() const
{
	return _sAnalyticFilename;
}

AnalyticInstanceStream::~AnalyticInstanceStream() {
}

} /* namespace dto */
} /* namespace opencctv */
