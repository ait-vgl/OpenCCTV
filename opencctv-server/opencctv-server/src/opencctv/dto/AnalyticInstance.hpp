#ifndef OPENCCTV_ANALYTICINSTANCE_HPP_
#define OPENCCTV_ANALYTICINSTANCE_HPP_

#include <string>

namespace opencctv {
namespace dto {

class AnalyticInstance {
private:
	unsigned int _iId;
	std::string _sAnalyticDirLocation;
	std::string _sAnalyticFilename;
public:
	AnalyticInstance();
	virtual ~AnalyticInstance();
	void setId(unsigned int iId);
	unsigned int getId() const;
	void setAnalyticDirLocation(const std::string& sLocation);
	const std::string& getAnalyticDirLocation() const;
	void setAnalyticFilename(const std::string& sFilename);
	const std::string& getAnalyticFilename() const;
};

} /* namespace dto */
} /* namespace opencctv */

#endif /* OPENCCTV_ANALYTICINSTANCE_HPP_ */
