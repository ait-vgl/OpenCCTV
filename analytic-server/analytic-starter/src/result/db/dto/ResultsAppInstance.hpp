/*
 * ResultsAppInstance.hpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#ifndef RESULTSAPPINSTANCE_HPP_
#define RESULTSAPPINSTANCE_HPP_

#include <string>

namespace result {
namespace db {
namespace dto {

class ResultsAppInstance {
private:
	unsigned int _iResultsAppInstanceId;
	std::string _sResultsAppInstanceName;
	unsigned int _iResultsAppConnectorId;
	std::string _sResultsAppConnectorName;
	std::string _sLibFilePath;
public:
	ResultsAppInstance();
	virtual ~ResultsAppInstance();

	unsigned int getResultsAppConnectorId() const;
	void setResultsAppConnectorId(unsigned int resultsAppConnectorId);
	unsigned int getResultsAppInstanceId() const;
	void setResultsAppInstanceId(unsigned int resultsAppInstanceId);
	const std::string& getLibFilePath() const ;
	void setLibFilePath(const std::string& libFilePath) ;
	const std::string& getResultsAppConnectorName() const;
	void setResultsAppConnectorName(const std::string& resultsAppConnectorName);
	const std::string& getResultsAppInstanceName() const;
	void setResultsAppInstanceName(const std::string& resultsAppInstanceName);
};

} /* namespace dto */
} /* namespace db */
} /* namespace result */

#endif /* RESULTSAPPINSTANCE_HPP_ */
