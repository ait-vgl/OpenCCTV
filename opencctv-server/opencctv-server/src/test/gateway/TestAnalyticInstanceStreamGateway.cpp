
#include "TestAnalyticInstanceStreamGateway.hpp"

namespace test {
namespace gateway {

TestAnalyticInstanceStreamGateway::TestAnalyticInstanceStreamGateway() {
}

void TestAnalyticInstanceStreamGateway::findAllForStream(unsigned int iStreamId, std::vector<opencctv::dto::AnalyticInstanceStream>& vToStoreAIS)
{
	using boost::property_tree::ptree;
	using opencctv::util::Util;
	ptree pt;
	try {
		/* ./ is the dir current dir */
		read_xml("./test/xml/analytic_instances_streams.xml", pt);
		BOOST_FOREACH( ptree::value_type const& v, pt.get_child("analyticinstancesstreams") ) {
			if (v.first == "analyticinstancestream" &&
					v.second.get<unsigned int>("streamid") == iStreamId) {
				opencctv::dto::AnalyticInstanceStream ais;
				ais.setId(v.second.get<unsigned int>("id"));
				ais.setAnalyticInstanceId(v.second.get<unsigned int>("analyticinstanceid"));
				ais.setInputName(
						Util::trim(v.second.get<std::string>("inputname")));
				ais.setAnalyticDirLocation(
						Util::trim(v.second.get<std::string>("analyticdir")));
				ais.setAnalyticFilename(
						Util::trim(v.second.get<std::string>("analyticfile")));
				vToStoreAIS.push_back(ais);
			}
	}
} catch (boost::exception &e) {
	std::string sErrMsg = "Failed to read from ./test/xml/analytic_instances_streams.xml. ";
	sErrMsg.append(boost::diagnostic_information(e));
	throw opencctv::Exception(sErrMsg);
}
}

TestAnalyticInstanceStreamGateway::~TestAnalyticInstanceStreamGateway() {
}

} /* namespace gateway */
} /* namespace test */
