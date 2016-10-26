
#include "FaceDetectAnalytic.hpp"

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

bool FaceDetectAnalytic::init(const std::string& sAnalyticPluginDirLocation)
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

	if(pConfig->get(PROPERTY_DISPLAY_OUTPUT_IMAGES).compare("yes") == 0)
	{
		_bDisplayOutput = true;
	}
	else
	{
		_bDisplayOutput = false;
	}
	_dScaleFactor = boost::lexical_cast<double>(pConfig->get(PROPERTY_CASCADE_SCALE_FACTOR));
	_iMinNeighbors = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MIN_NEIGHBORS));
	int iMinWidth = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MIN_SIZE_WIDTH));
	int iMinHeight = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MIN_SIZE_WIDTH));
	_minSize = cv::Size(iMinWidth, iMinHeight);

	int iMaxWidth = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MAX_SIZE_WIDTH));
	int iMaxHeight = boost::lexical_cast<int>(pConfig->get(PROPERTY_CASCADE_MAX_SIZE_WIDTH));
	_maxSize = cv::Size(iMaxWidth, iMaxHeight);

	return true;
}

void FaceDetectAnalytic::process(analytic::ConcurrentQueue<analytic::api::Image_t>* pInputQueue, analytic::ConcurrentQueue<analytic::api::Image_t>* pOutputQueue)
{
	/* Analytic process starts from here */
	while(pHaarCascade)
	{
		/* 1. get a image from input queue */
		api::Image_t image = pInputQueue->pop();
		cv::Mat matInputImage = image.matImage; // got it
		/* 2. clone the input image */
		cv::Mat matToBeProcessed = matInputImage.clone();
		/* 3. release the input image */
		matInputImage.release(); //please do this release!
		/* 4. OK, now use cloned image for analysis */

		// Start processing
		cv::Mat matGray;
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

		/* 5. set output */
		if(vFaces.size() > 0)
		{
			image.bGenerateAnalyticEvent = true;
			resultXml(vFaces, image.sCustomTextResult);
		}
		/* 6. push into output queue */
		pOutputQueue->push(image);
	}
}

void FaceDetectAnalytic::resultXml(vector<Rect_<int> > vBoundingBoxes, std::string& sToStoreXml)
{
	std::stringstream ssXml;
	ssXml << "<result>";
		ssXml << "<count>" << vBoundingBoxes.size() << "</count>";
		std::string sbBXml;
		generateBoundingBoxesXml(vBoundingBoxes, sbBXml);
		ssXml << sbBXml;
	ssXml << "</result>";
	sToStoreXml = ssXml.str();
}

FaceDetectAnalytic::~FaceDetectAnalytic() {
	if(pHaarCascade){
		delete pHaarCascade;
		pHaarCascade = NULL;
	}
}

