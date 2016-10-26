#include <iostream>
#include <stdio.h>
#include <string>
//using namespace std;

#include <boost/lexical_cast.hpp>

#include "opencctv/PluginLoader.hpp"
#include "opencctv/api/VmsConnector.hpp"
#include "Config.hpp"
//using namespace opencctv;

#include "ConsumerThread.hpp"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cerr << "Invalid number of arguments." << std::endl;
		std::cout << "Usage: " << argv[0]
				<< " <vms_connector_plugin_dir_path> <vms_connector_plugin_filename>"
				<< std::endl;
		std::cout << "Example: " << argv[0]
				<< " ~/vms_connectors/my_vms_connector my_vms_connector_plugin.so"
				<< std::endl;
		std::cout << "Provide VMS information using ./vms_connector.config"
				<< std::endl;
		return -1;
	}
	std::string vms_connector_plugin_dir_path = argv[1];
	std::string vms_connector_plugin_filename = argv[2];

	opencctv::ConcurrentQueue<opencctv::Image>* pQueue =
			new opencctv::ConcurrentQueue<opencctv::Image>(10);

	// Load configuration file
	std::string sConfigFilePath = "./vms_connector.config";
	Config* pConfig = NULL;
	try {
		pConfig = Config::getInstance(sConfigFilePath);
	} catch (std::runtime_error &e) {
		std::cerr << "Failed to read VMS Connector Configuration file. "
				<< e.what() << std::endl;
		return -1;
	}
	// Load VMS info from config file
	api::VmsConnectInfo vmsInfo;
	vmsInfo.sServerIp = pConfig->get(PROPERTY_SERVER_IP);
	vmsInfo.iServerPort = boost::lexical_cast<unsigned int>(
			pConfig->get(PROPERTY_SERVER_PORT));
	vmsInfo.sUsername = pConfig->get(PROPERTY_USERNAME);
	vmsInfo.sPassword = pConfig->get(PROPERTY_PASSWORD);
	vmsInfo.iWidth = boost::lexical_cast<unsigned int>(
			pConfig->get(PROPERTY_WIDTH));
	vmsInfo.iHeight = boost::lexical_cast<unsigned int>(
			pConfig->get(PROPERTY_HEIGHT));
	vmsInfo.sCameraId = pConfig->get(PROPERTY_CAMERA_ID);
	vmsInfo.bKeepAspectRatio = boost::lexical_cast<bool>(
			pConfig->get(PROPERTY_KEEP_ASPECT_RATIO));
	vmsInfo.bAllowUpsizing = boost::lexical_cast<bool>(
			pConfig->get(PROPERTY_ALLOW_UPSIZING));
	vmsInfo.iCompressionRate = boost::lexical_cast<unsigned int>(
			pConfig->get(PROPERTY_COMPRESSION_RATE));

	// Load VMS connector plugin
	api::VmsConnector* pVmsConnector = NULL;
	PluginLoader<api::VmsConnector> loader;
	std::string sPathToVmsConnectorPlugin = vms_connector_plugin_dir_path;
	if (*sPathToVmsConnectorPlugin.rbegin() != '/') // last char
			{
		sPathToVmsConnectorPlugin.append("/");
	}
	sPathToVmsConnectorPlugin.append(vms_connector_plugin_filename);
	try {
		loader.loadPlugin(sPathToVmsConnectorPlugin);
		std::cout << "VMS connector plugin loaded. "
				<< sPathToVmsConnectorPlugin << std::endl;
		pVmsConnector = loader.createPluginInstance();
		std::cout << "VMS connector instance created. " << std::endl;
	} catch (opencctv::Exception &e) {
		std::cerr << "Failed to load the Analytic plugin: "
				<< sPathToVmsConnectorPlugin << " Error: " << e.what()
				<< std::endl;
		return -1;
	}

	// Create Consumer thread to consume images
	ConsumerThread imageConsumer(pQueue);
	boost::thread* pConsumerThread = new boost::thread(imageConsumer);

	// Run analytic
	try {
		if (pVmsConnector->init(vmsInfo, vms_connector_plugin_dir_path)) {
			std::cout << "VMS connector init successful. " << std::endl;
			std::cout << "VMS connector produceImageObjects started. "
					<< std::endl;
			pVmsConnector->produceImageObjects(pQueue);
		} else {
			std::cerr
					<< "Failed to initialize the VMS connector. init() failed.";
		}
	} catch (std::exception &e) {
		std::cerr << "Exception in VMS connector: " << e.what() << std::endl;
	}

	return 0;
}
