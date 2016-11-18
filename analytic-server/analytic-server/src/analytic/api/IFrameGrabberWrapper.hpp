#ifndef IFRAMEGRABBERWRAPPER_H_
#define IFRAMEGRABBERWRAPPER_H_

#include <opencv2/core/core.hpp>
#include "FrameGrabber.h"

using namespace std;
using namespace cv;

namespace analytic {
namespace api {

class IFrameGrabberWrapper {
	public:
		IFrameGrabberWrapper() {}
		virtual void init() = 0;
		virtual void setDefaultConfig ( FileStorage config ) = 0;
		virtual void setStreamConfig ( string sUrl, int iWidth, int iHeight, int iFps ) = 0;
		virtual FrameGrabber *getFrameGrabberObj() = 0;
		virtual ~IFrameGrabberWrapper() {}
};

} /* namespace api */
} /* namespace analytic */
#endif /* IFRAMEGRABBERWRAPPER_H_ */
