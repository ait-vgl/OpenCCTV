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

namespace analytic {

class AnalyticServerController
{
private:
	static AnalyticServerController* _pAnalyticServerController;
	unsigned int iPort;
	unsigned int iNumOfAnalytics;
	zmq::socket_t* _pSocket;
	analytic::util::Config* _pConfig;

	AnalyticServerController();
	AnalyticServerController(AnalyticServerController const& source);
	AnalyticServerController& operator=(AnalyticServerController const&);

	std::string startAnalytic(const std::string& sRequest);
	std::string stopAnalytic(const std::string& sRequest);
	std::string killAllAnalytics(const std::string& sRequest);
	std::string getServerStatus();
	std::string reportError(const std::string& sOperation,const bool bDone,
			const std::string& sErrorMsg, const std::string& sExceptionMsg);
	void sendReply(const std::string& sMessage);

public:
	static AnalyticServerController* getInstance();
	void executeOperation();
	virtual ~AnalyticServerController();
};

} /* namespace analytic */

#endif /* ANALYTICSERVERCONTROLLER_HPP_ */
