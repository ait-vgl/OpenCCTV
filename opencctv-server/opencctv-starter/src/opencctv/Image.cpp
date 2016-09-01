
#include "Image.hpp"

namespace opencctv {

Image::Image()
{
	_iStreamId = 0;
	_iWidth = 0;
	_iHeight = 0;
}

Image::Image(std::vector<unsigned char>& vImageData, unsigned int iImageWidth, unsigned int iImageHeight, const std::string& sTimestamp)
{
	_iStreamId = 0;
	_vImageData = vImageData;
	_sTimestamp = sTimestamp;
	_iWidth = iImageWidth;
	_iHeight = iImageHeight;
}

unsigned int Image::getStreamId() const {
	return _iStreamId;
}

void Image::setStreamId(unsigned int streamId) {
	_iStreamId = streamId;
}

unsigned int Image::getWidth() const
{
	return _iWidth;
}
void Image::setWidth(unsigned int iImageWidth)
{
	_iWidth = iImageWidth;
}
unsigned int Image::getHeight() const
{
	return _iHeight;
}
void Image::setHeight(unsigned int iImageHeight)
{
	_iHeight = iImageHeight;
}

const std::string& Image::getInputName() const {
	return _sInputName;
}

void Image::setInputName(const std::string& inputName) {
	_sInputName = inputName;
}

const std::string& Image::getTimestamp() const {
	return _sTimestamp;
}

void Image::setTimestamp(const std::string& timestamp) {
	_sTimestamp = timestamp;
}

const std::vector<unsigned char>& Image::getImageData() const {
	return _vImageData;
}

void Image::setImageData(const std::vector<unsigned char>& imageData) {
//	std::copy(imageData.begin(), imageData.end(), _vImageData.begin());
	_vImageData = imageData;
}

} //namespace opencctv
