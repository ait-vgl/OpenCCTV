
#ifndef ANALYTIC_CONSUMERTHREAD_HPP_
#define ANALYTIC_CONSUMERTHREAD_HPP_

#include "AnalyticResult.hpp"
#include "Result_t.hpp"
#include "ResultsQueue.hpp"
#include <sstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include "../opencctv/mq/Mq.hpp"
#include "../opencctv/util/log/Loggers.hpp"
#include "../opencctv/Exception.hpp"
#include "../opencctv/util/Util.hpp"
#include "db/AnalyticResultsGateway.hpp"



namespace analytic
{

class ConsumerThread
{
private:
	//ConcurrentQueue<analytic::api::Results_t>* _pResultsOutputQueue;
	analytic::api::ResultsQueue<analytic::api::Result_t*>* _pResultsOutputQueue;
	unsigned int _iAnalyticInstanceId;
public:
	//ConsumerThread(ConcurrentQueue<api::Image_t>* pResultsOutputQueue, opencctv::mq::Sender* pSender, opencctv::util::serialization::Serializable* pSerializer);
	//ConsumerThread(ConcurrentQueue<analytic::api::Results_t>* pResultsOutputQueue);
	ConsumerThread(unsigned int iAnalyticInstanceId, analytic::api::ResultsQueue<analytic::api::Result_t*>* pResultsOutputQueue);
	void operator()();
};

} /* namespace analytic */

#endif /* ANALYTIC_CONSUMERTHREAD_HPP_ */
