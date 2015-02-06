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
	//Mat buf(Size(480, 720), CV_8U, &imageData[0]);
	Mat buf(Size(image.getWidth(), image.getHeight()), CV_8U, &imageData[0]);
	Mat matImage = imdecode(buf, CV_LOAD_IMAGE_COLOR);
	return matImage;
}

Image JpegImage::toOpenCCTVImage(const Image& imageInput, const Mat& imageOutput, const string& sResult)
{
    Image imageResutObj;

    //vector<char> resultImageData(imageOutput.cols * imageOutput.rows);
    //vector<char> resultImageData(imageOutput.rows * imageOutput.cols);
    //memcpy(resultImageData.data(), imageOutput.data, resultImageData.size() * sizeof(char));

    vector<uchar> imageOutputJpegData;
    imencode(".jpeg", imageOutput, imageOutputJpegData);

    imageResutObj.setHeight(imageOutput.rows);
    imageResutObj.setWidth(imageOutput.cols);
    imageResutObj.setInputName(imageInput.getInputName());
    imageResutObj.setStreamId(imageInput.getStreamId());
    imageResutObj.setTimestamp(imageInput.getTimestamp());
    vector<char> temp(imageOutputJpegData.begin(), imageOutputJpegData.end());
    imageResutObj.setImageData(temp);
    imageResutObj.setResult(sResult);

    return imageResutObj;
}

