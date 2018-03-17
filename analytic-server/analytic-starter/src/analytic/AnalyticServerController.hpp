/*
 * AnalyticServerController.hpp
 *
 *  Created on: May 8, 2017
 *      Author: anjana
 */

#ifndef ANALYTICSERVERCONTROLLER_HPP_
#define ANALYTICSERVERCONTROLLER_HPP_

#include <string>
#include <sstream>
#include <map>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "../opencctv/mq/MqUtil.hpp"
#include "../opencctv/Exception.hpp"
#include "../opencctv/util/log/Loggers.hpp"
#include "../analytic/xml/AnalyticMessage.hpp"
#include "../analytic/AnalyticProcess.hpp"
#include "../analytic/util/Config.hpp"
#include "../analytic/ApplicationModel.hpp"
#include "../result/ResultAppInstController.hpp"
#include "../result/ResultsTxThread.hpp"
#include "../result/AnalyticInstController.hpp"
#include "../result/db/ResultsAppInstanceGateway.hpp"
#include "../result/db/AnalyticInstanceGateway.hpp"
#include "../result/db/AnalyticServerGateway.hpp"

namespace analytic {

const std::string ANALYTIC_SERVER_STOPPED = "Stopped";
const std::string ANALYTIC_SERVER_RUNNING = "Running";
const std::string ANALYTIC_SERVER_ERROR = "Error";
const std::string ANALYTIC_SERVER_STATUS_UNKNOWN = "Unknown";

class AnalyticServerController
{
private:
	static AnalyticServerController* _pAnalyticServerController;
	std::string _sHost;
	unsigned int _iPort;
	//unsigned int _iNumOfAnalytics;
	zmq::socket_t* _pSocket;
	analytic::util::Config* _pConfig;
	std::string _sStatus;
	unsigned int _iPid;
	unsigned int _iServerId;

	AnalyticServerController();
	AnalyticServerController(AnalyticServerController const& source);
	AnalyticServerController& operator=(AnalyticServerController const&);

	std::string startAnalytic(const std::string& sRequest);
	std::string stopAnalytic(const std::string& sRequest);
	//std::string killAllAnalytics(const std::string& sRequest);
	std::string killAllAnalytics();
	std::string getServerStatus();
	std::string getAnalyticInstStatus();
	//std::string updateAnalyticInstStatus();
	std::string reportError(const std::string& sErrorMsg);
	void sendReply(const std::string& sMessage);

public:
	static AnalyticServerController* getInstance();
	void executeOperation();
	virtual ~AnalyticServerController();
	const std::string& getStatus() const;
	void updateStatus(const std::string& sStatus);
	void stopServer();
};

} /* namespace analytic */

#endif /* ANALYTICSERVERCONTROLLER_HPP_ */
