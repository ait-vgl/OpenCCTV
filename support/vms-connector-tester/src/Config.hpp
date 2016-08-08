
#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <map> // std::map
#include <string> // std::string
#include <fstream> // to read files on disk
#include <stdexcept> // std::exception
/* boost includes (/usr/local/include/) */
/* boost library links (/usr/local/lib/) -lboost_system */
#include <boost/config.hpp>
// boost library link for boost program options (/usr/local/lib/) -lboost_program_options
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

namespace pod = boost::program_options::detail;

const std::string PROPERTY_SERVER_IP = "server_ip";
const std::string PROPERTY_SERVER_PORT = "server_port";
const std::string PROPERTY_USERNAME = "username";
const std::string PROPERTY_PASSWORD = "password";
const std::string PROPERTY_CAMERA_ID = "camera_id";
const std::string PROPERTY_WIDTH = "width";
const std::string PROPERTY_HEIGHT = "height";
const std::string PROPERTY_KEEP_ASPECT_RATIO = "keep_aspect_ratio";
const std::string PROPERTY_ALLOW_UPSIZING = "allow_upsizing";
const std::string PROPERTY_COMPRESSION_RATE = "compression_rate";

class Config {
private:
	static Config* _pConfig;
	std::map<std::string, std::string> _mConfigDetails;
	Config(const std::string& sFilePath);
public:
	static Config* getInstance(const std::string& sFilePath);
	std::string get(const std::string& sPropertyName);
	virtual ~Config();
};

#endif /* CONFIG_HPP_ */
