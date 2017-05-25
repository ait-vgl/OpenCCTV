/*
 * AnalyticInstance.hpp
 *
 *  Created on: May 12, 2017
 *      Author: anjana
 */

#ifndef ANALYTICINSTANCE_HPP_
#define ANALYTICINSTANCE_HPP_

#include <string>

namespace result {
namespace db {
namespace dto {

class AnalyticInstance {
private:
	unsigned int _iAnalyticInstId;
	unsigned int _iOpenCCTVId;
	std::string _sName;
	std::string _sDescription;
	std::string _sLocation;

public:
	AnalyticInstance();
	virtual ~AnalyticInstance();
	const std::string& getDescription() const;
	void setDescription(const std::string& description);
	const std::string& getLocation() const;
	void setLocation(const std::string& location);;
	const std::string& getName() const ;
	void setName(const std::string& name);;
	unsigned int getOpenCctvId() const ;;
	void setOpenCctvId(unsigned int openCctvId);
	unsigned int getAnalyticInstId() const ;
	void setAnalyticInstId(unsigned int analyticInstId);
};

} /* namespace dto */
} /* namespace db */
} /* namespace result */

#endif /* ANALYTICINSTANCE_HPP_ */
