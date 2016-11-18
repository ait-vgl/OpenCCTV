
#ifndef OPENCCTV_CONFIG_HPP_
#define OPENCCTV_CONFIG_HPP_

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

namespace opencctv {
namespace util {
// for starter to start the process
const std::string PROPERTY_OPENCCTV_SERVER_PORT = "opencctv_server_port";
//const std::string PROPERTY_OPENCCTV_SERVER_DIR = "opencctv_server_path";

const std::string PROPERTY_ANALYTIC_SERVER_IP = "analytic_server_ip";
const std::string PROPERTY_ANALYTIC_SERVER_PORT = "analytic_server_port";
const std::string PROPERTY_INTERNAL_QUEUE_SIZE = "internal_queue_size";
const std::string PROPERTY_REMOTE_QUEUE_SIZE = "remote_queue_size";

const std::string PROPERTY_VMS_CONNECTOR_DIR = "vms_connector_directory";

const std::string PROPERTY_DB_SERVER_NAME = "db_server_name";
const std::string PROPERTY_DB_PORT = "db_port";
const std::string PROPERTY_DB_NAME = "db_name";
const std::string PROPERTY_DB_USERNAME = "db_username";
const std::string PROPERTY_DB_PASSWORD = "db_password";

const std::string PROPERTY_RESULTS_DB_SERVER_NAME = "results_db_server_name";
const std::string PROPERTY_RESULTS_DB_PORT = "results_db_port";
const std::string PROPERTY_RESULTS_DB_NAME = "results_db_name";
const std::string PROPERTY_RESULTS_DB_USERNAME = "results_db_username";
const std::string PROPERTY_RESULTS_DB_PASSWORD = "results_db_password";

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
} /* namespace opencctv */

#endif /* OPENCCTV_CONFIG_HPP_ */
