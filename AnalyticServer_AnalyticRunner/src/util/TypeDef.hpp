/*
 * TypeDef.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: anjana
 */

#ifndef TYPEDEF_HPP_
#define TYPEDEF_HPP_

#include <string>

using namespace std;
using namespace cv;

namespace util {

	//Typedef for extern "C" bool init(const map<string, string>& analyticParameters)
	typedef bool (*init_t)(const map<string, string>& analyticParameters);

	//Typedef for extern "C" int process(Mat& inputImg, string& resultStr)
	typedef int (*process_t)(Mat& inputImg, string& resultStr);

	//Typedef for extern "C" string getResult()
	typedef string (*results_t)();

	//Typedef for extern "C" Mat& getOutputImage()
	typedef Mat (*output_t)();

} /* namespace util */


#endif /* TYPEDEF_HPP_ */
