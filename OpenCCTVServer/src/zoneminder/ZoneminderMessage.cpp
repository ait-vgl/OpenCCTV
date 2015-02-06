/*
 * ZoneminderMessage.cpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#include "ZoneminderMessage.hpp"

namespace zoneminder {

ZoneminderMessage::ZoneminderMessage() {
	// TODO Auto-generated constructor stub

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

ZoneminderMessage::~ZoneminderMessage() {
	// TODO Auto-generated destructor stub
}

} /* namespace zoneminder */
