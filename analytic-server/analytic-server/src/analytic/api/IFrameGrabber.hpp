#ifndef IFRAMEGRABBER_H_
#define IFRAMEGRABBER_H_

namespace analytic {
namespace api {

class IFrameGrabber
{

protected:
    unsigned int _iStreamId;
    int _iStreamType;


public:
    unsigned int getStreamId()
    {
        return _iStreamId;
    }

    void setStreamId(unsigned int iStreamId)
    {
        this->_iStreamId = iStreamId;
    }

    int getStreamType()
    {
        return _iStreamType;
    }

    void setStreamType(int iStreamType)
    {
        this->_iStreamType = iStreamType;
    }

    IFrameGrabber() {
    }
    //virtual void init() = 0; // 1
    virtual ~IFrameGrabber() {}
};

} /* namespace api */
} /* namespace analytic */
#endif /* IFRAMEGRABBER_H_ */
