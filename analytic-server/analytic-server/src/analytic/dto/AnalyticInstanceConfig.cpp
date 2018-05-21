
#include "AnalyticInstanceConfig.hpp"

namespace analytic {
namespace dto {

AnalyticInstanceConfig::AnalyticInstanceConfig() {
    _iId = 0;
    _sFileName = "";
    _sData = "";
}

void AnalyticInstanceConfig::setId(unsigned int iId){
    _iId = iId;
}
unsigned int AnalyticInstanceConfig::getId() const{
    return _iId;
}

void AnalyticInstanceConfig::setFileName(const std::string& sFileName){
    _sFileName = sFileName;
}
std::string AnalyticInstanceConfig::getFileName() const{
    return _sFileName;
}

void AnalyticInstanceConfig::setData(const std::string& sData){
    _sData = sData;
}
std::string AnalyticInstanceConfig::getData() const{
    return _sData;	    
}

AnalyticInstanceConfig::~AnalyticInstanceConfig() {
}

} /* namespace dto */
} /* namespace analytic */
