/*
 * OpenCCTVServerMessage.cpp
 *
 *  Created on: Jun 1, 2015
 *      Author: anjana
 */

#include "OpenCCTVServerMessage.hpp"

namespace opencctv {
namespace util {
namespace xml {

/**
 * All the messages sent to OpenCCTV server will have the following format
 *

  <?xml version="1.0" encoding="utf-8"?>
  <opencctvmsg>
  <type>MessageType</type>
  <!-- Other tags as required by different messages types -->
  </opencctvmsg>

 *
 */

int OpenCCTVServerMessage::extractMessageDetails(const string& sRequest, string& sMessageType)
{
	int result = 1;

	ptree pt;
	istringstream iss(sRequest);

	try
	{
		read_xml(iss, pt);
		sMessageType = pt.get<string>("opencctvmsg.type");
		trim(sMessageType);
	} catch(boost::property_tree::xml_parser::xml_parser_error &e)
	{
		result = -1;
		string sMessage = "OpenCCTV Server: XML parsing error ";
		throw opencctv::Exception(sMessage.append(e.what()));
	} catch(boost::property_tree::ptree_bad_path &e)
	{
		result = -1;
		string sMessage = "OpenCCTV Server: XML parsing error ";
		throw opencctv::Exception(sMessage.append(e.what()));
	}

	return result;
}

int  OpenCCTVServerMessage::createStatusReply(const string& sContent, const string serverStatus, const pid_t serverProcessId, string& sReplyMessage )
{
	int result = 1;
	sReplyMessage = "";

	ptree pt;
	pt.put("opencctvmsg.type", "StatusReply");
	pt.put("opencctvmsg.content", sContent);
	pt.put("opencctvmsg.serverstatus", serverStatus);
	pt.put("opencctvmsg.serverpid", serverProcessId);

	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
		sReplyMessage = oss.str();
	}
	catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		result = -1;
		string sMessage = "OpenCCTV Server: XML parsing error ";
		throw opencctv::Exception(sMessage.append(e.what()));
	}

	return result;
}

int OpenCCTVServerMessage::createInvalidMessageReply(const string& sContent, const string serverStatus, const pid_t serverProcessId, string& sReplyMessage )
{
	int result = 1;
	sReplyMessage = "";

	ptree pt;
	pt.put("opencctvmsg.type", "Error");
	pt.put("opencctvmsg.content", sContent);
	pt.put("opencctvmsg.serverstatus", serverStatus);
	pt.put("opencctvmsg.serverpid", serverProcessId);

	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
		sReplyMessage = oss.str();
	}
	catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		result = -1;
		string sMessage = "OpenCCTV Server: XML parsing error ";
		throw opencctv::Exception(sMessage.append(e.what()));
	}

	return result;
}

int OpenCCTVServerMessage::createStartMessageReply(const string& sContent, const string serverStatus, const pid_t serverProcessId, string& sReplyMessage)
{
	int result = 1;

	sReplyMessage = "";

	ptree pt;
	pt.put("opencctvmsg.type", "StartReply");
	pt.put("opencctvmsg.content", sContent);
	pt.put("opencctvmsg.serverstatus", serverStatus);
	pt.put("opencctvmsg.serverpid", serverProcessId);

	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
		sReplyMessage = oss.str();
	}
	catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		result = -1;
		string sMessage = "OpenCCTV Server: XML parsing error ";
		throw opencctv::Exception(sMessage.append(e.what()));
	}

	return result;

}

int OpenCCTVServerMessage::createStopMessageReply(const string& sContent, const string serverStatus, const pid_t serverProcessId, string& sReplyMessage)
{
	int result = 1;

	sReplyMessage = "";

	ptree pt;
	pt.put("opencctvmsg.type", "StopReply");
	pt.put("opencctvmsg.content", sContent);
	pt.put("opencctvmsg.serverstatus", serverStatus);
	pt.put("opencctvmsg.serverpid", serverProcessId);

	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
		sReplyMessage = oss.str();
	}
	catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		result = -1;
		string sMessage = "OpenCCTV Server: XML parsing error ";
		throw opencctv::Exception(sMessage.append(e.what()));
	}

	return result;
}

} /* namespace xml */
} /* namespace util */
} /* namespace opencctv */
