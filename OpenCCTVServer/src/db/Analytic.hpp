/*
 * CameraAnalytic.hpp
 *
 *  Created on: Sep 17, 2014
 *      Author: Anjana Tissera
 */

#ifndef CAMERAANALYTIC_HPP_
#define CAMERAANALYTIC_HPP_

#include <string>

using namespace std;

namespace db
{
/**
 * Class to hold analytic information from the database
 */
class Analytic
{
private:
	unsigned int analyticId;
	string  location;

public:
	Analytic();
	Analytic(unsigned int analyticId, const string& location);
	~Analytic();
	unsigned int getAnalyticId() const;
	void setAnalyticId(unsigned int analyticId);
	const string& getLocation() const;
	void setLocation(const string& path);
};
} /* namespace db */

#endif /* CAMERAANALYTIC_HPP_ */
