
#ifndef OPENCCTV_SIMPLEFLOWCONTROLLER_HPP_
#define OPENCCTV_SIMPLEFLOWCONTROLLER_HPP_

#include "FlowController.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

namespace opencctv {
namespace util {
namespace flow {

class SimpleFlowController : public FlowController {
private:
	size_t _maxSize;
	size_t _numbOfImagesStillInMq;
	boost::mutex _mutex;
public:
	SimpleFlowController(size_t maxNumberOfImagesInRemoteQueue);
	bool canSendImageGeneratedAt(const unsigned long long& lImageGeneratedTimeMs);
	void sent(opencctv::Image* pImage,const unsigned long long& lImageGeneratedTimeMs);
	void received();
	virtual ~SimpleFlowController();
};

} /* namespace flow */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTV_SIMPLEFLOWCONTROLLER_HPP_ */
