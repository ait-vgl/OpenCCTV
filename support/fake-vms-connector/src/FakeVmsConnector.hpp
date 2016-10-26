#ifndef OPENCCTV_FAKEVMSCONNECTOR_HPP_
#define OPENCCTV_FAKEVMSCONNECTOR_HPP_

#include "opencctv/api/VmsConnector.hpp"
#include "util/Config.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>

class FakeVmsConnector: public opencctv::api::VmsConnector {
private:
	unsigned long long _lCount;
	std::vector<unsigned char>* _pVImageData;
	util::Config* _pConfig;
public:
	FakeVmsConnector();
	bool init(const opencctv::api::VmsConnectInfo& info, const std::string sPathToAnalyticDir);
	void produceImageObjects(opencctv::ConcurrentQueue<opencctv::Image>* pQueue);
	bool isStillProducingImages();
	virtual ~FakeVmsConnector();
};

// the class factories
extern "C" FakeVmsConnector* create()
{
	return new FakeVmsConnector;
}

extern "C" void destroy(FakeVmsConnector* p)
{
	if (p)
	{
		delete p;
	}
}

#endif /* OPENCCTV_FAKEVMSCONNECTOR_HPP_ */
