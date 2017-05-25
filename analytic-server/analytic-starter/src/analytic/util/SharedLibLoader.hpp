/*
 * SharedLibLoader.hpp
 *
 *  Created on: May 5, 2017
 *      Author: anjana
 */

#ifndef SHAREDLIBLOADER_HPP_
#define SHAREDLIBLOADER_HPP_

#include <string>
#include <dlfcn.h>
#include <stdexcept>

namespace analytic {
namespace util {

class SharedLibLoader
{
private:
	void* _pShLib;
public:
	SharedLibLoader();
	void loadLib(const std::string& sShLibPath);
	void closeLib();
	void* getFunctionExecutor(const std::string& sFunctionName);
	virtual ~SharedLibLoader();
};

} /* namespace util */
} /* namespace analytic */

#endif /* SHAREDLIBLOADER_HPP_ */
