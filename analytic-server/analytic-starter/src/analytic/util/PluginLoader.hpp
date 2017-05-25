/*
 * PluginLoader.hpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#ifndef PLUGINLOADER_HPP_
#define PLUGINLOADER_HPP_

#include "SharedLibLoader.hpp"
#include "../../opencctv/Exception.hpp"

namespace analytic {
namespace util {

template<typename T>
class PluginLoader
{
private:
	util::SharedLibLoader sharedLibLoader;
	typedef T* create_t();
	typedef void destroy_t(T*);

public:
	PluginLoader()
	{
	}

	void loadPlugin(const std::string& sPluginPath)
	{
		try
		{
			sharedLibLoader.loadLib(sPluginPath);
		}
		catch(std::exception &e)
		{
			throw opencctv::Exception(std::string(e.what()));
		}
	}

	void closePlugin()
	{
		sharedLibLoader.closeLib();
	}

	T* createPluginInstance()
	{
		create_t* createInstance = NULL;
		try
		{
			createInstance = (create_t*) sharedLibLoader.getFunctionExecutor("create");
			return createInstance();
		}
		catch(std::exception &e)
		{
			throw opencctv::Exception(std::string(e.what()));
		}
		return NULL;
	}

	void deletePluginInstance(T* pPluginInst)
	{
		destroy_t* deleteInstance = NULL;
		try
		{
			deleteInstance = (destroy_t*) sharedLibLoader.getFunctionExecutor("destroy");
			deleteInstance(pPluginInst);
		}
		catch(std::exception &e)
		{
			throw opencctv::Exception(std::string(e.what()));
		}
	}
};

} /* namespace util */
} /* namespace analytic */

#endif /* PLUGINLOADER_HPP_ */
