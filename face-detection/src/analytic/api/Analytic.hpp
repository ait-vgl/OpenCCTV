
#ifndef ANALYTIC_ANALYTIC_HPP_
#define ANALYTIC_ANALYTIC_HPP_

#include "../ConcurrentQueue.hpp"
#include <string>
#include <opencv2/core/core.hpp>
#include <map>

namespace analytic {
namespace api {

typedef struct
{
	cv::Mat matImage;
	unsigned int iStreamId;
	std::string sInputName;
	std::string sTimestamp;
	std::string sCustomTextResult;
	bool bGenerateAnalyticEvent;
} Image_t;

class Analytic
{
public:
	Analytic(){}
	virtual bool init(const std::string& sAnalyticPluginDirLocation) = 0;
	virtual void process(analytic::ConcurrentQueue<analytic::api::Image_t>* pInputQueue, analytic::ConcurrentQueue<analytic::api::Image_t>* pOutputQueue) = 0;
	virtual std::string getInputStreamNames()
	{
		std::map<std::string, std::string> mInputStreams;
		mInputStreams["default"] = "This analytic needs only 1 Input Stream";
		return generateInputStreamNamesXml(mInputStreams);
	}
	virtual ~Analytic(){}

protected:
	std::string generateInputStreamNamesXml(std::map<std::string, std::string>& mInputStreamDescriptions)
	{
		std::string sXml = "<inputstreams>";
		std::map<std::string, std::string>::iterator it;
		for(it = mInputStreamDescriptions.begin(); it != mInputStreamDescriptions.end(); ++it)
		{
			sXml.append("<input>");
			sXml.append("<name>").append(it->first).append("</name>");
			sXml.append("<description>").append(it->second).append("</description>");
			sXml.append("</input>");
		}
		sXml.append("</inputstreams>");
		return sXml;
	}

	void generateAnalyticResultXml(std::map<std::string, std::string>& mTagValuePair, std::string& sToStoreXml) {
		std::stringstream ssXml;
		ssXml << "<result>";
		std::map<std::string, std::string>::iterator it;
		for (it = mTagValuePair.begin(); it != mTagValuePair.end(); ++it) {
			ssXml << "<" << it->first << ">";
			ssXml << it->second;
			ssXml << "</" << it->first << ">";
		}
		ssXml << "</result>";
		sToStoreXml = ssXml.str();
	}
};

} /* namespace api */
} /* namespace analytic */

#endif /* ANALYTIC_ANALYTIC_HPP_ */

/*
 * In YOUR_ANALYTIC_CLASS.hpp include these two functions.
 *
extern "C" YOUR_ANALYTIC_CLASS* create() {
    return new YOUR_ANALYTIC_CLASS();
}

extern "C" void destroy(YOUR_ANALYTIC_CLASS* p) {
    if(p) {
    	delete p;
    }
}
*/
