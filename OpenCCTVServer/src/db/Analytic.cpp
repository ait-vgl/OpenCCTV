/*
 * CameraAnalytic.cpp
 *
 *  Created on: Sep 17, 2014
 *      Author: Anjana Tissera
 */

#include "Analytic.hpp"

namespace db
{
Analytic::Analytic()
{
	analyticId = 0;
	location = "";
}

Analytic::Analytic(unsigned int analyticId, const string& location ) {
	this->analyticId = analyticId;
	this->location = location;
}

Analytic::~Analytic()
{
}

unsigned int Analytic::getAnalyticId() const
{
	return analyticId;
}

void Analytic::setAnalyticId(unsigned int analyticId)
{
	this->analyticId = analyticId;
}

const string& Analytic::getLocation() const {
	return location;
}

void Analytic::setLocation(const string& path) {
	this->location = path;
}
} /* namespace db */
