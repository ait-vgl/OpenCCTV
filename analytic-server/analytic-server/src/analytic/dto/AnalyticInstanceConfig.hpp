#ifndef OPENCCTV_STREAM_HPP_
#define OPENCCTV_STREAM_HPP_

#include <string> // std::string

namespace analytic {
namespace dto {

class AnalyticInstanceConfig {
private:
	/* AnalayticInstanceConfig fields */
	unsigned int _iId;
	std::string _sFileName;
	std::string _sData;

public:
	AnalyticInstanceConfig();
	virtual ~AnalyticInstanceConfig();
	/* getters and setter */
	void setId(unsigned int iId);
	unsigned int getId() const;
	
	void setFileName(const std::string& sFileName);
	std::string getFileName() const;
	
	void setData(const std::string& sData);
	std::string getData() const;
};

} /* namespace dto */
} /* namespace analytic */

#endif /* OPENCCTV_STREAM_HPP_ */
