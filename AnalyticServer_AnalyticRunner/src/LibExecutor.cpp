/*
 * LibExecutor.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: lakindu
 */

#include "LibExecutor.hpp"

/*LibExecutor::LibExecutor(const string& libPath)
{
	this->streamId = 0;
	this->analyticInstId = 0;
	this->analyticId = 0;
	this->libPath = libPath;
	count = 0;
}*/

LibExecutor::LibExecutor(const int streamId, const int lyticInstId, const int analyticId, const string& libPath, const string& imageQueueInPort, const string& imageQueueOutPort, const int count){
	this->streamId = streamId;
	this->analyticInstId = lyticInstId;
	this->analyticId = analyticId;
	this->libPath = libPath;
	this->imageQueueInPort = imageQueueInPort;
	this->imageQueueOutPort = imageQueueOutPort;
	this->count = count;
}

int LibExecutor::start(){
	int result = 1;

	if (libPath.size() != 0 && !libPath.empty()) {

		//Load the library specified by the libPath and
		//Initialize the analytic lib
		LibLoader libLoder(libPath);
		map<string,string> analyticParameters;

		if(libLoder.load() && libLoder.init(analyticParameters)){

			//cout<<"imageQueueInPort : "<< imageQueueInPort << " imageQueueOutPort : " << imageQueueOutPort << endl;

			//Initialize the input queue
			TcpMq mqIn;
			mqIn.createNew(imageQueueInPort, ZMQ_PULL);


			//Initialize the output queue
			TcpMq mqOut;
			mqOut.createNew(imageQueueOutPort, ZMQ_PUSH);

			Image imageObj;
			string analysisResult;

			//string displayWindowName = "Display window:";
			//displayWindowName.append(imageQueueInPort);
			//namedWindow(displayWindowName, WINDOW_AUTOSIZE);

			//TODO Later change this to a while loop?
			//int count = 1;
			while(1)
			{
				//TODO : Remove this
				//cout << "Reading image : " << count << endl;
				//++count;

				//Read serialized Image object
				string serializedImageStr = mqIn.read();

				//Initialize with received serialized string data
				std::istringstream ibuffer(serializedImageStr);

				//De-serialize and create image object
				boost::archive::text_iarchive iarchive(ibuffer);
				iarchive & imageObj;

				//cout << "Image received : " << "\tStream ID: " << imageObj.getStreamId() << "\tTimestamp: " << imageObj.getTimestamp() << endl;

				Mat img = JpegImage::toOpenCvMat(imageObj); //Convert Image to OpenCV Mat object

				if (img.empty()) {
					std::cerr << "AnalyticRunner:LibExecutor: Image is empty." << std::endl;
					// TODO: throw exception
				} else {
					libLoder.process(img,analysisResult);
					string sAnalyticLibResult = AnalyticOutputMessage::getAnalyticLibResult(analysisResult);

					Mat outputImg = libLoder.getOutputImage();

					/*ostringstream filename;
					filename << "/usr/local/opencctv/images/temp/";
					filename << imageObj.getTimestamp();
					filename << ".jpg";
					imwrite(filename.str(), outputImg);*/

					Image imageResutObj = JpegImage::toOpenCCTVImage(imageObj,outputImg,sAnalyticLibResult);

					//Write the result to the output queue
					//Prepare the output buffer
					std::ostringstream obuffer;
					//Initialize the archive
					boost::archive::text_oarchive oarchive(obuffer);
					//Serialize the image object
					oarchive & (imageResutObj);
					//Get the serialization data (a string) from the buffer
					std::string outStr(obuffer.str());
					//Write to the output queue
					mqOut.write(outStr);

					//Write the result to the output queue
					//string sAnalyticLibResult = AnalyticOutputMessage::getAnalyticLibResult(analysisResult);
					//string sAnalyticResult = AnalyticOutputMessage::getAnalyticResult(streamId,analyticInstId,analyticId,imageObj.getTimestamp(),sAnalyticLibResult);
					//mqOut.write(sAnalyticResult);
					//cout << "LibExecutor::start()-process-end()" << endl;

					//cout << endl;
					//cout << sAnalyticResult << endl;

					//Mat outputImg = libLoder.getOutputImage();
					//imshow(displayWindowName, outputImg);
					//waitKey(1);
				}
			}

			/*while (1) {
				if (waitKey(100) == 27)
					break;
			}*/
		}
		else
		{
			result = -1;
		}
	}
	else
	{
		result = -1;
	}

	return result;

}

void LibExecutor::run(Mat& img)
{
}

LibExecutor::~LibExecutor()
{
}


