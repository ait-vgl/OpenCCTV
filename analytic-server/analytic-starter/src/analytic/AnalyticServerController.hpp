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
#include "../opencctv/mq/MqUtil.hpp"
#include "../opencctv/Exception.hpp"
#include "../opencctv/util/log/Loggers.hpp"
#include "../analytic/xml/AnalyticMessage.hpp"
#include "../analytic/AnalyticProcess.hpp"
#include "../analytic/util/Config.hpp"
#include "../analytic/ApplicationModel.hpp"
#include "../result/ResultAppInstController.hpp"
#include "../result/db/ResultsAppInstanceGateway.hpp"
#include "../result/ResultsTxThread.hpp"
#include "../result/AnalyticInstController.hpp"

namespace analytic {

class AnalyticServerController
{
private:
	static AnalyticServerController* _pAnalyticServerController;
	unsigned int iPort;
	unsigned int iNumOfAnalytics;
	zmq::socket_t* _pSocket;
	analytic::util::Config* _pConfig;
	std::string _sStatus;
	int _iPid;

	//std::string _sHost;

	AnalyticServerController();
	AnalyticServerController(AnalyticServerController const& source);
	AnalyticServerController& operator=(AnalyticServerController const&);

	std::string startAnalytic(const std::string& sRequest);
	std::string stopAnalytic(const std::string& sRequest);
	std::string killAllAnalytics(const std::string& sRequest);
	std::string getServerStatus();
	std::string getAnalyticInstStatus();
	std::string updateAnalyticInstStatus();
	/*std::string reportError(const std::string& sOperation,const bool bDone,
			const std::string& sErrorMsg, const std::string& sExceptionMsg);*/
	std::string reportError(const std::string& sErrorMsg);
	void sendReply(const std::string& sMessage);

public:
	static AnalyticServerController* getInstance();
	void executeOperation();
	virtual ~AnalyticServerController();
	const std::string& getStatus() const;
	void setStatus(const std::string& status);
	/*const std::string& getHost() const;
	void setHost(const std::string& host);*/
};

} /* namespace analytic */

#endif /* ANALYTICSERVERCONTROLLER_HPP_ */
