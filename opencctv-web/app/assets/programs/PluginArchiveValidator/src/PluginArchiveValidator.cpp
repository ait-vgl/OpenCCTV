
#include <sstream>
#include "opencctv/util/Unzipper.hpp"
#include "opencctv/util/Util.hpp"
#include "opencctv/PluginLoader.hpp"
#include "opencctv/api/VmsConnector.hpp"
#include "analytic/api/Analytic.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

const std::string S_PLUGIN_TYPE_VMS = "vms";
const std::string S_PLUGIN_TYPE_ANALYTIC = "analytic";

void appendHtmlTitle(const std::string& sTitle, std::stringstream& ss)
{
	ss << "<strong>" << sTitle << "</strong>";
}

std::string generatePluginValidatorOutput(bool bVerified, const std::string& sHtmlOutput, const std::string& sAnalyticInputs, const std::string& sAnalyticConfigs)
{
	std::stringstream ss;
	ss << "<pluginvalidatorreply>";
		ss << "<verified>" << bVerified << "</verified>";
		ss << "<html>" << sHtmlOutput << "</html>";
		ss << "<analytic>" << sAnalyticConfigs << sAnalyticInputs << "</analytic>";
	ss << "</pluginvalidatorreply>";
	return ss.str();
}

std::string generateConfigFileNameXml(std::map<std::string, std::string>& mConfigFileNames)
{
	std::string sXml = "<configfiles>";
	std::map<std::string, std::string>::iterator it;
	for(it = mConfigFileNames.begin(); it != mConfigFileNames.end(); ++it)
	{
		sXml.append("<configfile>");
		sXml.append("<name>").append(it->first).append("</name>");
		sXml.append("<filename>").append(it->second).append("</filename>");
		sXml.append("</configfile>");
	}
	sXml.append("</configfiles>");
	return sXml;
}

int main(int argc, char* argv[])
{
	if(argc < 5)
	{
		return -1;
	}
	std::string sLocation = argv[1];
	std::string sZipFilename = argv[2];
	std::string sUnzipDirName = argv[3];
	std::string spluginType = argv[4];

	std::stringstream ssHtmlOutput;
	bool bVerified = false;
	std::string sAnalyticInputs;

	opencctv::util::Unzipper unzipper(sLocation, sZipFilename, sUnzipDirName);
	bool bUnzipped = false;
	try
	{
		unzipper.unzip();
		bUnzipped = true;
		// Copy to opencctv dir (/usr/local/opencctv)
		std::stringstream ssCommand;
		ssCommand << "cp -rf ";
		ssCommand << sLocation << "/" << sUnzipDirName << " ";
		if(spluginType.compare(S_PLUGIN_TYPE_ANALYTIC) == 0)
		{
			ssCommand << "/usr/local/opencctv/analytics/" << sUnzipDirName;
			int systemRet = system(ssCommand.str().c_str());
		}
		else if(spluginType.compare(S_PLUGIN_TYPE_VMS) == 0)
		{
			ssCommand << "/usr/local/opencctv/vms_connectors/" << sUnzipDirName;
			int systemRet = system(ssCommand.str().c_str());
		}
	}
	catch(opencctv::Exception& e)
	{
		appendHtmlTitle("Error:", ssHtmlOutput);
		ssHtmlOutput << "<p>" << e.what() << "</p>";
	}

	std::map<std::string, std::string> mConfigFileNames;
	std::string sAnalyticConfigs;
	if(bUnzipped)
	{
		appendHtmlTitle("All the files", ssHtmlOutput);
		std::vector<std::string> files = unzipper.getFiles();
		ssHtmlOutput << "<p>";
		for(size_t i = 0; i < files.size(); ++i)
		{
			ssHtmlOutput << files[i] << "<br/>";

			if(files[i] != "zip" && files[i].substr(files[i].length() - 3) != ".so")
				mConfigFileNames[files[i]] = files[i] ;
                
		}
		ssHtmlOutput << "</p>";
		sAnalyticConfigs = generateConfigFileNameXml(mConfigFileNames);

	}
	else
	{
		appendHtmlTitle("No files found.", ssHtmlOutput);
	}

	std::string sSharedLibPath;
	if(bUnzipped && opencctv::util::Util::findSharedLibOfPlugin(unzipper.getUnzippedDirPath(), sSharedLibPath))
	{
		appendHtmlTitle("Shared Library found", ssHtmlOutput);
		ssHtmlOutput << "<p>" << unzipper.getShortFilePath(sSharedLibPath, sUnzipDirName) << "</p>";

		if(spluginType.compare(S_PLUGIN_TYPE_VMS) == 0)
		{
			opencctv::PluginLoader<opencctv::api::VmsConnector> loader;
			try
			{
				loader.loadPlugin(sSharedLibPath);
				opencctv::api::VmsConnector* vmsConn = loader.createPluginInstance();
				loader.deletePluginInstance(vmsConn);
				// appendHtmlTitle("VMS connector plugin verified!", ssHtmlOutput);
				bVerified = true;
			}
			catch(std::exception &e)
			{
				appendHtmlTitle("VMS connector plugin error:", ssHtmlOutput);
				ssHtmlOutput << "<p>" << e.what() << "</p>";
			}
		}
		else if(spluginType.compare(S_PLUGIN_TYPE_ANALYTIC) == 0)
		{
			opencctv::PluginLoader<analytic::api::Analytic> loader;
			try
			{
				loader.loadPlugin(sSharedLibPath);
				analytic::api::Analytic* analytic = loader.createPluginInstance();
				sAnalyticInputs = analytic->getInputStreamNames();
				loader.deletePluginInstance(analytic);
				appendHtmlTitle("Analytic plugin verified!", ssHtmlOutput);
				bVerified = true;
			}
			catch(std::exception &e)
			{
				appendHtmlTitle("Analytic plugin error:", ssHtmlOutput);
				ssHtmlOutput << "<p>" << e.what() << "</p>";
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		appendHtmlTitle("Shared Library not found. Invalid plugin archive!", ssHtmlOutput);
	}
	unzipper.removeUnzippedFiles();
	std::cout << generatePluginValidatorOutput(bVerified, ssHtmlOutput.str(), sAnalyticInputs, sAnalyticConfigs) << std::endl;
}
