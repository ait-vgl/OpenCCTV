
#include "opencctv/Exception.hpp"
#include "opencctv/util/log/Loggers.hpp"
#include <string>
#include <vector>
#include "analytic/AnalyticServerController.hpp"
#include "result/db/AnalyticResultGateway.hpp"
#include "result/AnalyticInstController.hpp"

int main()
{
	/*
	 * Initialize the analytic server
	 */
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

	/*
	 * If unsent results are available in the results database start sending them
	 */
	//Find the results applications for which the results available
	result::db::AnalyticResultGateway analyticResultGateway;
	std::vector<int> vAnlyticInstIds;
	analyticResultGateway.findAnalyticsWithUnsentResults(vAnlyticInstIds);
	result::AnalyticInstController analyticInstController;
	std::string sOutputMsg;
	//For each results_app_instance start sending results
	for(size_t i = 0; i < vAnlyticInstIds.size(); ++i)
	{
		analyticInstController.startResultsRouting(vAnlyticInstIds[i],sOutputMsg);
	}

	if(sOutputMsg.size() != 0 || !(sOutputMsg.empty()))
	{
		opencctv::util::log::Loggers::getDefaultLogger()->error(sOutputMsg);
	}

	/*
	 * Start responding to requests. At present supported requests are start analytic
	 * instance, stop analytic instance and kill all analytic instances
	 */
	while(bEnabled)
	{
		pAnalyticServerController->executeOperation();
	}

	std::string sErrMsg = "Analytic Server Exit...........";
	opencctv::util::log::Loggers::getDefaultLogger()->debug(sErrMsg);

	return 0;

}
