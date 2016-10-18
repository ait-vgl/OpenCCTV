
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

const std::string PROPERTY_CASCADE_FILE = "cascade_file";
const std::string PROPERTY_DISPLAY_OUTPUT_IMAGES = "display_output_images";
const std::string PROPERTY_CASCADE_SCALE_FACTOR = "cascade_detect_scale_factor";
const std::string PROPERTY_CASCADE_MIN_NEIGHBORS = "cascade_detect_min_neighbors";
const std::string PROPERTY_CASCADE_MIN_SIZE_WIDTH = "cascade_detect_min_size_width";
const std::string PROPERTY_CASCADE_MIN_SIZE_HEIGHT = "cascade_detect_min_size_height";
const std::string PROPERTY_CASCADE_MAX_SIZE_WIDTH = "cascade_detect_max_size_width";
const std::string PROPERTY_CASCADE_MAX_SIZE_HEIGHT = "cascade_detect_max_size_height";

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

#endif /* OPENCCTV_CONFIG_HPP_ */
