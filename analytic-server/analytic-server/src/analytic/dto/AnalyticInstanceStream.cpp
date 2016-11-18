#include "AnalyticInstanceStream.hpp"

namespace analytic {
namespace dto {

AnalyticInstanceStream::AnalyticInstanceStream() {
    _iStreamId = 0;
    _sInputName = "";
    _sURL = "";
    _iWidth = 0;
    _iHeight = 0;
    _sUsername = "";
    _sPassword = "";
    _sConfig = "";
}

void AnalyticInstanceStream::setStreamId(unsigned int iStreamId) {
    _iStreamId = iStreamId;
}
unsigned int AnalyticInstanceStream::getStreamId() const {
    return _iStreamId;
}

void AnalyticInstanceStream::setInputName(const std::string& sInputName){
    _sInputName = sInputName;
}
const std::string& AnalyticInstanceStream::getInputName() const {
    return _sInputName;
}

void AnalyticInstanceStream::setURL(const std::string& sURL) {
    _sURL = sURL;
}
const std::string& AnalyticInstanceStream::getURL() const {
    return _sURL;
}
	
void AnalyticInstanceStream::setWidth(int iWidth) {
  _iWidth = iWidth;
}
int AnalyticInstanceStream::getWidth() const {
    return _iWidth;
}

void AnalyticInstanceStream::setHeight(int iHeight) {
    _iHeight = iHeight;
}
int AnalyticInstanceStream::getHeight() const {
    return _iHeight;
}
	
void AnalyticInstanceStream::setUsername(const std::string& sUsername){
    _sUsername = sUsername;
}
const std::string& AnalyticInstanceStream::getUsername() const{
    return _sUsername;
}

void AnalyticInstanceStream::setPassword(const std::string& sPassword){
    _sPassword = sPassword;
}
const std::string& AnalyticInstanceStream::getPassword() const{
    return _sPassword;
}	

void AnalyticInstanceStream::setConfig(const std::string& sConfig){
    _sConfig = sConfig;
}
const std::string& AnalyticInstanceStream::getConfig() const{
    return _sConfig;
}

AnalyticInstanceStream::~AnalyticInstanceStream() {
}

} /* namespace dto */
} /* namespace analytic */
