/*
 * ZoneminderMessage.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#include "ZoneminderMessage.hpp"

namespace zoneminder {

ZoneminderMessage::ZoneminderMessage()
{
}

bool ZoneminderMessage::isConnectionSuccessful(string sResponseStatus)
{
	bool iResult = false;

	if(sResponseStatus.find("HTTP/")!= string::npos && sResponseStatus.find("200")!= string::npos)
	{
		iResult = true;
	}
	return iResult;
}

ZoneminderMessage::~ZoneminderMessage()
{
}

} /* namespace zoneminder */
