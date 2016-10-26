#include "Util.hpp"

namespace opencctv {
namespace util {

std::string Util::trim(std::string sToTrim) {
	boost::algorithm::trim(sToTrim);
	return sToTrim;
}

unsigned long long Util::getCurrentTimeMs() {
	unsigned long long ret = 0;
	struct timeval t;
	gettimeofday(&t, NULL);
	ret = (((unsigned long long) t.tv_sec) * 1000)
			+ (((unsigned long long) t.tv_usec) / 1000);
	return ret;
}

int Util::getCurrentVmUsageKb() {
	FILE* file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[128];
	while (fgets(line, 128, file) != NULL) {
		if (strncmp(line, "VmRSS:", 6) == 0) {
			result = parseLine(line);
			break;
		}
	}
	fclose(file);
	return result;
}

int Util::parseLine(char* line) {
	int i = strlen(line);
	while (*line < '0' || *line > '9')
		line++;
	line[i - 3] = '\0';
	i = atoi(line);
	return i;
}

bool Util::findSharedLibOfPlugin(const std::string& sPathToPluginDir, std::string& sPathToSharedLibFile) {
	namespace bf = boost::filesystem;
	std::string sDirToScan = sPathToPluginDir;
	while (true) {
		int iFileCount = 0;
		std::string sLastFilePath;
		std::string sLastSharedLibFilePath;
		int iSharedLibFileCount = 0;
		if (scanDir(sDirToScan, iFileCount, sLastFilePath, iSharedLibFileCount,
				sLastSharedLibFilePath)) {
			bf::path lastFilePath = bf::path(sLastFilePath);
			if (iFileCount == 1 && bf::is_directory(lastFilePath)) {
				sDirToScan = sLastFilePath;
			} else if (iSharedLibFileCount == 1) {
				sPathToSharedLibFile = sLastSharedLibFilePath;
				return true;
			} else
				break;
		} else {
			break;
		}
	}
	return false;
}

bool Util::scanDir(const std::string& sDirPath, int& iFileCount, std::string& sLastFilePath, int& iSharedLibFileCount, std::string& sLastSharedLibFilePath) {
	namespace bf = boost::filesystem;
	bf::path dirPath = bf::path(sDirPath);
	if (bf::exists(dirPath) && bf::is_directory(dirPath)) {
		bf::directory_iterator end_iter;
		for (bf::directory_iterator dir_itr(dirPath); dir_itr != end_iter;
				++dir_itr) {
			++iFileCount;
			sLastFilePath = dir_itr->path().string();
			if (bf::is_regular_file(dir_itr->status())) {
				if (dir_itr->path().extension().string().compare(".so") == 0) {
					++iSharedLibFileCount;
					sLastSharedLibFilePath = dir_itr->path().string();
				}
			}
		}
		return true;
	} else {
		return false;
	}
}

std::string Util::getPidMessage(pid_t pid)
{
	boost::property_tree::ptree pt;
	pt.put("pid", pid);
	std::ostringstream oss;
	try {
		write_xml(oss, pt);
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to generate PID message. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	std::string message = oss.str();
	boost::replace_all(message, "\n", "");
	message.append("\n");
	return message;
}

pid_t Util::getPid(const std::string& sPidMessage)
{
	pid_t pid = 0;
	boost::property_tree::ptree pt;
	std::istringstream iss(sPidMessage);
	try {
		read_xml(iss, pt);
		pid = pt.get<pid_t>("pid");
	} catch (boost::property_tree::xml_parser::xml_parser_error &e) {
		std::string sErrMsg = "Failed to parse PID message. ";
		sErrMsg.append(e.what());
		throw opencctv::Exception(sErrMsg);
	}
	return pid;
}

} /* namespace util */
} /* namespace opencctv */
