/*
 * MainController.hpp
 *
 *  Created on: Sep 27, 2016
 *      Author: nhutumdai
 */

#ifndef MAINCONTROLLER_HPP_
#define MAINCONTROLLER_HPP_

//#include "test/gateway/TestStreamGateway.hpp"
//#include "test/gateway/TestAnalyticInstanceStreamGateway.hpp"

#include "../Exception.hpp"

#include "../util/log/Loggers.hpp"
#include "../util/flow/SimpleFlowController.hpp"
#include "../util/Config.hpp"

#include "../ImageMulticaster.hpp"
#include "../ConsumerThread.hpp"
#include "../ProducerThread.hpp"
#include "../ResultRouterThread.hpp"

#include "../db/StreamGateway.hpp"
#include "../db/AnalyticInstanceStreamGateway.hpp"

#include "../../analytic/AnalyticData.hpp"
#include "../../analytic/AnalyticServer.hpp"
//#include "../../analytic/AnalyticServerManager.hpp"

#include "../OpenCCTVServer.hpp"
//#include "../OpenCCTVServerManager.hpp"

/* boost includes (/user/local/include) */
/* boost libraries (/usr/local/lib) -lboost_system */

#include <boost/thread/thread.hpp> // -lboost_thread -pthread
#include <boost/lockfree/queue.hpp>
#include <boost/lexical_cast.hpp> // to cast values
#include <signal.h> // to handle terminate signal

using namespace analytic;
using namespace opencctv;
using namespace util::log;

namespace opencctv {
namespace controller {

class MainController {
private:
	//AnalyticServerManager* pASM = NULL;

	//int _iAnalyticServerId;
	AnalyticServer* _pAS;

	//int _iOpenCCTVServerId;
	OpenCCTVServer* _pOS;

public:
	MainController(AnalyticServer* pAS, OpenCCTVServer* pOS);
	virtual ~MainController();

	int startAllAnalytic();

	AnalyticServer* getAS() {
		return _pAS;
	}

	OpenCCTVServer* getOS() {
		return _pOS;
	}
};
}/* namespace controller */
} /* namespace opencctv */

#endif /* MAINCONTROLLER_HPP_ */
