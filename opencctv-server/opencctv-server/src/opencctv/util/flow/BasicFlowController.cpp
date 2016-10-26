#include "BasicFlowController.hpp"
#include <iostream>

namespace opencctv {
namespace util {
namespace flow {

BasicFlowController::BasicFlowController() :
		FlowController() {
	_lLastReceivedTime = 0;
	_lLastImageGeneratedTime = 0;
	_numbOfImagesStillInMq = 0;
}

bool BasicFlowController::canSendImageGeneratedAt(const unsigned long long& lImageGeneratedTimeMs) {
	unsigned long long lAverageTimeDifference = 0;
	for (size_t i = 0; i < _vTimeDifferences.size(); i++) {
		lAverageTimeDifference += _vTimeDifferences[i];
	}
	if(lAverageTimeDifference > 0)
	{
		lAverageTimeDifference /= ((unsigned long long)_vTimeDifferences.size());
	}
	unsigned long long maxTimeDifference = 0;
	maxTimeDifference = lAverageTimeDifference;
//	maxTimeDifference = (lAverageTimeDifference/1000)*999;
	if ((lImageGeneratedTimeMs - _lLastImageGeneratedTime) < maxTimeDifference) {
		return false;
	}
	return true;
}

void BasicFlowController::sent(opencctv::Image* pImage, const unsigned long long& lImageGeneratedTimeMs) {
	_lLastImageGeneratedTime = lImageGeneratedTimeMs;
	boost::mutex::scoped_lock lock(_mutex);
	++_numbOfImagesStillInMq;
	std::cout << "Temp: " << "\033[35m" << "Numb of Images in remote MQ: " << _numbOfImagesStillInMq << "\033[0m" << std::endl;
}

void BasicFlowController::received() {
	unsigned long long lCurTime = Util::getCurrentTimeMs();
	if (_lLastReceivedTime > 0) {
		if (_vTimeDifferences.size() >= 10) {
			_vTimeDifferences.erase(_vTimeDifferences.begin());
		}
		_vTimeDifferences.push_back((lCurTime - _lLastReceivedTime));
	}
	_lLastReceivedTime = lCurTime;
	boost::mutex::scoped_lock lock(_mutex);
	--_numbOfImagesStillInMq;
}

BasicFlowController::~BasicFlowController() {
}

} /* namespace flow */
} /* namespace util */
} /* namespace opencctv */
