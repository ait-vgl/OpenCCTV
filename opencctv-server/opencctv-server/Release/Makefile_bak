# the program to build
NAME		:= OpenCCTVServerMain

#Path
BASE 		:= ../src/
O		:= ${BASE}opencctv/
A		:= ${BASE}analytic/
A_DB		:= ${A}db/
A_DTO		:= ${A}dto/
A_UTIL		:= ${A}util/

O_UTIL		:= ${O}util/
O_MQ		:= ${O}mq/
O_DB		:= ${O}db/
O_DTO		:= ${O}dto/

# Build tools and flags
CXX			:= /usr/bin/g++
LD			:= /usr/bin/g++

PKG_CONFIG_LIBS =

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

CPPFLAGS	:= -I /usr/local/opencv-2.4.9/include \
				-I /usr/local/opencv-2.4.9/include/opencv \
				-I /usr/include \
				-I /usr/include/mysql \
				-I ./ 
				
CXXFLAGS	:=  -O3 -fPIC #e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library path
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS		:= -L/usr/local/opencv-2.4.9/lib \
				-L/usr/lib \
				-L./

LIBS		:=  -lboost_thread -lboost_system -lboost_program_options -lboost_filesystem -lprotobuf -lzmq -ldl -lmysqlcppconn -pthread

all		: $(NAME)
Other_L		:=${A}AnalyticServerManager.o ${A}AnalyticServer.o ${A}AnalyticServer.o ${A}xml/AnalyticMessage.o ${A}AnalyticData.o ${A}AnalyticResult.o \
		${O}ResultRouterThread.o ${O}OpenCCTVServerManager.o ${O}OpenCCTVServer.o ${O}Exception.o ${O}Image.o ${O}ServerController.o ${O}Process.o \
		${O_UTIL}log/Loggers.o ${O_UTIL}xml/OpenCCTVServerMessage.o ${O_UTIL}Config.o ${O_UTIL}serialization/Serializers.o ${O_UTIL}serialization/gpb/ProtoBuf.o ${O_UTIL}serialization/gpb/image.pb.o ${O_UTIL}serialization/gpb/analytic_result.pb.o ${O_UTIL}log/colored/ColorLogger.o ${O_MQ}MqUtil.o ${O_MQ}TcpMqReceiver.o \
		${O_DB}AnalyticServerGateway.o ${O_DB}AnalyticResultGateway.o ${O_DB}DbConnector.o ${O_DB}AnalyticServerGateway.o $ ${O_DB}AnalyticInstanceGateway.o ${O_DB}AnalyticInstanceGateway.o ${O_DTO}AnalyticServerDTO.o ${O_DTO}AnalyticInstance.o 
		
			 

$(NAME)		: ${BASE}${NAME}.o ${Other_L}
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	
ProjectIncludes := /*.h	
%.o 		: %.cpp ${ProjectIncludes}
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
	
#Clean	
DIRS := . $(shell find $(BASE) -type d)
GARBAGE_PATTERNS := *.o 
#*~ core .depend .*.cmd *.ko *.mod.c
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))	
clean	:
	rm -rf $(GARBAGE) $(NAME)
