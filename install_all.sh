#!/bin/bash
INSTALL_PATH=/usr/local/opencctv

color_green='\033[0;32m'
color_reset='\033[0m'

sudo mkdir -p ${INSTALL_PATH}
sudo chmod -R 777 ${INSTALL_PATH}
echo -e "${color_green}Installation directory created at $INSTALL_PATH.${color_reset}"

# Installing OpenCCTVServer_Starter
echo -e "${color_green}*Building OpenCCTV Server Starter...${color_reset}"
cd OpenCCTVServer_Starter/Release/
# make clean && make all
make all
OPENCCTVSERVER_STARTER_PATH=${INSTALL_PATH%%/}/OpenCCTVServerStarter/
mkdir -p ${OPENCCTVSERVER_STARTER_PATH}
cp OpenCCTVServer_Starter opencctv-server.config opencctv-server_starter.config ${OPENCCTVSERVER_STARTER_PATH}
echo -e "${color_green}Done. OpenCCTV Server Starter installed at $OPENCCTVSERVER_STARTER_PATH.${color_reset}"

# Installing OpenCCTVServer
echo -e "${color_green}*Building OpenCCTV Server...${color_reset}"
cd ../../OpenCCTVServer/Release/
# make clean && make all
make all
OPENCCTVSERVER_PATH=${INSTALL_PATH%%/}/OpenCCTVServer/
mkdir -p ${OPENCCTVSERVER_PATH}
cp OpenCCTVServer ${OPENCCTVSERVER_PATH}
# cp -a test/ ${OPENCCTVSERVER_PATH}
echo -e "${color_green}Done. OpenCCTV Server installed at $OPENCCTVSERVER_PATH.${color_reset}"

# Installing AnalyticStarter
echo -e "${color_green}*Building Analytic Starter...${color_reset}"
cd ../../AnalyticStarter/Release/
# make clean && make all
make all
STARTER_PATH=${INSTALL_PATH%%/}/AnalyticStarter
mkdir -p ${STARTER_PATH}
cp AnalyticStarter analytic-server.config analytic-runner.config ${STARTER_PATH}
echo -e "${color_green}Done. Analytic Starter installed at $STARTER_PATH.${color_reset}"

# Installing AnalyticRunner
echo -e "${color_green}*Building Analytic Runner...${color_reset}"
cd ../../AnalyticRunner/Release/
# make clean && make all
make all
RUNNER_PATH=${INSTALL_PATH%%/}/AnalyticRunner
mkdir -p ${RUNNER_PATH}
cp AnalyticRunner ${RUNNER_PATH}
echo -e "${color_green}Done. Analytic Runner installed at $RUNNER_PATH.${color_reset}"

# Setup OpenCCTV Web Application
echo -e "${color_green}*Setup Web Application...${color_reset}"
cd ../../OpenCCTVWeb/app/assets/programs/PluginArchiveValidator/Release/
# make clean && make all
make all
cd ../../../../../..
cp -a OpenCCTVWeb/ ${INSTALL_PATH}
echo -e "${color_green}Done. OpenCCTV Web Application setup at $RUNNER_PATH.${color_reset}"

# Create directories for analytics and vms_connectors
ANALYTICS_PATH=${INSTALL_PATH%%/}/analytics
mkdir -p ${ANALYTICS_PATH}
CONNECTORS_PATH=${INSTALL_PATH%%/}/vms_connectors
mkdir -p ${CONNECTORS_PATH}

echo -e "${color_green}OpenCCTV installation Done.${color_reset}"