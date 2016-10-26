
#ifndef OPENCCTV_TESTSTREAMGATEWAY_HPP_
#define OPENCCTV_TESTSTREAMGATEWAY_HPP_

#include <vector> // std::vector
#include <fstream> // to read files
/* boost includes (/usr/local/include/) */
/* boost library links (/usr/local/lib/) -lboost_system */
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp> // boost::algorithm::trim()
#include <boost/exception/diagnostic_information.hpp> // to convert boost::exception into a string
#include "../../opencctv/dto/Stream.hpp"
#include "../../opencctv/Exception.hpp"
#include "../../opencctv/util/Util.hpp"

namespace test {
namespace gateway {

class TestStreamGateway {
public:
	TestStreamGateway();
	void findAll(std::vector<opencctv::dto::Stream>& vToStoreStreams);
	virtual ~TestStreamGateway();
};

} /* namespace gateway */
} /* namespace test */

#endif /* OPENCCTV_TESTSTREAMGATEWAY_HPP_ */
