
#ifndef OPENCCTV_BASICFLOWCONTROLLER_HPP_
#define OPENCCTV_BASICFLOWCONTROLLER_HPP_

#include "FlowController.hpp"
#include "../Util.hpp"
#include <vector>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

namespace opencctv {
namespace util {
namespace flow {

class BasicFlowController : public FlowController {
private:
	unsigned long long _lLastReceivedTime;
	std::vector<unsigned long long> _vTimeDifferences;
	unsigned long long _lLastImageGeneratedTime;
	size_t _numbOfImagesStillInMq;
	boost::mutex _mutex;
public:
	BasicFlowController();
	bool canSendImageGeneratedAt(const unsigned long long& lImageGeneratedTimeMs);
	void sent(opencctv::Image* pImage,const unsigned long long& lImageGeneratedTimeMs);
	void received();
	virtual ~BasicFlowController();
};

} /* namespace opencctv */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTV_BASICFLOWCONTROLLER_HPP_ */
