
#ifndef ANALYTIC_CONFIG_HPP_
#define ANALYTIC_CONFIG_HPP_

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/lexical_cast.hpp>
#include "../../opencctv/Exception.hpp"

namespace pod = boost::program_options::detail;

namespace analytic {
namespace util {

const std::string PROPERTY_STARTER_PORT = "analytic_starter_port";
const std::string PROPERTY_ANALYTIC_RUNNER_DIR = "analytic_server_directory";
const std::string PROPERTY_ANALYTIC_RUNNER_FILENAME = "analytic_server_filename";
const std::string PROPERTY_NUM_OF_ANALYTICS = "number_of_analytics";

const std::string PROPERTY_RESULTS_DB_SERVER = "results_db_server_name";
const std::string PROPERTY_RESULTS_DB_PORT = "results_db_port";
const std::string PROPERTY_RESULTS_DB_USERNAME = "results_db_username";
const std::string PROPERTY_RESULTS_DB_PASSWORD = "results_db_password";
const std::string PROPERTY_RESULTS_DB_NAME = "results_db_name";

class Config {
private:
	static Config* _pConfig;
	std::map<std::string, std::string> _mConfigDetails;
	Config();
public:
	static Config* getInstance();
	std::string get(const std::string& sPropertyName);
	virtual ~Config();
};

} /* namespace util */
} /* namespace analytic */

#endif /* ANALYTIC_CONFIG_HPP_ */
