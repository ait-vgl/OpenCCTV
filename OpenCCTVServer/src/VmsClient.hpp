/*
 * VmsClient.hpp
 *
 *  Created on: Oct 7, 2014
 *      Author: Lakindu Boteju
 */

#ifndef VMSCLIENT_HPP_
#define VMSCLIENT_HPP_

#include "db/Stream.hpp"
#include "ThreadSafeQueue.hpp"
#include "Image.hpp"
#include "db/Camera.hpp"

using namespace db;

class VmsClient
{
protected:
	Stream _stream;
	Camera _camera;
public:
	VmsClient(Stream &stream, Camera &camera)
	{
		_stream = stream;
		_camera = camera;
	}
	virtual ~VmsClient()
	{}
	virtual void produceImageObjects(ThreadSafeQueue<Image> &queue, Stream &stream, const unsigned int iImageCount) = 0;
	virtual bool init() = 0;
	virtual bool isStillProducingImages() = 0;
};

#endif /* VMSCLIENT_HPP_ */
