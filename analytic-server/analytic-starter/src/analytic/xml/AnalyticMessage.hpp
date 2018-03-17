
#ifndef ANALYTIC_ANALYTICMESSAGE_HPP_
#define ANALYTIC_ANALYTICMESSAGE_HPP_

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <sys/types.h>
#include "../../opencctv/Exception.hpp"

namespace analytic {
namespace xml {

const std::string OPERATION_START_ANALYTIC = "StartAnalytic";
const std::string OPERATION_KILL_ALL_ANALYTICS = "KillAllAnalytics";
const std::string OPERATION_STOP_ANALYTIC = "StopAnalytic";
const std::string OPERATION_ANALYTIC_SERVER_STATUS = "AnalyticServerStatus";
const std::string OPERATION_ANALYTIC_INST_STATUS = "AnalyticInstStatus";
const std::string OPERATION_UNKNOWN = "Unknown";

class AnalyticMessage {
public:
	static void extractInitialDetails(const std::string& sAnalyticRequest,
			unsigned int& iServerId, std::string& sOperation);
	static std::string getServerStatusReply(const std::string& sMessage,
			const std::string& sStatus, const int iPid);
	static std::string getAnalyticStartReply(const std::string& sMessage,
			const std::string& sServerStatus,const int iPid);

	// TODO May 08 2017 Later Remove =======
	//static std::string getAnalyticStartReply(const std::string& sAnalyticQueueInAddress,
	//const std::string& sAnalyticQueueOutAddress);

	static std::string getKillAllAnalyticsReply(const std::string& sMessage,
			const std::string& sServerStatus,const int iPid);
	static void extractAnalyticStartRequestData(const std::string& sAnalyticStartRequest,
			unsigned int& iAnalyticInstanceId, std::string& sAnalyticFilename);
	static void extractAnalyticStartReplyData(const std::string& sAnalyticStartReply,
			std::string& sAnalyticQueueInAddress, std::string& sAnalyticQueueOutAddress);
	static void extractAnalyticStopRequestData(const std::string& sAnalyticStopRequest,
			unsigned int& iAnalyticInstanceId);
    static std::string getStopAnalyticProcessesReply(bool bDone);
    static std::string getAnalyticStopReply(const std::string& sMessage,
    		const std::string& sServerStatus,const int iPid);
    static std::string getErrorReply(const std::string& sErrorMessage,
    		const std::string& sServerStatus,const int iPid);
    static void extractKillAllAnalyticsReply(const std::string& sKillAllAnalyticsReply,
    		std::string& sContent);
};

} /* namespace xml */
} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICMESSAGE_HPP_ */
