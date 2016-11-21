
#ifndef ANALYTIC_CONSUMERTHREAD_HPP_
#define ANALYTIC_CONSUMERTHREAD_HPP_

#include <sstream>
#include "../opencctv/mq/Mq.hpp"
#include "api/Analytic.hpp"
#include "../opencctv/util/log/Loggers.hpp"
#include "../opencctv/util/serialization/Serializable.hpp"
#include "AnalyticResult.hpp"
#include "../opencctv/util/Util.hpp"
#include <opencv2/highgui/highgui.hpp>

namespace analytic {

class ConsumerThread {
private:
	ConcurrentQueue<api::Image_t>* _pResultsOutputQueue;
	opencctv::mq::Sender* _pSender;
	opencctv::util::serialization::Serializable* _pSerializer;
public:
	ConsumerThread(ConcurrentQueue<api::Image_t>* pResultsOutputQueue, opencctv::mq::Sender* pSender, opencctv::util::serialization::Serializable* pSerializer);
	void operator()();
};

} /* namespace analytic */

#endif /* ANALYTIC_CONSUMERTHREAD_HPP_ */
