
#ifndef ANALYTIC_IMAGEUTIL_HPP_
#define ANALYTIC_IMAGEUTIL_HPP_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "../../opencctv/Image.hpp"

namespace analytic {
namespace util {

class ImageUtil {
public:
	static cv::Mat toOpenCvMat(const opencctv::Image* pImage);
	static void toOpenCvMat(const opencctv::Image* pImage, cv::Mat& matImage);
};

} /* namespace util */
} /* namespace analytic */

#endif /* ANALYTIC_IMAGEUTIL_HPP_ */
