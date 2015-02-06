/*
 * AnalyticServerMessage.hpp
 *
 *  Created on: Oct 11, 2014
 *      Author: lakindu
 */

#ifndef ANALYTICSERVERMESSAGE_HPP_
#define ANALYTICSERVERMESSAGE_HPP_

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;
using boost::property_tree::ptree;
using namespace boost::algorithm;

class AnalyticServerMessage
{
public:
	static string getStartAnalyticRequest(const string& analyticLocation, unsigned int analyticInstanceId, unsigned int analyticId, unsigned int streamId);
	static string getStartAnalyticReply(const string& analyticQueueInAddress,const string& analyticQueueOutAddress);
	static void extractStartAnalyticRequestData(const string& startAnalyticRequest, string& analyticLocation, unsigned int& analyticInstanceId, unsigned int& analyticId, unsigned int& streamId);
	static void getAnalyticQueueAddress(const string& sStartAnalyticReply, string& sAnalyticQueueInAddress, string& sAnalyticQueueOutAddress);
};

#endif /* ANALYTICSERVERMESSAGE_HPP_ */
