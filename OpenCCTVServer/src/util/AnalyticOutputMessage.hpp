/*
 * AnalyticOutputMessage.hpp
 *
 *  Created on: Nov 5, 2014
 *      Author: anjana
 */

#ifndef ANALYTICOUTPUTMESSAGE_HPP_
#define ANALYTICOUTPUTMESSAGE_HPP_

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;
using boost::property_tree::ptree;
using namespace boost::algorithm;

namespace util {

class AnalyticOutputMessage {
public:
	AnalyticOutputMessage();
	virtual ~AnalyticOutputMessage();

	static string getAnalyticLibResult(const string& sAnalyticResult);
	static string getAnalyticResult(const int iStreamId, const int iAnalyticInstId, const int iAnalyticId, const string& sImgTimestamp, const string& sResult );
	static void getAnalyticResultsData(const string& sAnalyticResultStr, int& iStreamId, int& iAnalyticInstId, int& iAnalyticId, string& sImgTimestamp, string& sResult );

};

} /* namespace util */

#endif /* ANALYTICOUTPUTMESSAGE_HPP_ */
