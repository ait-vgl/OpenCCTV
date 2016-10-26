/*
 * AnalyticResult.hpp
 *
 *  Created on: Jun 4, 2015
 *      Author: anjana
 */

#ifndef ANALYTICRESULT_HPP_
#define ANALYTICRESULT_HPP_

#include <string> // std::string

namespace opencctv {
namespace dto {

class AnalyticResult {
private:
	unsigned int _iAnalytic_id;
	std::string _sTimestamp;
	std::string _sRresult_text;
	//std::string time;
	//unsigned int count;
public:
	AnalyticResult();
	virtual ~AnalyticResult();

	unsigned int getAnalyticId() const {
		return _iAnalytic_id;
	}

	void setAnalyticId(unsigned int analyticId) {
		_iAnalytic_id = analyticId;
	}

	const std::string& getResultText() const {
		return _sRresult_text;
	}

	void setResultText(const std::string& resultText) {
		_sRresult_text = resultText;
	}

	const std::string& getTimestamp() const {
		return _sTimestamp;
	}

	void setTimestamp(const std::string& timestamp) {
		_sTimestamp = timestamp;
	}
};

} /* namespace dto */
} /* namespace opencctv */

#endif /* ANALYTICRESULT_HPP_ */
