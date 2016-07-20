
#ifndef OPENCCTV_IMAGE_HPP_
#define OPENCCTV_IMAGE_HPP_

#include <vector>
#include <string>
/* for boost serialization */
/* boost library -lboost_serialization */
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/serialization/vector.hpp>p>

namespace opencctv {

class Image
{
private:
	std::vector<unsigned char> _vImageData;
	unsigned int _iWidth;
	unsigned int _iHeight;
	unsigned int _iStreamId;
	std::string _sInputName;
	std::string _sTimestamp;

	/*
	//for boost serialization
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & archive, const unsigned version)
	{
		archive & _vImageData & _iWidth & _iHeight & _iStreamId & _sInputName & _sTimestamp;
	}
	*/

public:
	Image();
	Image(std::vector<unsigned char>& vImageData,  unsigned int iImageWidth, unsigned int iImageHeight, const std::string& sTimestamp);

	unsigned int getStreamId() const;
	void setStreamId(unsigned int streamId);
	unsigned int getWidth() const;
	void setWidth(unsigned int iImageWidth);
	unsigned int getHeight() const;
	void setHeight(unsigned int iImageHeight);
	const std::string& getInputName() const;
	void setInputName(const std::string& inputName);
	const std::string& getTimestamp() const;
	void setTimestamp(const std::string& timestamp);
	const std::vector<unsigned char>& getImageData() const;
	void setImageData(const std::vector<unsigned char>& imageData);
};

} // namespace opencctv
#endif // OPENCCTV_IMAGE_HPP_
