/*
 * ConfigDetails.hpp
 *
 *  Created on: Jan 23, 2015
 *      Author: anjana
 */

#ifndef CONFIGDETAILS_HPP_
#define CONFIGDETAILS_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/lexical_cast.hpp>

namespace util
{

using namespace std;
namespace pod = boost::program_options::detail;

class ConfigDetails
{
private:

	string _sAnalyticSererAddress;
	int _iAnalyticServerPort;
	static ConfigDetails *_pConfigDetails;

	ConfigDetails();
	ConfigDetails(ConfigDetails const&);
	void operator=(ConfigDetails const&);

public:
	static ConfigDetails *getInstance();
	int getAnalyticServerPort() const;
	const string& getAnalyticSererAddress() const;

	virtual ~ConfigDetails();
};

} /* namespace util */

#endif /* CONFIGDETAILS_HPP_ */
