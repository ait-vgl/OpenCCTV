#ifndef OPENCCTV_FLOWCONTROLLER_HPP_
#define OPENCCTV_FLOWCONTROLLER_HPP_

#include "../../Image.hpp"

namespace opencctv {
namespace util {
namespace flow {

class FlowController {
public:
	virtual bool canSendImageGeneratedAt(const unsigned long long& lImageGeneratedTimeMs) = 0;
	virtual void sent(opencctv::Image* pImage, const unsigned long long& lImageGeneratedTimeMs) = 0;
	virtual void received() = 0;
	virtual ~FlowController() {
	}
};

} /* namespace flow */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTV_FLOWCONTROLLER_HPP_ */
