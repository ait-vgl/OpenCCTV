/*
 * AnalyticOutputMessage.cpp
 *
 *  Created on: Nov 5, 2014
 *      Author: anjana
 */

#include "AnalyticOutputMessage.hpp"

namespace util {

AnalyticOutputMessage::AnalyticOutputMessage() {
	// TODO Auto-generated constructor stub

}

string AnalyticOutputMessage::getAnalyticLibResult(const string& sAnalyticResult){
		string result = "";
		ptree pt;
		istringstream iss(sAnalyticResult);
		try
		{
			read_xml(iss, pt);
			result = pt.get<string>("analyticOutput.result");
		}
		catch(boost::property_tree::xml_parser::xml_parser_error &e)
		{
			std::cerr << e.what() << std::endl;
		}

		return result;
}

string AnalyticOutputMessage::getAnalyticResult(const int iStreamId, const int iAnalyticInstId, const int iAnalyticId, const string& sImgTimestamp, const string& sResult)
{
	string message = "";
	ptree pt;
	pt.put("analyticOutput.messagetype", "analyticresult");
	pt.put("analyticOutput.streamid", iStreamId);
	pt.put("analyticOutput.analyticinstanceid", iAnalyticInstId);
	pt.put("analyticOutput.analyticid", iAnalyticId);
	pt.put("analyticOutput.imgtimestamp", sImgTimestamp);
	pt.put("analyticOutput.result", sResult);

	std::ostringstream oss;

	try {
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::cerr << e.what() << std::endl;
		return oss.str();
	}

	message = oss.str();
	return message;
}

void AnalyticOutputMessage::getAnalyticResultsData(const string& sAnalyticResultStr, int& iStreamId, int& iAnalyticInstId, int& iAnalyticId, string& sImgTimestamp, string& sResult )
{
	ptree pt;
	istringstream iss(sAnalyticResultStr);
	try {
		read_xml(iss, pt);

		iStreamId = pt.get<unsigned int>( "analyticOutput.streamid");
		iAnalyticInstId = pt.get<unsigned int>( "analyticOutput.analyticinstanceid");
		iAnalyticId = pt.get<unsigned int>( "analyticOutput.analyticid");
		sImgTimestamp = pt.get<string>("analyticOutput.imgtimestamp");
		trim (sImgTimestamp);
		sResult = pt.get<string>("analyticOutput.result");
		trim (sResult);

	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::cerr << e.what() << std::endl;
	}
}

AnalyticOutputMessage::~AnalyticOutputMessage() {
	// TODO Auto-generated destructor stub
}

} /* namespace util */
