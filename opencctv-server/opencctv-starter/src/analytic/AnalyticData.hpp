/*
 * AnalyticData.h
 *
 *  Created on: Sep 16, 2016
 *      Author: nhutumdai
 */

#ifndef ANALYTICDATA_H_
#define ANALYTICDATA_H_

#include <vector>

namespace analytic {
class AnalyticData {

public:

	bool bStatus;
	std::vector<unsigned int> vAnalyticInstanceStreamId; // list of analytic instance stream id per analytic
	std::vector<unsigned int> vStreamId;  // list of  stream id per analytic

	AnalyticData();
	virtual ~AnalyticData();
};

#endif /* ANALYTICDATA_H_ */
} // end namespace analytic
