
#ifndef OPENCCTV_PROTOBUF_HPP_
#define OPENCCTV_PROTOBUF_HPP_

#include <vector>
#include "../Serializable.hpp"
/* Google proto buf includes (/usr/local/include) */
/* Google proto buf libraries (/usr/local/lib) -lprotobuf */
#include "image.pb.h" // serializable image
#include "analytic_result.pb.h" // serializable analytic result

namespace opencctv {
namespace util {
namespace serialization {

class ProtoBuf : public Serializable {
public:
	ProtoBuf();
	std::string* serialize(const opencctv::Image* pImage);
	opencctv::Image* deserializeImage(const std::string* pSSerializedImage);
	std::string serialize(const analytic::AnalyticResult& analyticResult);
	analytic::AnalyticResult deserializeAnalyticResult(const std::string& sSerializedAnalyticResult);
	virtual ~ProtoBuf();
};

} /* namespace serialization */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTV_PROTOBUF_HPP_ */
