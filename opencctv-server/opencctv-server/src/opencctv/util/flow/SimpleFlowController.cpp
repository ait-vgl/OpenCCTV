
#include "SimpleFlowController.hpp"

namespace opencctv {
namespace util {
namespace flow {

SimpleFlowController::SimpleFlowController(size_t maxNumberOfImagesInRemoteQueue) {
	_maxSize = maxNumberOfImagesInRemoteQueue;
	_numbOfImagesStillInMq = 0;
}

bool SimpleFlowController::canSendImageGeneratedAt(const unsigned long long& lImageGeneratedTimeMs) {
	boost::mutex::scoped_lock lock(_mutex);
	if (_numbOfImagesStillInMq < _maxSize) {
		return true;
	}
	return false;
}

void SimpleFlowController::sent(opencctv::Image* pImage,
		const unsigned long long& lImageGeneratedTimeMs) {
	boost::mutex::scoped_lock lock(_mutex);
	++_numbOfImagesStillInMq;
}

void SimpleFlowController::received() {
	boost::mutex::scoped_lock lock(_mutex);
	--_numbOfImagesStillInMq;
}

SimpleFlowController::~SimpleFlowController() {
}

} /* namespace flow */
} /* namespace util */
} /* namespace opencctv */
