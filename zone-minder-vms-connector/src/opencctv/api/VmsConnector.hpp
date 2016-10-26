/*
 * VMS connector plugins must extend this class and implement all the abstract functions of this class.
 */

#ifndef OPENCCTV_VMSCONNECTOR_HPP_
#define OPENCCTV_VMSCONNECTOR_HPP_

#include <string> // std::string
#include <fstream> // to read files
#include "../ConcurrentQueue.hpp"
#include "../Image.hpp"

namespace opencctv {
namespace api {

typedef struct
{
	std::string sServerIp; // VMS IP
	unsigned short int iServerPort; // VMS port
	std::string sUsername; // VMS username
	std::string sPassword; // VMS password
	std::string sCameraId; // Camera ID from VMS
	unsigned int iWidth; // in pixels
	unsigned int iHeight; // in pixels
	/* These properties are used in Milestone XProtect VMS */
	bool bKeepAspectRatio;
	bool bAllowUpsizing;
	unsigned short int iCompressionRate;
} VmsConnectInfo;

class VmsConnector
{
protected:
	bool _bEnable;
public:
	VmsConnector()
	{
		_bEnable = false;
	}
	virtual ~VmsConnector(){}
	virtual bool init(const VmsConnectInfo& info, const std::string sPathToVmsConnectorDir)
	{
		/* Read a file from your dir */
		std::string sFilePath = sPathToVmsConnectorDir;
		sFilePath.append("/").append("some_filename");
		std::ifstream inFile(sFilePath.c_str());
		// then read file.
		// do some processing
		return false; // if init failed
	}
	virtual void produceImageObjects(opencctv::ConcurrentQueue<opencctv::Image>* pQueue)
	{
		// do some processing
		std::vector<unsigned char>* pVImage = new std::vector<unsigned char>();
		_bEnable = true;
		while(_bEnable)
		{
			// get image data from VMS
			// populate pVImage with image data
			opencctv::Image* pImage = new opencctv::Image(); // use new operator to construct Images
			/* setting following fields is compulsory */
			pImage->setWidth(720);
			pImage->setHeight(480);
			pImage->setTimestamp("Timestamp from VMS");
			pImage->setImageData(*pVImage);
		}
		/* don't forget to delete any memory allocated on the heap (used new operator),
		 * but NOT opencctv::Image objects stored in the opencctv::ConcurrentQueue. */
		if(pVImage) delete pVImage;
	}
	virtual bool isStillProducingImages() = 0;
	virtual void stop()
	{
		_bEnable = false;
	}
};

} // namespace api
} // namespace opencctv

#endif // OPENCCTV_VMSCONNECTOR_HPP_

/*
 * In YOUR_CONNECTOR_CLASS.hpp include these two functions.
 *
extern "C" YOUR_CONNECTOR_CLASS* create() {
    return new YOUR_CONNECTOR_CLASS();
}

extern "C" void destroy(YOUR_CONNECTOR_CLASS* p) {
    if(p) {
    	// do whatever you want to close everything
    	delete p;
    }
}
*/
