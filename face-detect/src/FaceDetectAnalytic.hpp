
#ifndef FACEDETECTANALYTIC_HPP_
#define FACEDETECTANALYTIC_HPP_

#include "analytic/api/Analytic.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Config.hpp"

using namespace cv;
using namespace std;
using namespace analytic;

class FaceDetectAnalytic : public api::Analytic {
private:
	CascadeClassifier* pHaarCascade;
	bool _bDisplayOutput;
	double _dScaleFactor;
	int _iMinNeighbors;
	cv::Size _minSize;
	cv::Size _maxSize;
public:
	FaceDetectAnalytic();
	bool init(const std::string& sAnalyticPluginDirLocation);
	void process(analytic::ConcurrentQueue<analytic::api::Image_t>* pInputQueue, analytic::ConcurrentQueue<analytic::api::Image_t>* pOutputQueue);
	void resultXml(vector<Rect_<int> > vBoundingBoxes, std::string& sToStoreXml);
	virtual ~FaceDetectAnalytic();
};

extern "C" FaceDetectAnalytic* create() {
    return new FaceDetectAnalytic();
}

extern "C" void destroy(FaceDetectAnalytic* p) {
    if(p) {
    	delete p;
    	p = NULL;
    }
}

#endif /* FACEDETECTANALYTIC_HPP_ */
