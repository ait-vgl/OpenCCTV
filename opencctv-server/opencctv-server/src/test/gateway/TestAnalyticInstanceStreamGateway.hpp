
#ifndef OPENCCTV_TESTANALYTICINSTANCESTREAMGATEWAY_HPP_
#define OPENCCTV_TESTANALYTICINSTANCESTREAMGATEWAY_HPP_

#include <vector>
#include <fstream> // to read files
/* boost includes (/usr/local/include/) */
/* boost library links (/usr/local/lib/) -lboost_system */
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp> // boost::algorithm::trim()
#include <boost/exception/diagnostic_information.hpp> // to convert boost::exception into a string
#include "../../opencctv/dto/AnalyticInstanceStream.hpp"
#include "../../opencctv/Exception.hpp"
#include "../../opencctv/util/Util.hpp"

namespace test {
namespace gateway {

class TestAnalyticInstanceStreamGateway {
public:
	TestAnalyticInstanceStreamGateway();
	void findAllForStream(unsigned int iStreamId, std::vector<opencctv::dto::AnalyticInstanceStream>& vToStoreAIS);
	virtual ~TestAnalyticInstanceStreamGateway();
};

} /* namespace gateway */
} /* namespace test */

#endif /* OPENCCTV_TESTANALYTICINSTANCESTREAMGATEWAY_HPP_ */
