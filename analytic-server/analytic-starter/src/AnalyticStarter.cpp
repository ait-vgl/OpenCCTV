
#include "opencctv/Exception.hpp"
#include "opencctv/util/log/Loggers.hpp"
#include <string>
#include <vector>
#include "analytic/AnalyticServerController.hpp"
#include "result/db/AnalyticResultGateway.hpp"
#include "result/AnalyticInstController.hpp"

void exitHandler(int iSignum);

int main()
{
	/*
	 * Registering signals with exit handler
	 */
	signal(SIGINT, exitHandler); // for Ctrl + C keyboard interrupt
	signal(SIGTERM, exitHandler); // for Terminate signal
	signal(SIGKILL, exitHandler); // for SIGKILL

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
			/*
			 * Set the server status to running
			 */
			pAnalyticServerController->updateStatus(analytic::ANALYTIC_SERVER_RUNNING);

			opencctv::util::log::Loggers::getDefaultLogger()->info("Analytic server started........");
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
	/*result::db::AnalyticResultGateway analyticResultGateway;
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
	}*/




	/*
	 * Start responding to requests. At present supported requests are start analytic
	 * instance, stop analytic instance kill all analytic instances and get analytic server status
	 */
	while(bEnabled)
	{
		pAnalyticServerController->executeOperation();
	}

	exitHandler(0);

	return 0;
}

void exitHandler(int iSignum)
{
	//Call stopServer which will stop the analytics, server and update status in the database
	analytic::AnalyticServerController* pAnalyticServerController = NULL;
	try
	{
		pAnalyticServerController = analytic::AnalyticServerController::getInstance();

	}catch(opencctv::Exception &e)
	{
		std::string sErrMsg = "Error occured while exiting analytic server. Failed to get access to  Analytic Server Controller : ";
		sErrMsg.append(e.what());
		opencctv::util::log::Loggers::getDefaultLogger()->error(sErrMsg);
	}

	if(pAnalyticServerController)
	{

		pAnalyticServerController->stopServer();

	}

	std::string sMsg = "Analytic Server Exit...........";
	opencctv::util::log::Loggers::getDefaultLogger()->debug(sMsg);
    exit(iSignum);
}
