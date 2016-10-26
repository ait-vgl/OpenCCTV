#include "Config.hpp"

namespace util {

Config* Config::_pConfig = NULL;

Config::Config(const std::string& sFilePath) {
	std::ifstream ifstrConfig(sFilePath.c_str());
	if (!ifstrConfig) {
		throw std::runtime_error("Cannot open Analytic server configuration file.");
	}
	std::set<std::string> setOptions;
	setOptions.insert("*");
	try {
		for (pod::config_file_iterator i(ifstrConfig, setOptions), end; i != end; ++i) {
			_mConfigDetails[i->string_key] = i->value[0];
		}
	} catch (std::exception& e) {
		std::string sErrMsg = "Failed to extract configuration details from Analytic server configuration file. ";
		sErrMsg.append(e.what());
		throw std::runtime_error(sErrMsg);
	}
}

Config* Config::getInstance(const std::string& sFilePath)
{
	if(!_pConfig)
	{
		_pConfig = new Config(sFilePath);
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
