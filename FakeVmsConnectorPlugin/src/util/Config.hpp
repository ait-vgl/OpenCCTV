
#ifndef OPENCCTV_CONFIG_HPP_
#define OPENCCTV_CONFIG_HPP_

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

namespace util {

const std::string PROPERTY_IMAGE_FILE = "image_file";
const std::string PROPERTY_IMAGE_WIDTH = "width";
const std::string PROPERTY_IMAGE_HEIGHT = "height";

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

} /* namespace util */

#endif /* OPENCCTV_CONFIG_HPP_ */
