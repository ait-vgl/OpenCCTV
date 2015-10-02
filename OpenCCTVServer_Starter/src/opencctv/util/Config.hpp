
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

const std::string PROPERTY_OPENCCTV_SERVER_PORT = "opencctv_server_starter_port";
const std::string PROPERTY_OPENCCTV_SERVER_DIR = "opencctv_server_path";

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
