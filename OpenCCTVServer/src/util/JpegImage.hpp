/*
 * JpegImage.hpp
 *
 *  Created on: Oct 13, 2014
 *      Author: lakindu
 */

#ifndef JPEGIMAGE_HPP_
#define JPEGIMAGE_HPP_

#include "../Image.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class JpegImage
{
public:
	static Mat toOpenCvMat(Image& image);
	static Image toOpenCCTVImage(const Image& imageInput, const Mat& imageOutput, const string& result);
};

#endif /* JPEGIMAGE_HPP_ */
