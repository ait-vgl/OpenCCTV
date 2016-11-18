#ifndef OPENCCTV_ANALYTICINSTANCESTREAM_HPP_
#define OPENCCTV_ANALYTICINSTANCESTREAM_HPP_

#include <string>

namespace opencctv {
namespace dto {

class AnalyticInstanceStream {
private:
	unsigned int _iId;
	unsigned int _iAnalyticInstanceId;
	//unsigned int _iStreamId;
	std::string _sInputName;
	std::string _sAnalyticDirLocation;
	std::string _sAnalyticFilename;
public:
	AnalyticInstanceStream();
	virtual ~AnalyticInstanceStream();
	void setId(unsigned int iId);
	unsigned int getId() const;
	void setAnalyticInstanceId(unsigned int iId);

	//unsigned int getStreamId() const;
	//void setStreamId(unsigned int iStreamId);

	unsigned int getAnalyticInstanceId() const;
	void setInputName(const std::string& sInputName);
	std::string getInputName() const;
	void setAnalyticDirLocation(const std::string& sLocation);
	const std::string& getAnalyticDirLocation() const;
	void setAnalyticFilename(const std::string& sFilename);
	const std::string& getAnalyticFilename() const;
};

} /* namespace dto */
} /* namespace opencctv */

#endif /* OPENCCTV_ANALYTICINSTANCESTREAM_HPP_ */
