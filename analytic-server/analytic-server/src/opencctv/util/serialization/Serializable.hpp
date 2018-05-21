
#ifndef OPENCCTV_SERIALIZABLE_HPP_
#define OPENCCTV_SERIALIZABLE_HPP_

#include <string>
#include "../../Image.hpp"
#include "../../../analytic/AnalyticResult.hpp"

namespace opencctv {
namespace util {
namespace serialization {

class Serializable
{
public:
	/**
	 * Alert! Please remember to delete the opencctv::Image
	 * if it was created on the heap using new operator.
	 * Please delete returned std::string when you are done with it
	 * because it was constructed on heap using new operator.
	 * This function is used to serialized large Image objects.
	 */
	virtual std::string* serialize(const opencctv::Image* pImage) = 0;

	/**
	 * Alert! Please remember to delete the std::string
	 * if it was created on the heap using new operator.
	 * Please delete returned opencctv::Image when you are done with it
	 * because it was constructed on heap using new operator.
	 * This function is used to de-serialized large Image objects.
	 */
	virtual opencctv::Image* deserializeImage(const std::string* pSSerializedImage) = 0;
	virtual std::string serialize(const analytic::AnalyticResult& analyticResult) = 0;
	virtual analytic::AnalyticResult deserializeAnalyticResult(const std::string& sSerializedAnalyticResult) = 0;
	virtual ~Serializable(){}
};

} /* namespace serialization */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTV_SERIALIZABLE_HPP_ */
