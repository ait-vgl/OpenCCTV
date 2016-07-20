/*
 * MilestoneMessage.hpp
 *
 *  Created on: Sep 23, 2014
 *      Author: lakindu
 */
#ifndef MILESTONEMESSAGE_HPP_
#define MILESTONEMESSAGE_HPP_

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;
using boost::property_tree::ptree;
using namespace boost::algorithm;

namespace milestone
{
class MilestoneMessage
{
public:
	/**
	 * Gets XML connect message used to connect to a particular camera through Milestone tcp/ip server.
	 */
	static string getConnectMessage(int iRequestid, const string& sUsername, const string& sPassword, const string& sCameraid, int iWidth, int iHeight, bool bKeepAspectRatio, bool bAllowUpsizing);
	/**
	 * Gets XML live message used to get live video stream
	 * from the connected camera through Milestone tcp/ip server.
	 */
	static string getLiveMessage(int iRequestId, int iCompressionRate);
	/**
	 * Checks a given response for Milestone connect message,
	 * whether the connection is successful or not.
	 */
	static bool isConnectionSuccessful(string connectResponse);

	static string getLiveMessage(int iRequestId);
};
}

#endif /* MILESTONEMESSAGE_HPP_ */
