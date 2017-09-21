/*
 * AnalyticInstance.cpp
 *
 *  Created on: May 12, 2017
 *      Author: anjana
 */

#include "AnalyticInstance.hpp"

namespace result {
namespace db {
namespace dto {

AnalyticInstance::AnalyticInstance()
{
	//_iOpenCCTVId = 0;
	_iAnalyticInstId = 0;
}

const std::string& AnalyticInstance::getDescription() const
{
	return _sDescription;
}

void AnalyticInstance::setDescription(const std::string& description)
{
	_sDescription = description;
}

const std::string& AnalyticInstance::getName() const
{
	return _sName;
}

void AnalyticInstance::setName(const std::string& name)
{
	_sName = name;
}

unsigned int AnalyticInstance::getAnalyticInstId() const
{
	return _iAnalyticInstId;
}

void AnalyticInstance::setAnalyticInstId(unsigned int analyticInstId)
{
	_iAnalyticInstId = analyticInstId;
}

/*unsigned int AnalyticInstance::getOpenCctvId() const
{
	return _iOpenCCTVId;
}

void AnalyticInstance::setOpenCctvId(unsigned int openCctvId)
{
	_iOpenCCTVId = openCctvId;
}

const std::string& AnalyticInstance::getLocation() const
{
	return _sLocation;
}

void AnalyticInstance::setLocation(const std::string& location)
{
	_sLocation = location;
}*/

AnalyticInstance::~AnalyticInstance()
{
}

} /* namespace dto */
} /* namespace db */
} /* namespace result */
