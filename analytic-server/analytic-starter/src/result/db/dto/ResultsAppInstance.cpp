/*
 * ResultsAppInstance.cpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#include "ResultsAppInstance.hpp"

namespace result {
namespace db {
namespace dto {

ResultsAppInstance::ResultsAppInstance()
{
	_iResultsAppConnectorId = 0;
	_iResultsAppInstanceId = 0;

}

ResultsAppInstance::~ResultsAppInstance()
{
}

unsigned int ResultsAppInstance::getResultsAppConnectorId() const
{
	return _iResultsAppConnectorId;
}

void ResultsAppInstance::setResultsAppConnectorId(unsigned int resultsAppConnectorId)
{
	_iResultsAppConnectorId = resultsAppConnectorId;
}

unsigned int ResultsAppInstance::getResultsAppInstanceId() const
{
	return _iResultsAppInstanceId;
}

void ResultsAppInstance::setResultsAppInstanceId(unsigned int resultsAppInstanceId)
{
	_iResultsAppInstanceId = resultsAppInstanceId;
}

/*const std::string& ResultsAppInstance::getConfigFilePath() const
{
	return _sConfigFilePath;
}

void ResultsAppInstance::setConfigFilePath(const std::string& configFilePath)
{
	_sConfigFilePath = configFilePath;
}*/

const std::string& ResultsAppInstance::getLibFilePath() const
{
	return _sLibFilePath;
}

void ResultsAppInstance::setLibFilePath(const std::string& libFilePath)
{
	_sLibFilePath = libFilePath;
}

const std::string& ResultsAppInstance::getResultsAppConnectorName() const {
	return _sResultsAppConnectorName;
}

void ResultsAppInstance::setResultsAppConnectorName(
		const std::string& resultsAppConnectorName) {
	_sResultsAppConnectorName = resultsAppConnectorName;
}

const std::string& ResultsAppInstance::getResultsAppInstanceName() const {
	return _sResultsAppInstanceName;
}

void ResultsAppInstance::setResultsAppInstanceName(const std::string& resultsAppInstanceName) {
	_sResultsAppInstanceName = resultsAppInstanceName;
}

} /* namespace dto */
} /* namespace db */
} /* namespace result */
