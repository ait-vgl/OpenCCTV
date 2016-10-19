
#ifndef ANALYTIC_ANALYTICRESULT_HPP_
#define ANALYTIC_ANALYTICRESULT_HPP_

#include <string>

namespace analytic {

class AnalyticResult {
private:
	unsigned int _iStreamId;
	std::string _sInputName;
	std::string _sTimestamp;
	std::string _sCustomText;
	bool _bWriteToDatabase;

public:
	AnalyticResult();
	AnalyticResult(unsigned int iStreamId, const std::string& sInputName, std::string sTimestamp, std::string sCustomText, bool bWriteToDatabase);
	void setStreamId(unsigned int iStreamId);
	unsigned int getStreamId() const;
	void setInputName(const std::string& sInputName);
	const std::string& getInputName() const;
	void setTimestamp(const std::string& sTimestamp);
	const std::string& getTimestamp() const;
	void setCustomText(const std::string& sCustomText);
	const std::string& getCustomText() const;
	void setWriteToDatabase(bool bWriteToDatabase);
	bool getWriteToDatabase() const;
	virtual ~AnalyticResult();
};

} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICRESULT_HPP_ */
