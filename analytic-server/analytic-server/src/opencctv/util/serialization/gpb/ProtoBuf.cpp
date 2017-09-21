
#include "ProtoBuf.hpp"

namespace opencctv {
namespace util {
namespace serialization {

ProtoBuf::ProtoBuf() : Serializable() {
}

/**
 * Alert! Please remember to delete the opencctv::Image
 * if it was created on the heap using new operator.
 * Please delete returned std::string when you are done with it
 * because it was constructed on heap using new operator.
 * This function is used to serialized large Image objects.
 */
std::string* ProtoBuf::serialize(const opencctv::Image* pImage)
{
	image* pSerializableImage = new image();
	pSerializableImage->set_stream_id(pImage->getStreamId());
	pSerializableImage->set_width(pImage->getWidth());
	pSerializableImage->set_height(pImage->getHeight());
	pSerializableImage->set_timestamp(pImage->getTimestamp());
	pSerializableImage->set_input_name(pImage->getInputName());
	for(size_t j = 0; j < pImage->getImageData().size(); ++j)
	{
		pSerializableImage->add_image_data(pImage->getImageData()[j]);
	}
	std::string* pSSerializedImage = new std::string();
	pSerializableImage->SerializeToString(pSSerializedImage);
	if(pSerializableImage) delete pSerializableImage;
	return pSSerializedImage;
}

/**
 * Alert! Please remember to delete the std::string
 * if it was created on the heap using new operator.
 * Please delete returned opencctv::Image when you are done with it
 * because it was constructed on heap using new operator.
 * This function is used to de-serialized large Image objects.
 */
opencctv::Image* ProtoBuf::deserializeImage(const std::string* pSSerializedImage)
{
	image* pParsedSerializableImage = new image();
	pParsedSerializableImage->ParseFromString(*pSSerializedImage);
	opencctv::Image* pParsedImage = new opencctv::Image();
	pParsedImage->setWidth(pParsedSerializableImage->width());
	pParsedImage->setHeight(pParsedSerializableImage->height());
	pParsedImage->setTimestamp(pParsedSerializableImage->timestamp());
	pParsedImage->setInputName(pParsedSerializableImage->input_name());
	std::vector<unsigned char>* vParsedImageData = new std::vector<unsigned char>(pParsedSerializableImage->image_data().begin(), pParsedSerializableImage->image_data().end());
	pParsedImage->setImageData(*vParsedImageData);
	if(pParsedSerializableImage) delete pParsedSerializableImage;
	if(vParsedImageData) delete vParsedImageData;
	return pParsedImage;
}

std::string ProtoBuf::serialize(const analytic::AnalyticResult& analyticResult)
{
	analytic_result serializableAnalyticResult;
	serializableAnalyticResult.set_stream_id(analyticResult.getStreamId());
	serializableAnalyticResult.set_input_name(analyticResult.getInputName());
	serializableAnalyticResult.set_timestamp(analyticResult.getTimestamp());
	serializableAnalyticResult.set_custom_text(analyticResult.getCustomText());
	serializableAnalyticResult.set_write_to_db(analyticResult.getWriteToDatabase());
	return serializableAnalyticResult.SerializeAsString();
}

analytic::AnalyticResult ProtoBuf::deserializeAnalyticResult(const std::string& sSerializedAnalyticResult)
{
	analytic_result parsedAnalyticResult;
	parsedAnalyticResult.ParseFromString(sSerializedAnalyticResult);
	analytic::AnalyticResult analyticResult(parsedAnalyticResult.stream_id(), parsedAnalyticResult.input_name(),
			parsedAnalyticResult.timestamp(), parsedAnalyticResult.custom_text(), parsedAnalyticResult.write_to_db());
	return analyticResult;

}

ProtoBuf::~ProtoBuf() {
}

} /* namespace serialization */
} /* namespace util */
} /* namespace opencctv */
