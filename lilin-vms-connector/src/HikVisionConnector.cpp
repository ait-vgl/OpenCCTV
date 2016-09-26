
#include "HikVisionConnector.hpp"

opencctv::ConcurrentQueue<opencctv::Image>* _pQueue = NULL;

HikVisionConnector::HikVisionConnector() : opencctv::api::VmsConnector() {
	_pContext = (struct ctx*)malloc(sizeof(*_pContext));
	_bEnable = false;
	_pVlcInstance = NULL;
	_pVlcMedia = NULL;
	_pVlcMediaPlayer = NULL;
}

bool HikVisionConnector::init(const opencctv::api::VmsConnectInfo& info, const std::string sPathToAnalyticDir)
{
	// init context
	_pContext->pImage = cvCreateImage(cvSize(info.iWidth, info.iHeight), IPL_DEPTH_8U, 4);
	_pContext->puchPixels = (unsigned char*)_pContext->pImage->imageData;

	std::stringstream ssUri;
	ssUri << "http://";
	 if(!info.sUsername.empty())
	 {
		 ssUri << info.sUsername << ":" << info.sPassword << "@";
	 }
	// ssUri << info.sServerIp << ":" << 554 << "/ISAPI/Streaming/channels/" << info.sCameraId;

	ssUri << info.sServerIp << ":" << info.iServerPort << "/" << info.sCameraId;

	//const char * const aChVlcArgs[] = {"--rtsp-tcp"};

	 const char *const aChVlcArgs[] = {
	        "-I",
	        "dummy",
	        "--ignore-config",
	        "--extraintf=logger",
	       // "--verbose=2",
		"--no-plugins-cache",
		"--demux=h264",
		"--h264-fps=25.000000",
	    };

	_pVlcInstance = libvlc_new(sizeof(aChVlcArgs) / sizeof(aChVlcArgs[0]), aChVlcArgs);
	_pVlcMedia = libvlc_media_new_location(_pVlcInstance, ssUri.str().c_str());
	_pVlcMediaPlayer = libvlc_media_player_new_from_media(_pVlcMedia);
	libvlc_video_set_callbacks(_pVlcMediaPlayer, lock, unlock, display, _pContext);
	libvlc_video_set_format(_pVlcMediaPlayer, "RV32", info.iWidth, info.iHeight, info.iWidth * 4);

	_bEnable = true;
	return _bEnable;
}

void HikVisionConnector::produceImageObjects(opencctv::ConcurrentQueue<opencctv::Image>* pQueue)
{
	_pQueue = pQueue;


	libvlc_media_player_play(_pVlcMediaPlayer);
	//std::cout << "Start vlc player" << std::endl;
	pause();
	//_bEnable = false;
	//_pQueue = NULL;
}

bool HikVisionConnector::isStillProducingImages()
{
	return _bEnable;
}

void* HikVisionConnector::lock(void *data, void**p_pixels)
{
	struct ctx *ctx = (struct ctx*) data;
	if(_pQueue)
	{
		*p_pixels = ctx->puchPixels;
	}
	return NULL;
}
void HikVisionConnector::unlock(void *data, void *id, void * const *p_pixels)
{
	assert(id == NULL);
}
void HikVisionConnector::display(void *data, void *id)
{
			std::cout << "VLC display." << std::endl;

			struct ctx *ctx = (struct ctx*)data;
			IplImage *pImageDisplay = ctx->pImage;
			cv::Mat matImage(pImageDisplay, false);
			if(!matImage.empty())
			{
				opencctv::Image* pImage = new opencctv::Image();
				std::vector<unsigned char> vTemp;
				cv::imencode(".jpg", matImage, vTemp);
				pImage->setImageData(vTemp);
				pImage->setTimestamp(currentDateTime());
				_pQueue->push(pImage);
			}
}

// Get current date/time in ISO 8601 format (YYYYMMDDTHHmmssZ)
std::string HikVisionConnector::currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y%m%dT%H%M%SZ", &tstruct);
    return buf;
}

HikVisionConnector::~HikVisionConnector() {

	// Cannot have any text output, it effects to "Pluglinvalidater in Web"

	//std::cout<< "test xxxxxxxxxxxxxxxxxxxxxxxxxxx" << std::endl;

	if(_pContext)
	{
		// clear media player

		std::cout << "5. Linin connector: destructure called" << std::endl;

		libvlc_media_release (_pVlcMedia);
		libvlc_media_player_stop(_pVlcMediaPlayer);
		libvlc_media_player_release(_pVlcMediaPlayer);
		libvlc_release(_pVlcInstance);


		/*if(libvlc_media_player_is_playing(_pVlcMediaPlayer)){

			if(libvlc_media_player_can_pause(_pVlcMediaPlayer)){
				libvlc_media_player_pause(_pVlcMediaPlayer);
				std::cout << "5.0.1 pause _pVlcMediaPlayer" << std::endl;
			}


			//libvlc_media_player_release(_pVlcMediaPlayer);
			//libvlc_release(_pVlcInstance);
			std::cout << "5.0  Stop _pVlcMediaPlayer" << std::endl;
		}



		if(_pVlcInstance){
			//delete _pVlcInstance;
			//_pVlcInstance = NULL;

			std::cout << "5.3 delete _pVlcInstance" << std::endl;
		}


		if(_pVlcMedia){
			//delete _pVlcMedia->libvlc_media_t;
			//_pVlcMedia = NULL;

			std::cout << "5.1 delete _pVlcMedia" << std::endl;
		}

		if(_pVlcMediaPlayer){
			//delete _pVlcMediaPlayer;
			//_pVlcMediaPlayer = NULL;

			std::cout << "5.2 delete _pVlcMediaPlayer" << std::endl;
		}






		std::cout << "5.5 Finish Destructor" << std::endl;
		//delete _pContext;
		//_pContext = NULL;*/

		//_bEnable = false;
	}
}

