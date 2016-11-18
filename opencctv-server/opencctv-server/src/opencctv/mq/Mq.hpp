
#ifndef OPENCCTV_MQ_HPP_
#define OPENCCTV_MQ_HPP_

#include <string>

namespace opencctv {
namespace mq {

class Sender
{
public:
	/**
	 * Alert! Please remember to delete the inputed std::string
	 * if it was created on the heap using new operator.
	 * This function is used to write large serialized images to a MQ.
	 */
	virtual int send(const std::string* pSMessageToSend) = 0;
	virtual ~Sender(){}
};

class Receiver
{
public:
	/**
	 * Alert! Please remember to delete the returned std::string after using
	 * if it has been created on the heap using new operator.
	 * This function can be used to read large serialized images from a MQ.
	 */
	virtual std::string* receive() = 0;
	virtual ~Receiver(){}
};

} //namespace mq
} //namespace opencctv

#endif /* OPENCCTV_MQ_HPP_ */
