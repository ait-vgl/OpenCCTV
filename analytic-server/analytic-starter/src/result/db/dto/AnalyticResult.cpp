/*
 * AnalyticResult.cpp
 *
 *  Created on: May 12, 2017
 *      Author: anjana
 */

#include "AnalyticResult.hpp"

namespace result {
namespace db {
namespace dto {

AnalyticResult::AnalyticResult()
{
	_iAnalyticInstId = 0;
	_iResultsId = 0;

}

unsigned int AnalyticResult::getAnalyticInstId() const
{
	return _iAnalyticInstId;
}

void AnalyticResult::setAnalyticInstId(unsigned int analyticInstId)
{
	_iAnalyticInstId = analyticInstId;
}

unsigned int AnalyticResult::getResultsId() const
{
	return _iResultsId;
}

void AnalyticResult::setResultsId(unsigned int resultsId)
{
	_iResultsId = resultsId;
}

const std::string& AnalyticResult::getResult() const
{
	return _sResult;
}

void AnalyticResult::setResult(const std::string& result)
{
	_sResult = result;
}

const std::string& AnalyticResult::getTimestamp() const
{
	return _sTimestamp;
}

void AnalyticResult::setTimestamp(const std::string& timestamp)
{
	_sTimestamp = timestamp;
}

AnalyticResult::~AnalyticResult()
{
	// TODO Auto-generated destructor stub
}

} /* namespace dto */
} /* namespace db */
} /* namespace result */
