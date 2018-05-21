
#include "Serializers.hpp"

namespace opencctv {
namespace util {
namespace serialization {

Serializable* Serializers::getInstanceOfDefaultSerializer() {
	return new ProtoBuf();
}

} /* namespace serialization */
} /* namespace util */
} /* namespace opencctv */
