
#ifndef OPENCCTV_PLUGINLOADER_HPP_
#define OPENCCTV_PLUGINLOADER_HPP_

#include "util/SharedLibLoader.hpp"
#include "Exception.hpp"

namespace opencctv {

template<typename T>
class PluginLoader {
private:
	util::SharedLibLoader sll;
	typedef T* create_t();
	typedef void destroy_t(T*);
public:
	PluginLoader() {
	}
	void loadPlugin(const std::string& sPluginPath)
	{
		try
		{
			sll.loadLib(sPluginPath);
		}
		catch(std::exception &e)
		{
			throw Exception(std::string(e.what()));
		}
	}
	T* createPluginInstance() {
		create_t* createInstance = NULL;
		try
		{
			createInstance = (create_t*) sll.getFunctionExecutor("create");
			return createInstance();
		}
		catch(std::exception &e)
		{
			throw Exception(std::string(e.what()));
		}
		return NULL;
	}
	void deletePluginInstance(T* pPluginInst)
	{
		destroy_t* deleteInstance = NULL;
		try
		{
			deleteInstance = (destroy_t*) sll.getFunctionExecutor("destroy");
			deleteInstance(pPluginInst);
		}
		catch(std::exception &e)
		{
			throw Exception(std::string(e.what()));
		}
	}
};

} /* namespace opencctv */

#endif /* OPENCCTV_PLUGINLOADER_HPP_ */
