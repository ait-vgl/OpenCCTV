#include "AnalyticMessage.hpp"

namespace analytic {
namespace xml {

void AnalyticMessage::extractInitialDetails(const std::string& sAnalyticRequest, unsigned int& iServerId, std::string& sOperation)
{
	sOperation = "";
	iServerId = 0;
	boost::property_tree::ptree pt;
	std::istringstream iss(sAnalyticRequest);
	try
	{
		read_xml(iss, pt);
		iServerId = pt.get<unsigned int>("analyticrequest.serverid");
		sOperation = pt.get<std::string>("analyticrequest.operation");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to parse server Id and operation details form the request. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	boost::algorithm::trim (sOperation);
}

std::string AnalyticMessage::getServerStatusReply(const std::string& sMessage, const std::string& sStatus, const int iPid)
{
	boost::property_tree::ptree pt;
	pt.put("analyticreply.type", OPERATION_ANALYTIC_SERVER_STATUS);
	pt.put("analyticreply.content", sMessage);
	pt.put("analyticreply.serverstatus", sStatus);
	pt.put("analyticreply.serverpid", iPid);
	std::ostringstream oss;

	try
	{
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to generate Analytic Server Status Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}

std::string AnalyticMessage::getKillAllAnalyticsReply(const std::string& sMessage,
		const std::string& sServerStatus,const int iPid)
{
	boost::property_tree::ptree pt;
	pt.put("analyticreply.type", OPERATION_KILL_ALL_ANALYTICS);
	pt.put("analyticreply.content", sMessage);
	pt.put("analyticreply.serverstatus", sServerStatus);
	pt.put("analyticreply.serverpid", iPid);
	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to generate Analytic Start Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}

void AnalyticMessage::extractKillAllAnalyticsReply(const std::string& sKillAllAnalyticsReply,
		std::string& sContent)
{
	sContent = "";
	boost::property_tree::ptree pt;
	std::istringstream iss(sKillAllAnalyticsReply);
	try
	{
		read_xml(iss, pt);
		sContent = pt.get<std::string>("analyticreply.content");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "AnalyticMessage::extractKillAllAnalyticsReply : ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}

/*
 *=======May 08 2017===========
 *======= Later Remove=========*/
/*std::string AnalyticMessage::getAnalyticStartReply(const std::string& sAnalyticQueueInAddress,
		const std::string& sAnalyticQueueOutAddress)
{
	boost::property_tree::ptree pt;
	pt.put("analyticreply.operation", "startanalytic");
	pt.put("analyticreply.analyticqueueinaddress", sAnalyticQueueInAddress);
	pt.put("analyticreply.analyticqueueoutaddress", sAnalyticQueueOutAddress);
	std::ostringstream oss;
	try {
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to generate Analytic Start Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}*/

std::string AnalyticMessage::getAnalyticStartReply(const std::string& sMessage,
		const std::string& sServerStatus,const int iPid)
{
	/*
	 * Example Message
	 * <?xml version="1.0" encoding="utf-8"?>
	 * <analyticreply><type>StartAnalytic</type>
	 * <content>Analytic instance 4 started</content>
	 * <serverstatus>Running</serverstatus>
	 * <serverpid>17287</serverpid></analyticreply>
	 */

	boost::property_tree::ptree pt;
	pt.put("analyticreply.type", OPERATION_START_ANALYTIC);
	pt.put("analyticreply.content", sMessage);
	pt.put("analyticreply.serverstatus", sServerStatus);
	pt.put("analyticreply.serverpid", iPid);
	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to generate Analytic Start Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}

void AnalyticMessage::extractAnalyticStartRequestData(const std::string& sAnalyticStartRequest,
		unsigned int& iAnalyticInstanceId, std::string& sAnalyticFilename)
{
	boost::property_tree::ptree pt;
	std::istringstream iss(sAnalyticStartRequest);
	iAnalyticInstanceId = -1;
	sAnalyticFilename = "";

	try
	{
		read_xml(iss, pt);
		iAnalyticInstanceId = pt.get<unsigned int>("analyticrequest.analyticinstanceid");
		sAnalyticFilename = pt.get<std::string>("analyticrequest.analyticfilename");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to parse Analytic Start Request. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}

	if(iAnalyticInstanceId <= 0)
	{
		std::string sErrMsg = "Invalid analytic instance id";
		throw opencctv::Exception(sErrMsg);
	}

	if(sAnalyticFilename.empty())
	{
		std::string sErrMsg = "Invalid analytic name";
		throw opencctv::Exception(sErrMsg);
	}
}

void AnalyticMessage::extractAnalyticStopRequestData(const std::string& sAnalyticStopRequest,
		unsigned int& iAnalyticInstanceId)
{
	boost::property_tree::ptree pt;
	std::istringstream iss(sAnalyticStopRequest);
	try
	{
		read_xml(iss, pt);
		iAnalyticInstanceId = pt.get<unsigned int>("analyticrequest.analyticinstanceid");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to parse Analytic Stop Request. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}


std::string AnalyticMessage::getStopAnalyticProcessesReply(bool bDone)
{
	boost::property_tree::ptree pt;
	pt.put("analyticreply.operation", OPERATION_STOP_ANALYTIC);
	pt.put("analyticreply.done", bDone);
	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to generate Stop Analytic Processs Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}

std::string AnalyticMessage::getAnalyticStopReply(const std::string& sMessage,
		const std::string& sServerStatus,const int iPid)
{
	boost::property_tree::ptree pt;
	pt.put("analyticreply.type", OPERATION_STOP_ANALYTIC);
	pt.put("analyticreply.content", sMessage);
	pt.put("analyticreply.serverstatus", sServerStatus);
	pt.put("analyticreply.serverpid", iPid);
	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to generate Analytic Start Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}

std::string AnalyticMessage::getErrorReply(const std::string& sErrorMessage,
		const std::string& sServerStatus,const int iPid)
{
	/*
	 * Example
	 * <?xml version="1.0" encoding="utf-8"?>
	 * <analyticreply>
	 * <type>Error</type>
	 * <content>Failed to start analytic instance 4</content>
	 * </analyticreply>
	 */
	std::stringstream ossReply;
	ossReply << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	ossReply << "<analyticreply>";
	ossReply << "<type>Error</type>";
	ossReply << "<content>";
	ossReply <<	sErrorMessage;
	ossReply << "</content>";
	ossReply << "<serverstatus>";
	ossReply <<	sServerStatus;
	ossReply << "</serverstatus>";
	ossReply << "<serverpid>";
	ossReply <<	iPid;
	ossReply << "</serverpid>";
	ossReply << "</analyticreply>";

	std::string sReplyMessage = ossReply.str();
	return sReplyMessage;
}

} /* namespace xml */
} /* namespace analytic */
