/*
 * ZoneminderMessage.hpp
 *
 *  Created on: Feb 4, 2015
 *      Author: anjana
 */

#ifndef ZONEMINDERMESSAGE_HPP_
#define ZONEMINDERMESSAGE_HPP_

#include <iostream>
#include<string>

using namespace std;

namespace zoneminder {

class ZoneminderMessage {
public:
	ZoneminderMessage();
	static bool isConnectionSuccessful(string sResponseStatus);
	virtual ~ZoneminderMessage();
};
} /* namespace zoneminder */

#endif /* ZONEMINDERMESSAGE_HPP_ */
