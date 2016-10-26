
#include "ImageUtil.hpp"

namespace analytic {
namespace util {

cv::Mat ImageUtil::toOpenCvMat(const opencctv::Image* pImage)
{
//	std::vector<unsigned char> imageData = pImage->getImageData();
//	cv::Mat buf(cv::Size(pImage->getWidth(), pImage->getHeight()), CV_8U, &imageData[0]);
	cv::Mat matImage = cv::imdecode(pImage->getImageData(), CV_LOAD_IMAGE_COLOR);
//	buf.refcount = 0;
//	buf.release();
//	imageData.clear();
	return matImage;
}

void ImageUtil::toOpenCvMat(const opencctv::Image* pImage, cv::Mat& matImage)
{
	matImage = cv::imdecode(pImage->getImageData(), CV_LOAD_IMAGE_COLOR);
}

} /* namespace util */
} /* namespace analytic */
