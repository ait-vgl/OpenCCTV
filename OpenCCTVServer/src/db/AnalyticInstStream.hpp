/*
 * AnalyticInstStream.hpp
 *
 *  Created on: Oct 28, 2014
 *      Author: anjana
 */

#ifndef ANALYTICINSTTREAM_HPP_
#define ANALYTICINSTTREAM_HPP_

#include <string>

using namespace std;

namespace db {

class AnalyticInstStream {
private:

	unsigned int _iId;
	unsigned int _iAnalyticInstanceId;
	unsigned int _iStreamId;
	string _sInputName;
	unsigned int _iAnalyticId;
	string _sLocation;

public:
	AnalyticInstStream();
	virtual ~AnalyticInstStream();

	unsigned int getAnalyticId() const;
	void setAnalyticId(unsigned int analyticId);
	unsigned int getAnalyticInstanceId() const ;
	void setAnalyticInstanceId(unsigned int analyticInstanceId);
	unsigned int getId() const ;
	void setId(unsigned int id) ;
	unsigned int getStreamId() const ;
	void setStreamId(unsigned int streamId) ;
	const string& getInputName() const ;
	void setInputName(const string& inputName);
	const string& getLocation() const;
	void setLocation(const string& location);
};

} /* namespace db */

#endif /* ANALYTICINSTSTREAM_HPP_ */
