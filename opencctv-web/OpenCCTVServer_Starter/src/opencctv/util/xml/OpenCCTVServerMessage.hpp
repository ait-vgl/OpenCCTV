/*
 * OpenCCTVServerMessage.hpp
 *
 *  Created on: Jun 1, 2015
 *      Author: anjana
 */

#ifndef OPENCCTVSERVERMESSAGE_HPP_
#define OPENCCTVSERVERMESSAGE_HPP_

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "../TypeDefinitions.hpp"
#include "../../Exception.hpp"

using namespace std;
using boost::property_tree::ptree;
using namespace boost::algorithm;

namespace opencctv {
namespace util {
namespace xml {

class OpenCCTVServerMessage {
public:
	int static extractMessageDetails(const string& sRequest, string& sMessageType);
	int static createInvalidMessageReply(const string& sContent, const string serverStatus, const pid_t serverProcessId, string& sReplyMessage );
	int static createStartMessageReply(const string& sContent, const string serverStatus, const pid_t serverProcessId, string& sReplyMessage);
	int static createStopMessageReply(const string& sContent, const string serverStatus, const pid_t serverProcessId, string& sReplyMessage);
	int static createStatusReply(const string& sContent, const string serverStatus, const pid_t serverProcessId, string& sReplyMessage);
};

} /* namespace xml */
} /* namespace util */
} /* namespace opencctv */

#endif /* OPENCCTVSERVERMESSAGE_HPP_ */
