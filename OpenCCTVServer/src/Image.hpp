/*
 * Image.hpp
 *
 *  Created on: Oct 9, 2014
 *      Author: lakindu
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <vector>
#include <string>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

class Image
{
private:
	unsigned int _iWidth;
	unsigned int _iHeight;
	vector<char> _vImageData;
	unsigned int _iStreamId;
	string _sInputName;
	string _sTimestamp;
	string _sResult;

	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive & archive, const unsigned version)
	{
		archive & const_cast<unsigned int&>(_iWidth) & const_cast<unsigned int&>(_iHeight) & _vImageData & const_cast<unsigned int&>(_iStreamId) & _sInputName & _sTimestamp & _sResult;
	}

public:
	Image();
	Image(unsigned int iWidth, unsigned int iHeight, vector<char>& vImageData, unsigned int iStreamId, const string& sTimestamp, const string& sResult);
	~Image();

	unsigned int getHeight() const;
	void setHeight(unsigned int height);
	unsigned int getStreamId() const;
	void setStreamId(unsigned int streamId);
	unsigned int getWidth() const;
	void setWidth(unsigned int width);
	const string& getInputName() const;
	void setInputName(const string& inputName);
	const string& getTimestamp() const;
	void setTimestamp(const string& timestamp);
	const vector<char>& getImageData() const;
	void setImageData(const vector<char>& imageData);
	const string& getResult() const;
	void setResult(const string& result);
};

#endif /* IMAGE_HPP_ */
