
#include "AnalyticResult.hpp"

namespace analytic {

AnalyticResult::AnalyticResult() {
	_iStreamId = 0;
	_bWriteToDatabase = false;
}

AnalyticResult::AnalyticResult(unsigned int iStreamId, const std::string& sInputName, std::string sTimestamp, std::string sCustomText, bool bWriteToDatabase)
{
	_iStreamId = iStreamId;
	_sTimestamp = sTimestamp;
	_sCustomText = sCustomText;
	_bWriteToDatabase = bWriteToDatabase;
}

void AnalyticResult::setStreamId(unsigned int iStreamId)
{
	_iStreamId = iStreamId;
}
unsigned int AnalyticResult::getStreamId() const
{
	return _iStreamId;
}

void AnalyticResult::setInputName(const std::string& sInputName)
{
	_sInputName = sInputName;
}
const std::string& AnalyticResult::getInputName() const
{
	return _sInputName;
}

void AnalyticResult::setTimestamp(const std::string& sTimestamp)
{
	_sTimestamp = sTimestamp;
}

const std::string& AnalyticResult::getTimestamp() const
{
	return _sTimestamp;
}

void AnalyticResult::setCustomText(const std::string& sCustomText)
{
	_sCustomText = sCustomText;
}

const std::string& AnalyticResult::getCustomText() const
{
	return _sCustomText;
}

void AnalyticResult::setWriteToDatabase(bool bWriteToDatabase)
{
	_bWriteToDatabase = bWriteToDatabase;
}

bool AnalyticResult::getWriteToDatabase() const
{
	return _bWriteToDatabase;
}

AnalyticResult::~AnalyticResult() {
}

} /* namespace analytic */
