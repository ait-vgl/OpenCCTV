
#include "FaceDetectAnalytic.hpp"
#include <sys/time.h>
#include <fstream>
using namespace std;

// Get current date/time, format is YYYY-MM-DD_HH:mm:ss
std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d_%X", &tstruct);
    return buf;
}

void generateBoundingBoxesXml(vector<Rect_<int> > vBoxes, std::string& sXml)
{
	std::stringstream ssXml;
	ssXml << "<boxes>";
	for(size_t i = 0; i < vBoxes.size(); ++i)
	{
		ssXml << "<box>";
		ssXml << "<x>" << vBoxes[i].x << "</x>";
		ssXml << "<y>" << vBoxes[i].y << "</y>";
		ssXml << "<w>" << vBoxes[i].width << "</w>";
		ssXml << "<h>" << vBoxes[i].height << "</h>";
		ssXml << "</box>";
	}
	ssXml << "</boxes>";
	sXml = ssXml.str();
}

FaceDetectAnalytic::FaceDetectAnalytic() : api::Analytic() {
	pHaarCascade = NULL;
	_dScaleFactor = 1.1;
	_iMinNeighbors = 3;
	_bDisplayOutput = true;
}

bool FaceDetectAnalytic::init(const std::string& sAnalyticPluginDirLocation, std::map<string,FileStorage*> mConfigFiles, std::map<string,FrameGrabberWrapper*> mFrameGrabbers)
{	
	/* access files on analytic directory */
	// prepare file paths as below and read them from the disk.
	std::string sDir = sAnalyticPluginDirLocation;
	if(*sDir.rbegin() != '/') // check last char
	{
		sDir.append("/");
	}
	std::string sConfigFilePath = sDir;
	sConfigFilePath.append("analytic.config");
	Config* pConfig = Config::getInstance(sConfigFilePath);

	std::string sHaarCascadeFilePath = sDir;
	sHaarCascadeFilePath.append(pConfig->get(PROPERTY_CASCADE_FILE));

	// do initialization
	pHaarCascade = new CascadeClassifier();
	pHaarCascade->load(sHaarCascadeFilePath);

	//if(pConfig->get(PROPERTY_DISPLAY_OUTPUT_IMAGES).compare("yes") == 0)
	//{
	//	_bDisplayOutput = true;
	//}
	//else
	//{
	//	_bDisplayOutput = false;
	//}
	//_dScaleFactor = boost::lexical_cast<double>(pConfig->get(PROPERTY_CASCADE_SCALE_FACTOR));
	//_iMinNeighbors = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MIN_NEIGHBORS));
	//int iMinWidth = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MIN_SIZE_WIDTH));
	//int iMinHeight = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MIN_SIZE_WIDTH));
	//_minSize = cv::Size(iMinWidth, iMinHeight);

	//int iMaxWidth = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MAX_SIZE_WIDTH));
	//int iMaxHeight = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MAX_SIZE_WIDTH));
	//_maxSize = cv::Size(iMaxWidth, iMaxHeight);

	return true;
}

void FaceDetectAnalytic::process(analytic::ConcurrentQueue<analytic::api::Image_t>* pOutputQueue)
{
	//float fNumFramesPerSec = 0;
	//int iFrameNo = 0;
	//timeval tvStart;
	//timeval tvCur;
	//gettimeofday(&tvStart,NULL);
	//gettimeofday(&tvCur,NULL);

	// Enable writing to file
	//ofstream writeFile;
	//int iSec = tvStart.tv_sec;
	//int iUs = tvStart.tv_usec;
	//stringstream ssOut;
	//ssOut.str("");
	//ssOut << iSec;
	//string sFilename = "FPS_Log_"+ssOut.str()+"s_";
	//ssOut.str("");
	//ssOut << iUs;
	//sFilename += ssOut.str() + ".log";
	//writeFile.open(sFilename.c_str());
	/* Analytic process starts from here */
	while(pHaarCascade)
	{
		/* 1. get a image from input queue */
		//api::Image_t image = pInputQueue->pop();
		//cv::Mat matInputImage = image.matImage; // got it

		//iFrameNo++;
		//gettimeofday(&tvCur,NULL);
		// Displaying fps information every 30 frames
		//if(iFrameNo%30==0)
		//{
		//	double startTimeInMs = tvStart.tv_sec*1000+tvStart.tv_usec/1000.0f;
		//	double endTimeInMs = tvCur.tv_sec*1000+tvCur.tv_usec/1000.0f;
		//	double diffTimeInSec = (endTimeInMs - startTimeInMs)/1000.0f;

			// REcompute diffTimeInSec as it gets truncated above.
		//	diffTimeInSec = (tvCur.tv_sec-tvStart.tv_sec);
		//	if(diffTimeInSec>=0)
		//	{
		//		if(tvCur.tv_usec >=tvStart.tv_usec)
		//		{
		//			diffTimeInSec += (tvCur.tv_usec - tvStart.tv_usec) /1000000.0;
		//		}
		//		else
		//		{
		//			diffTimeInSec -= 1.0f;
		//			diffTimeInSec += (tvStart.tv_usec - tvCur.tv_usec) /1000000.0;
		//		}
		//	}

		//	fNumFramesPerSec = 30.0f/diffTimeInSec;
		//	writeFile << "matInputImage: " << matInputImage.rows << " x " << matInputImage.cols << " type: " << matInputImage.type() << endl;
		//	writeFile << "startTimeInMs: " << startTimeInMs << " ms" << endl;
		//	writeFile << "endTimeInMs: " << endTimeInMs << " ms" << endl;
		//	writeFile << "diffTimeInSec: " << diffTimeInSec << " s"<<endl;
		//	writeFile << "fNumFramesPerSec: " << fNumFramesPerSec << " fps" << endl;
		//	writeFile << tvStart.tv_sec << "s " << tvStart.tv_usec << "us -> "
		//			<< tvCur.tv_sec << "s " << tvCur.tv_usec << "us ---- " << fNumFramesPerSec << " fps" << endl;
			// Reset start time for next check
		//	gettimeofday(&tvStart,NULL);
		//}

		/* 2. clone the input image */
		//cv::Mat matToBeProcessed = matInputImage.clone();
		/* 3. release the input image */
		//matInputImage.release(); //please do this release!
		/* 4. OK, now use cloned image for analysis */

		//usleep(33000); //33 ms to get 30fps   1 frame interval (us) = 1000 000 us / 30 frames
		sleep(2); // 1 second
		// Start processing
		/*cv::Mat matGray;
		cv::cvtColor(matToBeProcessed, matGray, CV_BGR2GRAY);
		equalizeHist(matGray, matGray);
		vector<Rect_<int> > vFaces;
		pHaarCascade->detectMultiScale(matGray, vFaces, _dScaleFactor, _iMinNeighbors, 0|CV_HAAR_SCALE_IMAGE, _minSize, _maxSize);
		for (size_t i = 0; i < vFaces.size(); ++i) {
			Rect recFace = vFaces[i];
			Mat matFace = matGray(recFace);
			rectangle(matToBeProcessed, recFace, CV_RGB(0, 255, 0), 1);
			matFace.release();
		}

		if(_bDisplayOutput)
		{
			cv::imshow("Output", matToBeProcessed);
			cv::waitKey(1);
		}

		// freeing generated Mats
		matGray.release();
		matToBeProcessed.release();
		*/
	
		/* 5. set output */
		api::Image_t image;
		image.bGenerateAnalyticEvent = true;
		resultXml(image.sCustomTextResult);
		
		/* 6. push into output queue */
		pOutputQueue->push(image);

	}
	//writeFile.close();
}

//void FaceDetectAnalytic::resultXml(vector<Rect_<int> > vBoundingBoxes, std::string& sToStoreXml)
void FaceDetectAnalytic::resultXml(std::string& sToStoreXml)
{
	std::stringstream ssXml;
	ssXml << "<result>";
		//ssXml << "<count>" << vBoundingBoxes.size() << "</count>";
		//std::string sbBXml;
		//generateBoundingBoxesXml(vBoundingBoxes, sbBXml);
		//ssXml << sbBXml;
	ssXml << "</result>";
	sToStoreXml = ssXml.str();
}

FaceDetectAnalytic::~FaceDetectAnalytic() {
	if(pHaarCascade){
		delete pHaarCascade;
		pHaarCascade = NULL;
	}
}

