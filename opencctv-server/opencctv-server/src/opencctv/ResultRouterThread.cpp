
#include "ResultRouterThread.hpp"

namespace opencctv
{

ResultRouterThread::ResultRouterThread(unsigned int iAnalyticInstanceId, std::string sAnalyticServerIp, std::string sAnalyticQueueOutPort)
{

	_iAnalyticInstanceId = iAnalyticInstanceId;

	_sAnalyticServerIp = sAnalyticServerIp;
	_sAnalyticQueueOutPort = sAnalyticQueueOutPort;
	_pSerializer = util::serialization::Serializers::getInstanceOfDefaultSerializer();
}

void ResultRouterThread::operator()()
{
	if (_sAnalyticQueueOutPort != "" && _sAnalyticServerIp != "")
	{
		//Initialize the ZMQ connection to the analytic instance's output queue
		bool bConnected = false;
		mq::TcpMqReceiver receiver;

		try
		{
			receiver.connectToMq(_sAnalyticServerIp, _sAnalyticQueueOutPort);
			bConnected = true;
		}
		catch (Exception &e)
		{
			std::string sErrMsg = "Failed to connect to Analytic Output Queue. ";
			sErrMsg.append(e.what());
			util::log::Loggers::getDefaultLogger()->error(sErrMsg);
		}

		//Create the AnalyticResultGateway to the DB
		opencctv::db::AnalyticResultGateway *_pAnalyticResultGateway = NULL;

		try
		{
			_pAnalyticResultGateway = new opencctv::db::AnalyticResultGateway();
		}
		catch (opencctv::Exception &e)
		{
			util::log::Loggers::getDefaultLogger()->error(e.what());
		}

		//Start inserting the analytic instance's results to the results DB
		while (bConnected  && _pAnalyticResultGateway)
		{

			std::string *pSSerializedResult = receiver.receive();
			analytic::AnalyticResult result = _pSerializer->deserializeAnalyticResult(*pSSerializedResult);
			std::string sMsg;
			/*
			ssMsg = "\t\tReceived Result of ";
			sMsg.append(result.getTimestamp());
			util::log::Loggers::getDefaultLogger()->debug(sMsg);
			*/

			//Saving to DB
			if (result.getWriteToDatabase())
			{
				try
				{
					opencctv::db::AnalyticResultGateway analyticResultGateway;
					analyticResultGateway.insertResults(_iAnalyticInstanceId, result);
					sMsg = "\t\tResult written to the database: ";
					sMsg.append(result.getTimestamp());
					util::log::Loggers::getDefaultLogger()->info(sMsg);

				}
				catch (opencctv::Exception &e)
				{
					std::ostringstream sErrorMessage;
					sErrorMessage << "Failed to write results to the results database : Analytic ID - ";
					sErrorMessage << _iAnalyticInstanceId;
					sMsg = sErrorMessage.str();
					util::log::Loggers::getDefaultLogger()->error(sMsg);
				}
			}

			//_pFlowController->received();
			if (pSSerializedResult) delete pSSerializedResult;

		}
	}

}

ResultRouterThread::~ResultRouterThread()
{
	// _pFlowController
	// _pSerializer
    if(_pSerializer) delete _pSerializer;
	opencctv::util::log::Loggers::getDefaultLogger()->debug("Result route thread resources were released.");
}

} /* namespace opencctv */
