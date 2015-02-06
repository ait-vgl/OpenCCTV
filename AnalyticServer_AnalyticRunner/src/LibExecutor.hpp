/*
 * LibraryExecutor.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: lakindu
 */

#ifndef LIBRARYEXECUTOR_HPP_
#define LIBRARYEXECUTOR_HPP_

#include <string>
#include <sstream>
//#include <dlfcn.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "mq/TcpMq.hpp"
#include "util/JpegImage.hpp"
#include "Image.hpp"
#include "LibLoader.hpp"

#include "util/AnalyticOutputMessage.hpp"

using namespace std;
using namespace cv;
using namespace mq;

class LibExecutor
{
private:
	int streamId;
	int analyticInstId;
	int analyticId;
	string libPath;
	string imageQueueInPort;
	string imageQueueOutPort;
	int count;	//TODO:Temporary count of images to process; remove later


public:
	//LibExecutor(const string& libPath);
	LibExecutor(const int streamId, const int lyticInstId, const int analyticId, const string& libPath, const string& imageQueueInPort, const string& imageQueueOutPort, const int count);
	int start();
	void run(Mat& img);
	~LibExecutor();
};

#endif /* LIBRARYEXECUTOR_HPP_ */
