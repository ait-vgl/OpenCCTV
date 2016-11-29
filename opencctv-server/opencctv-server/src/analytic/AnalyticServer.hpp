
#ifndef ANALYTIC_SERVER_HPP_
#define ANALYTIC_SERVER_HPP_

#include <map>

#include "xml/AnalyticMessage.hpp"

#include "AnalyticData.hpp"

#include "../opencctv/OpenCCTVServer.hpp"


#include "../opencctv/util/Config.hpp"
#include "../opencctv/util/log/Loggers.hpp"
#include "../opencctv/util/TypeDefinitions.hpp"

#include "../opencctv/mq/MqUtil.hpp"

//#include "opencctv/util/flow/SimpleFlowController.hpp"
#include "../opencctv/Exception.hpp"

#include "../opencctv/db/StreamGateway.hpp"
//#include "../opencctv/db/AnalyticServerGateway.hpp"
#include "../opencctv/db/AnalyticInstanceGateway.hpp"


#include "../opencctv/ResultRouterThread.hpp"

namespace analytic {

class AnalyticServer {
private:
	int _iServerId;
    std::string _sAnalyticServerIp;
	zmq::socket_t* _pSocket;
	std::map<unsigned int, AnalyticData*> _mAnalyticDatas; // Key is analytic instance id, Value is analytic data
    bool startAnalyticInstance(unsigned int iAnalyticInstanceId, const std::string& sAnalyticPluginDirLocation, const std::string& sAnalyticPluginFilename, std::string& sAnalyticQueueInAddress, std::string& sAnalyticQueueOutAddress);
    bool startAnalyticInstanceWrapper(std::vector<opencctv::dto::Stream>& vStreams);
    bool removeAnalyticData(unsigned int iAnalyticInstanceId);

public:
	AnalyticServer(const int& analyticServerId, const std::string& sAnalyticServerIp, const std::string& sAnalyticServerPort);

    bool startAllAnalyticInstanceAction();
    bool startAnalyticInstanceAction(unsigned int iAnalyticInstanceId);
    bool stopAnalyticInstanceAction(unsigned int iAnalyticInstanceId);
	bool stopAllAnalyticInstanceAction();
    
    
	//const std::map<unsigned int, AnalyticData*>& getAllAnalyticData();
	void setAnalyticData(unsigned int iAnalyticInstanceId, AnalyticData*& pAnalyticData);
	AnalyticData* getAnalyticData(unsigned int iAnalyticInstanceId);
   
	bool isAnalyticInstance(int iAnalyticInstanceId);

	//bool isMultipleStreamPerAnalytic(const unsigned int& iAnalyticInstanceId);

	//bool isImageInputQueueAddress(const int& );

	virtual ~AnalyticServer();

	int getServerId(){
		return _iServerId;
	}
};

} /* namespace analytic */

#endif /* ANALYTIC_ANALYTICINSTANCEMANAGER_HPP_ */
