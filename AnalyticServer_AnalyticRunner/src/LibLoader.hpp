/*
 * LibLoader.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: anjana
 */

#ifndef LIBLOADER_HPP_
#define LIBLOADER_HPP_

#include <string>
#include <map>
#include <dlfcn.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

#include "util/TypeDef.hpp"

using namespace std;
using namespace cv;
using namespace util;

class LibLoader {
private:
	string libPath; //This is the absolute path to the analytics library
	void* handle;

public:
	LibLoader();
	LibLoader(const string& libPath);
	virtual ~LibLoader();
	int load();
	bool init(const map<string, string>& analyticParameters);
	//bool process(Mat& inputImg);
	int process(Mat& inputImg, string& resultStr);
	string geResult();
	Mat getOutputImage();
};


#endif /* LIBLOADER_HPP_ */
