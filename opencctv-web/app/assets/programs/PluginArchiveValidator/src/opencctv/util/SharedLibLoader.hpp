
#ifndef OPENCCTV_SHAREDLIBLOADER_HPP_
#define OPENCCTV_SHAREDLIBLOADER_HPP_

#include <string>
#include <dlfcn.h>
#include <stdexcept>

namespace opencctv {
namespace util {

class SharedLibLoader {
private:
	void* _pShLib;
public:
	SharedLibLoader();
	void loadLib(const std::string& sShLibPath);
	void* getFunctionExecutor(const std::string& sFunctionName);
	virtual ~SharedLibLoader();
};

} /* namespace opencctv */
} /* namespace util */

#endif /* OPENCCTV_SHAREDLIBLOADER_HPP_ */
