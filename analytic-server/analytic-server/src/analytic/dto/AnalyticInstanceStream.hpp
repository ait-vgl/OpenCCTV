#ifndef ANALYTIC_ANALYTICINSTANCESTREAM_HPP_
#define ANALYTIC_ANALYTICINSTANCESTREAM_HPP_

#include <string>

namespace analytic {
namespace dto {

class AnalyticInstanceStream {
private:
	unsigned int _iStreamId;
	std::string _sInputName;
	std::string _sURL;
	int _iWidth;
	int _iHeight;
	std::string _sUsername;
	std::string _sPassword;
	std::string _sConfig;
	
public:
	AnalyticInstanceStream();
	virtual ~AnalyticInstanceStream();
	void setStreamId(unsigned int iStreamId);
	unsigned int getStreamId() const;
	void setInputName(const std::string& sInputName);
	const std::string& getInputName() const;
	void setURL(const std::string& sURL);
	const std::string& getURL() const;
	
	void setWidth(int iWidth);
	int getWidth() const;
	void setHeight(int iHeight);
	int getHeight() const;
	
	void setUsername(const std::string& sUsername);
	const std::string& getUsername() const;
	void setPassword(const std::string& sPassword);
	const std::string& getPassword() const;
	
	void setConfig(const std::string& sConfig);
	const std::string& getConfig() const;
};

} /* namespace dto */
} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICINSTANCESTREAM_HPP_ */
