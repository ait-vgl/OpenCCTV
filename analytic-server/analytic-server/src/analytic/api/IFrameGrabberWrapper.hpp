#ifndef IFRAMEGRABBERWRAPPER_H_
#define IFRAMEGRABBERWRAPPER_H_

#include <opencv2/core/core.hpp>
#include "IFrameGrabber.hpp"

using namespace std;
using namespace cv;

namespace analytic {
namespace api {

class IFrameGrabberWrapper
{
public:	
	 
	IFrameGrabberWrapper(){}
	virtual bool init(analytic::util::log::Logger *pAnalyticLog) = 0; // 1
	virtual bool setDefaultConfig(FileStorage config) = 0; // 2
	virtual bool setStreamConfig(string sUrl, int iWidth, int iHeight, int iFps) = 0; //3
	virtual bool setStreamId(unsigned int iStreamId) = 0;
    virtual IFrameGrabber* getFrameGrabberObj() = 0; //4
	virtual ~IFrameGrabberWrapper(){}
};

} /* namespace api */
} /* namespace analytic */
#endif /* IFRAMEGRABBERWRAPPER_H_ */
