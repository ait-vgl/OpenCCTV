/*
 * AnalyticData.h
 *
 *  Created on: Sep 16, 2016
 *      Author: nhutumdai
 */

#ifndef ANALYTICDATA_H_
#define ANALYTICDATA_H_

#include <vector>
#include <map> // for NULL

#include "../opencctv/util/flow/FlowController.hpp"

#include <boost/thread/thread.hpp>
#include "../opencctv/util/log/Loggers.hpp"


namespace analytic {
class AnalyticData {
	//unsigned int _iId;
	//bool _bStatus;
	//std::vector<unsigned int> _vStreamIds;  // list of  stream id per analytic
	boost::thread* _pResultRouterThread;

	//std::string _sAnalyticQueueInAddress;
//	std::string _sAnalyticQueueOutPort;
	//opencctv::util::flow::FlowController* _pFlowController;


public:
	AnalyticData();
	virtual ~AnalyticData();

	//bool isAnalyticQueueInAddress();
	//bool isAnalyticQueueOutPort();
	//bool isMultipleStream();
	//bool isFlowController();
/*
	opencctv::util::flow::FlowController* getFlowController() const {
		return _pFlowController;
	}

	void setFlowController(opencctv::util::flow::FlowController* flowController) {
		_pFlowController = flowController;
	}
*/
	boost::thread* getResultRouterThread() const {
		return _pResultRouterThread;
	}

	void setResultRouterThread(boost::thread* resultRouterThread) {
		_pResultRouterThread = resultRouterThread;
	}
/*
	std::string getAnalyticQueueOutAddress() {
		return _sAnalyticQueueOutPort;
	}

	void setAnalyticQueueOutAddress(std::string& analyticQueueOutPort) {
		_sAnalyticQueueOutPort = analyticQueueOutPort;
	}

	std::string getAnalyticQueueInAddress() {
		return _sAnalyticQueueInAddress;
	}

	void setAnalyticQueueInAddress(std::string& imageInputQueueAddress) {
		_sAnalyticQueueInAddress = imageInputQueueAddress;
	}
*/
/*
	bool isStatus() const {
		return _bStatus;
	}

	void setStatus(bool status) {
		_bStatus = status;
	}
*/

	/*const std::vector<unsigned int>& getStreamIds() const {
		return _vStreamIds;
	}*/
/*
	void setStreamId(unsigned int streamId) {
		_vStreamIds.push_back(streamId);
	}
	*/
};


} // end namespace analytic

#endif /* ANALYTICDATA_H_ */
