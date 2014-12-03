/*
 * JpegImage.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: lakindu
 */

#include "JpegImage.hpp"

Mat JpegImage::toOpenCvMat(Image& image)
{
	// image.getWidth(), image.getHeight()
	vector<char> imageData = image.getImageData();
	Mat buf(Size(480, 720), CV_8U, &imageData[0]);
	Mat matImage = imdecode(buf, CV_LOAD_IMAGE_COLOR);
	return matImage;
}

