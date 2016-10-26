
#include "SharedLibLoader.hpp"

namespace opencctv {
namespace util {

SharedLibLoader::SharedLibLoader() {
	_pShLib = NULL;
}

void SharedLibLoader::loadLib(const std::string& sShLibPath)
{
	_pShLib = dlopen(sShLibPath.c_str(), RTLD_LAZY);
	if (!_pShLib) {
		throw std::runtime_error(dlerror());
	}
}

void* SharedLibLoader::getFunctionExecutor(const std::string& sFunctionName) {
	void* ret = NULL;
	if (_pShLib) {
		ret = dlsym(_pShLib, sFunctionName.c_str());
		const char* pErrorStr = dlerror();
		if (!ret || pErrorStr) {
			throw std::runtime_error(pErrorStr);
		}
	}
	return ret;
}

SharedLibLoader::~SharedLibLoader() {
	if (_pShLib) {
		// delete _pShLib;
	}
}

} /* namespace opencctv */
} /* namespace util */
