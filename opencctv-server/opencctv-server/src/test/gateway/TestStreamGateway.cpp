#include "TestStreamGateway.hpp"

namespace test {
namespace gateway {

TestStreamGateway::TestStreamGateway() {
}

void TestStreamGateway::findAll(std::vector<opencctv::dto::Stream>& vToStoreStreams) {
	using boost::property_tree::ptree;
	using opencctv::util::Util;
	ptree pt;
	try
	{
		/* ./ is the dir current dir */
		read_xml("./test/xml/streams.xml", pt);
		BOOST_FOREACH( ptree::value_type const& v, pt.get_child("streams") ) {
			if (v.first == "stream") {
				opencctv::dto::Stream stream;
				stream.setId(v.second.get<unsigned int>("id"));
				stream.setWidth(v.second.get<unsigned int>("width"));
				stream.setHeight(v.second.get<unsigned int>("height"));
				stream.setKeepAspectRatio(v.second.get<bool>("keepaspectratio"));
				stream.setAllowUpSizing(v.second.get<bool>("allowupsizing"));
				stream.setCompressionRate(v.second.get<int>("compressionrate"));
				stream.setCameraId(
						Util::trim(v.second.get<std::string>("cameraid")));
				stream.setVmsTypeId(v.second.get<unsigned int>("vmstypeid"));
				stream.setVmsServerIp(
						Util::trim(v.second.get<std::string>("serverip")));
				stream.setVmsServerPort(v.second.get<unsigned short int>("serverport"));
				stream.setVmsUsername(
						Util::trim(v.second.get<std::string>("username")));
				stream.setVmsPassword(
						Util::trim(v.second.get<std::string>("password")));
				stream.setVmsConnectorDirLocation(
						Util::trim(v.second.get<std::string>("vmsconnectordir")));
				stream.setVmsConnectorFilename(
						Util::trim(v.second.get<std::string>("vmsconnectorfile")));
				vToStoreStreams.push_back(stream);
			}
		}
	}
	catch(boost::exception &e)
	{
		std::string sErrMsg = "Failed to read from ./test/xml/streams.xml. ";
		sErrMsg.append(boost::diagnostic_information(e));
		throw opencctv::Exception(sErrMsg);
	}
}

TestStreamGateway::~TestStreamGateway() {
}

} /* namespace gateway */
} /* namespace test */
