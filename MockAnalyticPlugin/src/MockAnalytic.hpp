
#ifndef ANALYTIC_MOCKANALYTIC_HPP_
#define ANALYTIC_MOCKANALYTIC_HPP_

#include "analytic/api/Analytic.hpp"
/* opencv includes in /usr/local/include */
/* opencv libraries in /usr/local/lib */
/* All available opencv libs are linked to this project
 * (in eclipse to go Project Properties -> C/C++ Build -> Settings -> GCC C++ Linker -> Libraries) */
#include <opencv/cv.h>
#include <opencv/highgui.h>
/* these are few extra boost includes, no need to use in your project.
 * all you need is opencv 2.4.9 */
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

using namespace analytic;

class MockAnalytic : public api::Analytic {
public:
	MockAnalytic();
	bool init(const std::string& sAnalyticPluginDirLocation);
	void process(analytic::ImageQueue<analytic::api::Image_t>* pInputQueue, analytic::ImageQueue<analytic::api::Image_t>* pOutputQueue);
	void resultXml(const std::string& sText, std::string& sToStoreXml);
	virtual ~MockAnalytic();
};

extern "C" MockAnalytic* create() {
    return new MockAnalytic();
}

extern "C" void destroy(MockAnalytic* p) {
    if(p) {
    	delete p; p = NULL;
    }
}

#endif /* ANALYTIC_MOCKANALYTIC_HPP_ */
