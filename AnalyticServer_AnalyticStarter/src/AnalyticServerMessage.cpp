/*
 * AnalyticServerMessage.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: lakindu
 */

#include "AnalyticServerMessage.hpp"

string AnalyticServerMessage::getStartAnalyticRequest(const string& analyticLocation, unsigned int analyticInstanceId, unsigned int analyticId, unsigned int streamId)
{
	ptree pt;
	pt.put("opencctvrequest.operation", "startanalytic");
	pt.put("opencctvrequest.analyticlocation", analyticLocation);
	pt.put("opencctvrequest.analyticinstanceid", analyticInstanceId);
	pt.put("opencctvrequest.analyticid", analyticId);
	pt.put("opencctvrequest.streamid", streamId);
	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::cerr << e.what() << std::endl;
		return oss.str();
	}
	string message = oss.str();
	return message;
}

string AnalyticServerMessage::getStartAnalyticReply(const string& analyticQueueInAddress,const string& analyticQueueOutAddress)
{
	ptree pt;
	pt.put("analyticserverreply.operation", "startanalytic");
	pt.put("analyticserverreply.analyticqueueinaddress", analyticQueueInAddress);
	pt.put("analyticserverreply.analyticqueueoutaddress", analyticQueueOutAddress);
	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::cerr << e.what() << std::endl;
		return oss.str();
	}
	string message = oss.str();
	return message;
}

 void AnalyticServerMessage::extractStartAnalyticRequestData(const string& startAnalyticRequest, string& analyticLocation, unsigned int& analyticInstanceId, unsigned int& analyticId, unsigned int& streamId)
{
	ptree pt;
	istringstream iss(startAnalyticRequest);
	try
	{
		read_xml(iss, pt);
	}
	catch(boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::cerr << e.what() << std::endl;
	}
	try
	{
		analyticLocation = pt.get<string>("opencctvrequest.analyticlocation");
		trim(analyticLocation);
		analyticInstanceId = pt.get<unsigned int>("opencctvrequest.analyticinstanceid");
		analyticId = pt.get<unsigned int>("opencctvrequest.analyticid");
		streamId = pt.get<unsigned int>("opencctvrequest.streamid");
	}
	catch(boost::property_tree::ptree_bad_path &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void AnalyticServerMessage::getAnalyticQueueAddress(const string& sStartAnalyticReply, string& sAnalyticQueueInAddress, string& sAnalyticQueueOutAddress)
{
	ptree pt;
	istringstream iss(sStartAnalyticReply);
	try
	{
		read_xml(iss, pt);
		sAnalyticQueueInAddress = pt.get<string>("analyticserverreply.analyticqueueinaddress");
		sAnalyticQueueOutAddress = pt.get<string>("analyticserverreply.analyticqueueoutaddress");
	}
	catch(boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::cerr << "AnalyticServerMessage:getAnalyticQueueAddress: " << e.what() << std::endl;
	}

	trim(sAnalyticQueueInAddress);
	trim(sAnalyticQueueOutAddress);
}
