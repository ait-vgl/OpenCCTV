/*
 * Stream.hpp
 *
 *  Created on: Oct 27, 2014
 *      Author: lakindu
 */

#ifndef STREAM_HPP_
#define STREAM_HPP_

#include <string>

using namespace std;

namespace db
{
class Stream
{
private:
	unsigned int _iId;
	unsigned int _iWidth;
	unsigned int _iHeight;
	bool _bKeepAspectRatio;
	bool _bAllowupSizing;
	int _iCompressionRate;
	unsigned int _iCameraId;
public:
	Stream();
	~Stream();
	void setId(unsigned int iId);
	unsigned int getId() const;
	void setWidth(unsigned int iWidth);
	unsigned int getWidth() const;
	void setHeight(unsigned int iHeight);
	unsigned int getHeight() const;
	void setKeepAspectRatio(bool bKeepAspectRatio);
	bool getKeepAspectRatio() const;
	void setAllowupSizing(bool bAllowupSizing);
	bool getAllowupSizing() const;
	void setCompressionRate(int iCompressionRate);
	int getCompressionRate() const;
	void setCamerId(unsigned int iCameraId);
	unsigned int getCameraId() const;
};
} /* namespace db */

#endif /* STREAM_HPP_ */
