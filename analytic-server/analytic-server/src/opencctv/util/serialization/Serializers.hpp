
#ifndef OPENCCTV_SERIALIZERS_HPP_
#define OPENCCTV_SERIALIZERS_HPP_

#include "Serializable.hpp"
#include "gpb/ProtoBuf.hpp"

namespace opencctv {
namespace util {
namespace serialization {

class Serializers {
public:
	static Serializable* getInstanceOfDefaultSerializer();
};

} /* namespace serialization */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTV_SERIALIZERS_HPP_ */
