#include "AnalyticInstance.hpp"

namespace opencctv {
namespace dto {

AnalyticInstance::AnalyticInstance() {
	_iId = 0;
}

void AnalyticInstance::setId(unsigned int iId) {
	_iId = iId;
}

unsigned int AnalyticInstance::getId() const {
	return _iId;
}

void AnalyticInstance::setAnalyticDirLocation(const std::string& sLocation) {
	_sAnalyticDirLocation = sLocation;
}

const std::string& AnalyticInstance::getAnalyticDirLocation() const {
	return _sAnalyticDirLocation;
}

void AnalyticInstance::setAnalyticFilename(const std::string& sFilename)
{
	_sAnalyticFilename = sFilename;
}

const std::string& AnalyticInstance::getAnalyticFilename() const
{
	return _sAnalyticFilename;
}

AnalyticInstance::~AnalyticInstance() {
}

} /* namespace dto */
} /* namespace opencctv */
