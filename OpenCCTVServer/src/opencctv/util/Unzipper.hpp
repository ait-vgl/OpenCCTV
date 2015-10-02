
#ifndef UNZIPPER_HPP_
#define UNZIPPER_HPP_

#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include "../Exception.hpp"
#include "Util.hpp"

namespace opencctv {
namespace util {

class Unzipper {
private:
	std::string _sLocation;
	std::string _sZipFileName;
	std::string _sTempDirName;
	std::vector<std::string> _files;
public:
	Unzipper(std::string sLocation, std::string sZipFilePath, std::string sTempDirPath);
	bool unzip();
	std::string getUnzippedDirPath();
	void removeUnzippedFiles();
	const std::vector<std::string>& getFiles();
	std::string getShortFilePath(const std::string& sFullPath, const std::string& sParentDirName);
	virtual ~Unzipper();
};

} /* namespace util */
} /* namespace opencctv */

#endif /* UNZIPPER_HPP_ */
