
#include "opencctv/mq/MqUtil.hpp"
#include "analytic/xml/AnalyticMessage.hpp"
#include "opencctv/Exception.hpp"
#include "opencctv/util/log/Loggers.hpp"
#include "analytic/AnalyticProcess.hpp"
#include "analytic/util/Config.hpp"
#include <string>
#include <sstream>
#include <map>
#include "analytic/ApplicationModel.hpp"
#include "analytic/AnalyticServerController.hpp"
#include "result/ResultsTxThread.hpp"

int main()
{
	analytic::AnalyticServerController* pAnalyticServerController = NULL;
	bool bEnabled = false;
	try
	{
		pAnalyticServerController = analytic::AnalyticServerController::getInstance();
		if(pAnalyticServerController)
		{
			bEnabled = true;
		}
	}catch(opencctv::Exception &e)
	{
		std::string sErrMsg = "Failed to initialize the analytic server : ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		return -1;
	}

	//======================
	//analytic::ApplicationModel* pModel = analytic::ApplicationModel::getInstance();
	/*boost::thread_group* _pResultsTxThreadGroup = new boost::thread_group();
	pModel->setResultsTxThreadGroup(_pResultsTxThreadGroup);*/
	/*result::ResultsTxThread* pResultsTx = new result::ResultsTxThread();
	boost::thread* pResultsTxThread = new boost::thread(*pResultsTx);
	if (pResultsTxThread->joinable()) {
		pModel->getResultsTxThreadGroup()->add_thread(pResultsTxThread);
	}*/

	//======================

	while(bEnabled)
	{
		pAnalyticServerController->executeOperation();
	}

	std::string sErrMsg = "Analytic Server Exit...........";
	opencctv::util::log::Loggers::getDefaultLogger()->debug(sErrMsg);

	return 0;

}
