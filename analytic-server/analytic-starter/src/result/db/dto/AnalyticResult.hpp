/*
 * AnalyticResult.hpp
 *
 *  Created on: May 12, 2017
 *      Author: anjana
 */

#ifndef ANALYTICRESULT_HPP_
#define ANALYTICRESULT_HPP_

#include <string>

namespace result {
namespace db {
namespace dto {

class AnalyticResult {
private:
	unsigned int _iResultsId;
	unsigned int _iAnalyticInstId;
	std::string _sTimestamp;
	std::string _sResult;

public:
	AnalyticResult();
	virtual ~AnalyticResult();
	unsigned int getAnalyticInstId() const;
	void setAnalyticInstId(unsigned int analyticInstId);
	unsigned int getResultsId() const;
	void setResultsId(unsigned int resultsId);
	const std::string& getResult() const ;
	void setResult(const std::string& result);
	const std::string& getTimestamp() const;
	void setTimestamp(const std::string& timestamp);
};

} /* namespace dto */
} /* namespace db */
} /* namespace result */

#endif /* ANALYTICRESULT_HPP_ */
