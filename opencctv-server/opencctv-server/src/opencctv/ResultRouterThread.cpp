
#include "ResultRouterThread.hpp"

namespace opencctv
{

ResultRouterThread::ResultRouterThread(unsigned int iAnalyticInstanceId, std::string sAnalyticServerIp, std::string sAnalyticQueueOutPort)
{

	_iAnalyticInstanceId = iAnalyticInstanceId;
    //_pFlowController = pFlowController;
	_sAnalyticServerIp = sAnalyticServerIp;
	_sAnalyticQueueOutPort = sAnalyticQueueOutPort;
	
}

void ResultRouterThread::operator()()
{
   opencctv::util::serialization::Serializable *_pSerializer = new opencctv::util::serialization::ProtoBuf;
   
	if (_sAnalyticQueueOutPort != "" && _sAnalyticServerIp != "")
	{
		//Initialize the ZMQ connection to the analytic instance's output queue
		bool bConnected = false;
		mq::TcpMqReceiver receiver;

		try
		{
			bConnected = receiver.connectToMq(_sAnalyticServerIp, _sAnalyticQueueOutPort);
			//bConnected = true;
            
            if(bConnected)
            {
                util::log::Loggers::getDefaultLogger()->info("ResultRouterThread connected with analytic server: " + _sAnalyticServerIp + ":" + _sAnalyticQueueOutPort);
            }
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
            util::log::Loggers::getDefaultLogger()->info("Creating AnalyticResultGateway is done");
		}
		catch (opencctv::Exception &e)
		{
			util::log::Loggers::getDefaultLogger()->error(e.what());
		}

		//Start inserting the analytic instance's results to the results DB
		while (bConnected  && _pAnalyticResultGateway)
		{

            util::log::Loggers::getDefaultLogger()->debug("Waiting for receiving message from analytic server: " + _sAnalyticServerIp + " Analytic instance id: "+ boost::lexical_cast<std::string>(_iAnalyticInstanceId));
            
			const std::string *pSSerializedResult = receiver.receive();
            
           // util::log::Loggers::getDefaultLogger()->debug("Received " + *pSSerializedResult);

            // util::log::Loggers::getDefaultLogger()->debug("tmp              tmp");
            
            analytic::AnalyticResult result = _pSerializer->deserializeAnalyticResult(*pSSerializedResult);
            //_pSerializer->testF(tmp); 
            
            //((util::serialization::ProtoBuf*)(_pSerializer))->deserializeAnalyticResult(tmp);
            
          //  util::log::Loggers::getDefaultLogger()->debug("Deseiralized .....");
			
             std::string sMsg;
			
			sMsg = "\t\tReceived Result of ";
			sMsg.append(result.getTimestamp());
			util::log::Loggers::getDefaultLogger()->debug(sMsg);
			

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
		
        util::log::Loggers::getDefaultLogger()->debug("End wile loop of ResultRouterThread for analytic instance: " + boost::lexical_cast<std::string>(_iAnalyticInstanceId));
	}

}

ResultRouterThread::~ResultRouterThread()
{
	// _pFlowController
	// _pSerializer
    //if(_pSerializer) delete _pSerializer;
	opencctv::util::log::Loggers::getDefaultLogger()->debug("Result route thread resources were released. Analytic instance id: "+ boost::lexical_cast<std::string>(_iAnalyticInstanceId));
}

} /* namespace opencctv */
