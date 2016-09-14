#include "Unzipper.hpp"

namespace opencctv {
namespace util {

Unzipper::Unzipper(std::string sLocation, std::string sZipFileName, std::string sTempDirName) {
	_sLocation = sLocation;
	_sTempDirName = sTempDirName;
	_sZipFileName = sZipFileName;
}

bool Unzipper::unzip() {
	std::stringstream ssCommand;
	ssCommand << "unzip ";
	ssCommand << _sLocation << "/" << _sZipFileName << " ";
	ssCommand << "-d " << _sLocation << "/" << _sTempDirName;
	FILE* pReadStream = popen(ssCommand.str().data(), "r");
	if (!pReadStream) {
		std::string sErrMsg = "Failed to extract zip file.";
		throw opencctv::Exception(sErrMsg);
	} else {
		const int iBufSize = 1024;
		char buf[iBufSize];
		while (fgets(buf, iBufSize, pReadStream)) {
			std::string sStreamOutput = std::string(buf);
			std::string sFilePath = getShortFilePath(sStreamOutput, _sTempDirName);
			if(!sFilePath.empty())
			{
				_files.push_back(sFilePath);
			}
		}
	}
	pclose(pReadStream);
	if(_files.size() > 0)
		return true;
	return false;
}

void Unzipper::removeUnzippedFiles()
{
	std::stringstream ssCommand;
	ssCommand << "rm -rf ";
	ssCommand << _sLocation << "/" << _sTempDirName;
	popen(ssCommand.str().data(), "r");
}

std::string Unzipper::getUnzippedDirPath()
{
	std::stringstream ssPath;
	ssPath << _sLocation << "/" << _sTempDirName;
	return ssPath.str();
}

const std::vector<std::string>& Unzipper::getFiles()
{
	return _files;
}

std::string Unzipper::getShortFilePath(const std::string& sFullPath, const std::string& sParentDirName)
{
	std::size_t pos = sFullPath.find(sParentDirName);
	if (pos != std::string::npos) {
		std::string sFilePath = sFullPath.substr(pos + sParentDirName.size() + 1);
		return opencctv::util::Util::trim(sFilePath);
	}
	return std::string(); //empty string
}

Unzipper::~Unzipper() {
}

} /* namespace util */
} /* namespace opencctv */
