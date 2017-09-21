#include "AnalyticMessage.hpp"

namespace analytic {
namespace xml {

std::string AnalyticMessage::extractAnalyticRequestOperation(const std::string& sAnalyticRequest)
{
	std::string sRet;
	boost::property_tree::ptree pt;
	std::istringstream iss(sAnalyticRequest);
	try
	{
		read_xml(iss, pt);
		sRet = pt.get<std::string>("analyticrequest.operation");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to parse operation details form the request. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	boost::algorithm::trim (sRet);
	return sRet;
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
/*
std::string AnalyticMessage::getKillAllAnalyticProcessesRequest()
{
	boost::property_tree::ptree pt;
	pt.put("analyticrequest.operation", OPERATION_KILL_ALL_ANALYTICS);
	std::ostringstream oss;
	try {
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to generate Kill All Analytic Processes Request. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}
*/

/*
 *=======May 08 2017===========
 *======= Later Remove=========*/
 std::string AnalyticMessage::getKillAllAnalyticProcessesReply(bool bDone)
{
	boost::property_tree::ptree pt;
	pt.put("analyticreply.operation", OPERATION_KILL_ALL_ANALYTICS);
	pt.put("analyticreply.done", bDone);
	std::ostringstream oss;
	try {
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to generate Kill All Analytic Processes Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}

std::string AnalyticMessage::getKillAllAnalyticProcessesReply(bool bDone, const std::string& sMessage)
{
	boost::property_tree::ptree pt;
	pt.put("analyticreply.operation", OPERATION_KILL_ALL_ANALYTICS);
	pt.put("analyticreply.done", bDone);
	pt.put("analyticreply.message", sMessage);
	std::ostringstream oss;
	try
	{
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to generate Kill All Analytic Processes Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}
/*
std::string AnalyticMessage::getAnalyticStartRequest(const unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation)
std::string AnalyticMessage::getAnalyticStartRequest(unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename) {
	boost::property_tree::ptree pt;
	pt.put("analyticrequest.operation", OPERATION_START_ANALYTIC);
	pt.put("analyticrequest.analyticinstanceid", iAnalyticInstanceId);
	pt.put("analyticrequest.analyticdirlocation", sAnalyticPluginDirLocation);
	pt.put("analyticrequest.analyticfilename", sAnalyticPluginFilename);
	std::ostringstream oss;
	try {
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to generate Analytic Start Request. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	return message;
}
*/

/*
 *=======May 08 2017===========
 *======= Later Remove=========*/
std::string AnalyticMessage::getAnalyticStartReply(const std::string& sAnalyticQueueInAddress,
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
}

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

/*void AnalyticMessage::extractAnalyticStartRequestData(const std::string& sAnalyticStartRequest,
		unsigned int& iAnalyticInstanceId, std::string& sAnalyticDirLocation)
{
	boost::property_tree::ptree pt;
	std::istringstream iss(sAnalyticStartRequest);
	try
	{
		read_xml(iss, pt);
		iAnalyticInstanceId = pt.get<unsigned int>("analyticrequest.analyticinstanceid");
		sAnalyticDirLocation = pt.get<std::string>("analyticrequest.analyticdirlocation");
		boost::algorithm::trim (sAnalyticDirLocation);
		//sAnalyticFilename = pt.get<std::string>("analyticrequest.analyticfilename");
		//boost::algorithm::trim (sAnalyticFilename);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to parse Analytic Start Request. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}*/

/*void AnalyticMessage::extractAnalyticStartRequestData(const std::string& sAnalyticStartRequest,
		unsigned int& iAnalyticInstanceId, std::string& sAnalyticDirLocation, std::string& sAnalyticFilename)
{
	boost::property_tree::ptree pt;
	std::istringstream iss(sAnalyticStartRequest);
	try
	{
		read_xml(iss, pt);
		iAnalyticInstanceId = pt.get<unsigned int>("analyticrequest.analyticinstanceid");
		sAnalyticDirLocation = pt.get<std::string>("analyticrequest.analyticdirlocation");
		sAnalyticFilename = pt.get<std::string>("analyticrequest.analyticfilename");
		boost::algorithm::trim (sAnalyticDirLocation);
		boost::algorithm::trim (sAnalyticFilename);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e)
	{
		std::string sErrMsg = "Failed to parse Analytic Start Request. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}*/

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

/*std::string AnalyticMessage::getErrorReply(const std::string& sType, const bool bDone,
		const std::string& sErrorMessage)
{

	 * Example
	 * <?xml version="1.0" encoding="utf-8"?>
	 * <analyticreply>
	 * <type>Error</type>
	 * <content>Failed to start analytic instance 4</content>
	 * <serverstatus>Running</serverstatus>
	 * <serverpid>17287</serverpid>
	 * </analyticreply>

	std::stringstream ossReply;
	ossReply << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
	ossReply << "<analyticreply>";
	ossReply << "<type>";
	ossReply <<	sType;
	ossReply << "</type>";
	ossReply << "<content>";
	ossReply <<	sErrorMessage;
	ossReply << "</content>";
	ossReply << "<serverstatus>serverstatus</serverstatus>"; //Remove
	ossReply << "<serverpid>0</serverpid>"; //Remove
	ossReply << "</analyticreply>";

	std::string sReplyMessage = ossReply.str();
	return sReplyMessage;
}*/

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


/*
void AnalyticMessage::extractAnalyticStartReplyData(const std::string& sAnalyticStartReply, std::string& sAnalyticQueueInAddress, std::string& sAnalyticQueueOutAddress)
{
	boost::property_tree::ptree pt;
	std::istringstream iss(sAnalyticStartReply);
	try {
		read_xml(iss, pt);
		sAnalyticQueueInAddress = pt.get<std::string>("analyticreply.analyticqueueinaddress");
		sAnalyticQueueOutAddress = pt.get<std::string>("analyticreply.analyticqueueoutaddress");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to parse Analytic Start Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	boost::algorithm::trim(sAnalyticQueueInAddress);
	boost::algorithm::trim(sAnalyticQueueOutAddress);
}

void AnalyticMessage::parseKillAllAnalyticProcessesReply(const std::string& sReply, bool& sDone)
{
	boost::property_tree::ptree pt;
	std::istringstream iss(sReply);
	try {
		read_xml(iss, pt);
		sDone = pt.get<bool>("analyticreply.done");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to parse Kill All Analytic Processes Reply. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
}

pid_t AnalyticMessage::getPid(const std::string& sPidMessage)
{
	pid_t pid = 0;
	boost::property_tree::ptree pt;
	std::istringstream iss(sPidMessage);
	try {
		read_xml(iss, pt);
		pid = pt.get<pid_t>("pid");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to parse PID message. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	return pid;
}


std::string AnalyticMessage::getPidMessage(pid_t pid)
{
	boost::property_tree::ptree pt;
	pt.put("pid", pid);
	std::ostringstream oss;
	try {
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to generate PID message. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	boost::replace_all(message, "\n", "");
	message.append("\n");
	return message;
}
*/

} /* namespace xml */
} /* namespace analytic */
