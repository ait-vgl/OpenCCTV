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
	_sAnalyticSererAddress = "";
	_iAnalyticServerPort = 0;
}

ConfigDetails *ConfigDetails::getInstance()
{
	string sConfigFilename = "opencctv-server.config";
	ifstream ifstrConfig(sConfigFilename.c_str());
	if (!ifstrConfig)
	{
		cerr << "OpenCCTVServer:getInstance: Error: " << "Cannot open OpenCCTV server configuration file"
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
		_pConfigDetails->_iAnalyticServerPort = boost::lexical_cast<int>(_mapCofigParameters["analytic_server_port"]);
		_pConfigDetails->_sAnalyticSererAddress = _mapCofigParameters["analytic_server_address"];

	} catch (exception& e)
	{
		cerr << "OpenCCTVServer:getInstance:Exception: " << e.what() << endl;
	}

	return _pConfigDetails;
}

int ConfigDetails::getAnalyticServerPort() const
{
		return _iAnalyticServerPort;
}

const string& ConfigDetails::getAnalyticSererAddress() const
{
	return _sAnalyticSererAddress;
}

ConfigDetails::~ConfigDetails()
{
	delete _pConfigDetails; _pConfigDetails = NULL;
}

} /* namespace util */
