FROM opencctv:opencctv

ARG INSTALL_PATH=/usr/local/opencctv

ARG color_green='\033[0;32m'
ARG color_reset='\033[0m'


# Installing OpenCCTVServer_Starter
ARG OPENCCTVSERVER_STARTER_PATH=${INSTALL_PATH}/OpenCCTVServerStarter/
RUN mkdir -p ${OPENCCTVSERVER_STARTER_PATH}
COPY OpenCCTVServer_Starter opencctv-server.config opencctv-server_starter.config ${OPENCCTVSERVER_STARTER_PATH}
RUN echo -e "${color_green}Done. OpenCCTV Server Starter installed at $OPENCCTVSERVER_STARTER_PATH.${color_reset}"

# Installing OpenCCTVServer
ARG OPENCCTVSERVER_PATH=${INSTALL_PATH}/OpenCCTVServer/
RUN mkdir -p ${OPENCCTVSERVER_PATH}
COPY OpenCCTVServer/OpenCCTVServer ${OPENCCTVSERVER_PATH}
RUN echo -e "${color_green}Done. OpenCCTV Server installed at $OPENCCTVSERVER_PATH.${color_reset}"

RUN mkdir -p /usr/local/opencctv/analytics
RUN mkdir -p /usr/local/opencctv/vms_connectors
#VOLUME ["/usr/local/opencctv/analytics","/usr/local/opencctv/vms_connectors"]

EXPOSE 4444

WORKDIR ${OPENCCTVSERVER_STARTER_PATH}
CMD ["./OpenCCTVServer_Starter"]
