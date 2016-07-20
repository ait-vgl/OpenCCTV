/*
 * MilestoneMessage.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: lakindu
 */
#include "MilestoneMessage.hpp"

namespace milestone
{
string MilestoneMessage::getConnectMessage(int iRequestId, const string& sUsername, const string& sPassword, const string& sCameraId, int iWidth, int iHeight, bool bKeepAspectRatio, bool bAllowUpsizing)
{
	ptree tree;
	string sRequestId = boost::lexical_cast<std::string>(iRequestId);
	tree.put("methodcall.requestid", sRequestId);
	tree.put("methodcall.methodname", "connect");
	tree.put("methodcall.username", sUsername);
	tree.put("methodcall.password", sPassword);
	tree.put("methodcall.cameraid", sCameraId);
	tree.put("methodcall.alwaysstdjpeg", "yes");
	tree.put("methodcall.transcode.width", iWidth);
	tree.put("methodcall.transcode.height", iHeight);
	if(bKeepAspectRatio)
	{
		tree.put("methodcall.transcode.keepaspectratio", "yes");
	}
	else
	{
		tree.put("methodcall.transcode.keepaspectratio", "no");
	}
	if(bAllowUpsizing)
	{
		tree.put("methodcall.transcode.allowupsizing", "yes");
	}
	else
	{
		tree.put("methodcall.transcode.allowupsizing", "no");
	}
	std::ostringstream oss;
	try
	{
		write_xml(oss, tree);
	}
	catch(boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::cerr << "MilestoneMessage:getConnectMessage: "<<e.what() << std::endl;
		return oss.str();
	}
	string connectMessage = oss.str();

	//cout << connectMessage << endl;

	connectMessage.append("\r\n\r\n");
	return connectMessage;
}

string MilestoneMessage::getLiveMessage(int iRequestId, int iCompressionRate)
{
	ptree tree;
	string sRequestId = static_cast<ostringstream*>( &(ostringstream() << iRequestId) )->str();
	tree.put("methodcall.requestid", sRequestId);
	tree.put("methodcall.methodname", "live");
	tree.put("methodcall.compressionrate", iCompressionRate);
	std::ostringstream oss;
	try
	{
		write_xml(oss, tree);
	}
	catch(boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::cerr << "MilestoneMessage:getLiveMessage: " << e.what() << std::endl;
		return oss.str();
	}
	string liveMessage = oss.str();
	liveMessage.append("\r\n\r\n");
	return liveMessage;
}

string MilestoneMessage::getLiveMessage(int iRequestId)
{
	ptree tree;
	string sRequestId = static_cast<ostringstream*>(&(ostringstream()
			<< iRequestId))->str();
	tree.put("methodcall.requestid", sRequestId);
	tree.put("methodcall.methodname", "live");
	std::ostringstream oss;
	try {
		write_xml(oss, tree);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::cerr << "MilestoneMessage:getLiveMessage: " <<e.what() << std::endl;
		return oss.str();
	}
	string liveMessage = oss.str();
	liveMessage.append("\r\n\r\n");
	return liveMessage;
}

bool MilestoneMessage::isConnectionSuccessful(string connectResponse)
{
	string connectionSucessfulStr = "yes";
	ptree pt;
	istringstream iss(connectResponse);
	try
	{
		read_xml(iss, pt);
	}
	catch(boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::cerr << "MilestoneMessage:isConnectionSuccessful: "<<e.what() << std::endl;
		return false;
	}
	string connected;
	try
	{
	connected = pt.get<string>("methodresponse.connected");
	}
	catch(boost::property_tree::ptree_bad_path &e)
	{
		std::cerr << "MilestoneMessage:isConnectionSuccessful: "<< e.what() << std::endl;
		return false;
	}
	trim(connected);
	if(connectionSucessfulStr.compare(connected) == 0)
	{
		return true;
	}
	return false;
}
}
