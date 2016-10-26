
#ifndef OPENCCTV_HIKVISIONCONNECTOR_HPP_
#define OPENCCTV_HIKVISIONCONNECTOR_HPP_

#include "opencctv/api/VmsConnector.hpp"

#include <vlc/vlc.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cassert>

#include <vector>
#include <sstream>

struct ctx
{
	IplImage *pImage;
	uchar *puchPixels;
};

class HikVisionConnector: public opencctv::api::VmsConnector {
private:
	ctx* _pContext;
	std::vector<unsigned char> _vCurrentImageData;
	libvlc_instance_t* _pVlcInstance;
	libvlc_media_t* _pVlcMedia;
	libvlc_media_player_t* _pVlcMediaPlayer;
	static void* lock(void *data, void**p_pixels);
	static void unlock(void *data, void *id, void * const *p_pixels);
	static void display(void *data, void *id);
	static std::string currentDateTime();
public:
	HikVisionConnector();
	bool init(const opencctv::api::VmsConnectInfo& info, const std::string sPathToAnalyticDir);
	void produceImageObjects(opencctv::ConcurrentQueue<opencctv::Image>* pQueue);
	bool isStillProducingImages();
	virtual ~HikVisionConnector();
};

extern "C" HikVisionConnector* create() {
	return new HikVisionConnector;
}

extern "C" void destroy(HikVisionConnector* p) {
	if (p) {
		delete p;
		p = NULL;
	}
}

#endif /* OPENCCTV_HIKVISIONCONNECTOR_HPP_ */
