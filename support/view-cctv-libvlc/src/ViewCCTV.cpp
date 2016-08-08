//#include "stdafx.h"
//#include <cvaux.h>
//#include <highgui.h>"
//#include <windows.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vlc/vlc.h>


#define VIDEO_WIDTH   640
#define VIDEO_HEIGHT  480

using namespace cv;

struct ctx
{
    Mat* image;
    //HANDLE mutex;
    uchar *pixels;
};

void *lock( void *data, void**p_pixels )
{
	 std::cout << "* lock * " << std::endl;
    struct ctx *ctx = (struct ctx*)data;
    //WaitForSingleObject( ctx->mutex, INFINITE );
    *p_pixels = ctx->pixels;
    return NULL;
}

void display( void *data, void *id )
{
	 std::cout << "* Display * " << std::endl;
    (void) data;
    assert( id == NULL );
}

void unlock( void *data, void *id, void *const *p_pixels )
{
	 std::cout << "* unclock * " << std::endl;
    struct ctx *ctx = (struct ctx*)data;
    cv::Mat img = *ctx->image;
    imshow("test", img);
    //ReleaseMutex( ctx->mutex );
    std::cout << "Image " << img.cols << std::endl;
    // cv::imwrite("cam.jpg", img);
}

int main()
{
    libvlc_instance_t  *vlcInstance;
    libvlc_media_player_t *mp;
    libvlc_media_t   *media;

    const char *const vlc_args[] = {
        "-I",
        "dummy",
        "--ignore-config",
        "--extraintf=logger",
        "--verbose=2",
		"--demux=h264",
		"--h264-fps=25.000000",

    };

    int vlc_argc = sizeof(vlc_args) / sizeof(vlc_args[0]);

    //libvlc_instance_t* libvlc_new ( int argc, const char *const *argv )
    vlcInstance = libvlc_new( vlc_argc, vlc_args );
    if ( vlcInstance == NULL ) {
        std::cout << "Create Media Stream Error" << std::endl;
        return 0;
    }

    //LIBVLC_API libvlc_media_t* libvlc_media_new_location( libvlc_instance_t *p_instance, const char *psz_mrl )
    //const char *psz_mrl = "rtsp://admin:pass@192.41.170.253:554/rtsph264720p";
    const char *psz_mrl = "http://admin:@192.41.170.222:8021/getstream000";
    media = libvlc_media_new_location( vlcInstance, psz_mrl );
    if ( media == NULL ) {
        std::cout << "Media Stream is Null" << std::endl;
        return 0;
    }

    //LIBVLC_API libvlc_media_player_t* libvlc_media_player_new_from_media( libvlc_media_t *p_md )

    mp = libvlc_media_player_new_from_media( media );

    // LIBVLC_API void libvlc_media_release( libvlc_media_t *p_md )

    libvlc_media_release(media);


    struct ctx* context = (struct ctx*)malloc( sizeof(*context) );
    //context->mutex      = CreateMutex( NULL, false, NULL );
    context->image      = new Mat( VIDEO_HEIGHT, VIDEO_WIDTH, CV_8UC3 );
    context->pixels     = (unsigned char *)context->image->data;

    // show blank image
    cv::Mat img = *context->image;
    imshow("test", img);


    // LIBVLC_API void libvlc_video_set_callbacks( libvlc_media_player_t  *mp,
    //                                             libvlc_video_lock_cb    lock,
    //                                             libvlc_video_unlock_cb  unlock,
    //                                             libvlc_video_display_cb display,
    //                                             void *opaque )

    //設定Callback Function複製影像資訊，可藉由libvlc_video_set_format和libvlc_video_set_format_callbacks設定解碼資訊
    libvlc_video_set_callbacks( mp, lock, unlock, display, context );


    // LIBVLC_API void libvlc_video_set_format( libvlc_media_player_t *mp,
    //                                          const char *chroma,
    //                                          unsigned    width,
    //                                          unsigned    height,
    //                                          unsigned    pitch )

    const char *chroma = "RV24";
    unsigned width     = VIDEO_WIDTH;
    unsigned height    = VIDEO_HEIGHT;
    unsigned pitch     = VIDEO_WIDTH * 24 / 8;
    libvlc_video_set_format( mp, chroma, width, height, pitch);


    // LIBVLC_API int libvlc_media_player_play( libvlc_media_player_t *p_mi )

    libvlc_media_player_play(mp);


    while ( true ) {
        cv::waitKey( 1000 );
    }

    libvlc_media_player_stop( mp );
    libvlc_media_player_release( mp );
    libvlc_release( vlcInstance );

    return 0;
}
