/*
 * ConfigDetails.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: anjana
 */

#include "ConfigDetails.hpp"

namespace util
{

ConfigDetails *ConfigDetails::_pConfigDetails = NULL;

ConfigDetails::ConfigDetails()
{
	_sHostAddress = "";
	_iHostPort = 0;
	_sOpenCCTVServerAddress = "";
	_iOpenCCTVServerPort = 0;
}

ConfigDetails *ConfigDetails::getInstance()
{
	string sConfigFilename = "analytic-server.config";
	ifstream ifstrConfig(sConfigFilename.c_str());
	if (!ifstrConfig)
	{
		cerr << "ConfigDetails:getInstance: Error: " << "Cannot open Analytic server configuration file"
		<< sConfigFilename << endl;
	}

	set<string> setOptions;
	setOptions.insert("*");
	map<string,string> _mapCofigParameters;

	try
	{
		for (pod::config_file_iterator i(ifstrConfig, setOptions), end; i != end; ++i)
		{
			//cout << i->string_key << " " << i->value[0] << endl;
			_mapCofigParameters[i->string_key] = i->value[0];
		}

		_pConfigDetails = new ConfigDetails;
		_pConfigDetails->_sHostAddress = _mapCofigParameters["host_ip_address"];
		_pConfigDetails->_iHostPort = boost::lexical_cast<int>(_mapCofigParameters["host_port"]);
		_pConfigDetails->_sOpenCCTVServerAddress = _mapCofigParameters["opencctv_server_address"];
		_pConfigDetails->_iOpenCCTVServerPort = boost::lexical_cast<int>(_mapCofigParameters["opencctv_server_port"]);

	} catch (exception& e)
	{
		cerr << "ConfigDetails:getInstance:Exception: " << e.what() << endl;
	}

	return _pConfigDetails;
}

int ConfigDetails::getOpenCctvServerPort() const {
	return _iOpenCCTVServerPort;
}

int ConfigDetails::getHostPort() const {
	return _iHostPort;
}

const string& ConfigDetails::getHostAddress() const {
	return _sHostAddress;
}

const string& ConfigDetails::getOpenCctvServerAddress() const {
	return _sOpenCCTVServerAddress;
}

ConfigDetails::~ConfigDetails()
{
	delete _pConfigDetails; _pConfigDetails = NULL;
}

} /* namespace util */
