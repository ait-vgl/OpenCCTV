#include "Config.hpp"

namespace opencctv {
namespace util {

Config* Config::_pConfig = NULL;

Config::Config() {
	std::string sConfigFilename = "./opencctv-server-starter.config";
	std::ifstream ifstrConfig(sConfigFilename.c_str());
	if (!ifstrConfig) {
		throw opencctv::Exception("Cannot open OpenCCTV Server starter configuration file.");
	}
	std::set<std::string> setOptions;
	setOptions.insert("*");
	try {
		for (pod::config_file_iterator i(ifstrConfig, setOptions), end; i != end; ++i) {
			_mConfigDetails[i->string_key] = i->value[0];
		}
	} catch (std::exception& e) {
		std::string sErrMsg = "Failed to extract configuration details from OpenCCTV Server starter configuration file. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}

Config* Config::getInstance()
{
	if(!_pConfig)
	{
		_pConfig = new Config();
	}
	return _pConfig;
}

std::string Config::get(const std::string& sPropertyName)
{
	std::string sRet;
	std::map<std::string, std::string>::iterator it = _mConfigDetails.find(sPropertyName);
	if (it != _mConfigDetails.end())
	{
		sRet = it->second;
	}
	return sRet;
}

Config::~Config() {
	if(_pConfig)
	{
		delete _pConfig;
		_pConfig = NULL;
	}
}

} /* namespace util */
} /* namespace opencctv */
