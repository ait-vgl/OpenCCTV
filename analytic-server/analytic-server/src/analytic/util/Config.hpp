
#ifndef ANALYTIC_CONFIG_HPP_
#define ANALYTIC_CONFIG_HPP_

#include <map> // std::map
#include <string> // std::string
#include <fstream> // to read files on disk
/* boost includes (/usr/local/include/) */
/* boost library links (/usr/local/lib/) -lboost_system */
#include <boost/config.hpp>
// boost library link for boost program options (/usr/local/lib/) -lboost_program_options
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include "../../opencctv/Exception.hpp"

namespace pod = boost::program_options::detail;

namespace analytic {
namespace util {

const std::string PROPERTY_ANALYTIC_PLUGIN_DIR = "analytic_plugin_directory";

const std::string PROPERTY_DB_SERVER_NAME = "db_server_name";
const std::string PROPERTY_DB_PORT = "db_port";
const std::string PROPERTY_DB_NAME = "db_name";
const std::string PROPERTY_DB_USERNAME = "db_username";
const std::string PROPERTY_DB_PASSWORD = "db_password";

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
