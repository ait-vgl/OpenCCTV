
#include "ResultRouterThread.hpp"

namespace opencctv {

ResultRouterThread::ResultRouterThread(unsigned int iAnalyticInstanceId) {



	int iAnalyticServerId = 1;
	_pAS = analytic::AnalyticServerManager::getInstance()->getAnalyticServer(iAnalyticServerId);

	_pAD = _pAS->getAnalyticData(iAnalyticInstanceId);

	_pFlowController = NULL;
	if(_pAD and _pAD->isFlowController())
	{
		_pFlowController = _pAD->getFlowController();
	}


	_pSerializer = util::serialization::Serializers::getInstanceOfDefaultSerializer();
	_iAnalyticInstanceId = iAnalyticInstanceId;
}

void ResultRouterThread::operator()()
{
	util::Config* pConfig = util::Config::getInstance();

	if(_pAD and _pAD->isAnalyticQueueOutAddress())
	{
		//Initialize the ZMQ connection to the analytic instance's output queue
		bool bConnected = false;
		mq::TcpMqReceiver receiver;
		try
		{
			receiver.connectToMq(pConfig->get(util::PROPERTY_ANALYTIC_SERVER_IP), _pAD->getAnalyticQueueOutAddress());
			bConnected = true;
		}
		catch(Exception &e)
		{
			std::string sErrMsg = "Failed to connect to Analytic Output Queue. ";
			sErrMsg.append(e.what());
			util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}

		//Create the AnalyticResultGateway to the DB
		opencctv::db::AnalyticResultGateway* _pAnalyticResultGateway = NULL;
		try
		{
			_pAnalyticResultGateway = new opencctv::db::AnalyticResultGateway();

		}catch(opencctv::Exception &e)
		{
			util::log::Loggers::getDefaultLogger()->error(e.what());
		}

		//Start inserting the analytic instance's results to the results DB
		while(bConnected && _pFlowController && _pAnalyticResultGateway)
		{

			//try{

				//boost::this_thread::interruption_point(); // Interruption

				std::string* pSSerializedResult = receiver.receive();
				analytic::AnalyticResult result = _pSerializer->deserializeAnalyticResult(*pSSerializedResult);
				std::string sMsg;
				/*
				ssMsg = "\t\tReceived Result of ";
				sMsg.append(result.getTimestamp());
				util::log::Loggers::getDefaultLogger()->debug(sMsg);
				*/

				//Saving to DB
				if(result.getWriteToDatabase())
				{
					try
					{
						opencctv::db::AnalyticResultGateway analyticResultGateway;
						analyticResultGateway.insertResults(_iAnalyticInstanceId, result);
						sMsg = "\t\tResult written to the database: ";
						sMsg.append(result.getTimestamp());
						util::log::Loggers::getDefaultLogger()->info(sMsg);

					}catch(opencctv::Exception &e)
					{
						std::ostringstream sErrorMessage;
						sErrorMessage << "Failed to write results to the results database : Analytic ID - ";
						sErrorMessage << _iAnalyticInstanceId;
						sMsg = sErrorMessage.str();
						util::log::Loggers::getDefaultLogger()->error(sMsg);
					}
				}

				_pFlowController->received();
				if(pSSerializedResult) delete pSSerializedResult;

			//}catch (boost::thread_interrupted&) {
				//opencctv::util::log::Loggers::getDefaultLogger()->error("Result router Thread interrupted.");
			//}
		}
	}

}

ResultRouterThread::~ResultRouterThread(){
	// inside pModel
		// _pFlowController
		// _pSerializer
	std::cout << "3. Result Router Therad destructor called." << std::endl;}


} /* namespace opencctv */
