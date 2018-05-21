# the program to build
NAME		:= AnalyticServer

#Path
BASE 		:= ../src/
O		:= ${BASE}opencctv/
A		:= ${BASE}analytic/
A_DB		:= ${A}db/
A_DTO		:= ${A}dto/
A_UTIL		:= ${A}util/

O_UTIL		:= ${O}util/
O_MQ		:= ${O}mq/

# Build tools and flags
CXX			:= /usr/bin/g++
LD			:= /usr/bin/g++

PKG_CONFIG_LIBS =

#export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

CPPFLAGS	:= `pkg-config --cflags opencv` \
				-I /usr/local/opencctv/vgl/libVGL_bin/libVGL-2.0.4/include \
				-I /usr/include \
				-I /usr/include/mysql \
				-I /usr/local/include/ImageMagick-7 \
				-I /usr/local/opencctv/analytic/api \
				-I /usr/local/opencctv/vgl/FrameGrabber/src \
				-I ./ 
				
CXXFLAGS	:=  -O3 -fPIC -std=c++11#e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library path
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS		:= `pkg-config --libs opencv` \
				-L /usr/local/opencctv/vgl/libVGL_bin/libVGL-2.0.4/lib \
				-L /usr/local/opencctv/vgl/FrameGrabber/src \
				-L/usr/local/lib \
				-L/usr/lib \
				-L.

LIBS		:= -lFrameGrabber -lVGL-2.0.4-16 -lopencv_core -lboost_program_options \
			-lboost_filesystem -lopencv_contrib \
			-lopencv_calib3d -lopencv_highgui \
			-lopencv_imgproc -lopencv_ml -lopencv_legacy \
			-lopencv_objdetect -lopencv_gpu -lopencv_flann -lopencv_photo \
			-lopencv_video -lopencv_videostab -lopencv_features2d \
			-lopencv_superres -lopencv_stitching -lopencv_ocl -lopencv_nonfree \
			-ldl -lboost_system -lboost_thread -lzmq -lpthread -lmysqlcppconn   \

all		: $(NAME)
Other_L		:=${A}ConsumerThread.o ${A}AnalyticResult.o \
			${A_DB}AnalyticInstanceConfigGateway.o ${A_DB}AnalyticInstanceStreamGateway.o \
			${A_DB}AnalyticResultsGateway.o ${A_DB}DbConnector.o \
			${A_DTO}AnalyticInstanceConfig.o ${A_DTO}AnalyticInstanceStream.o \
			${A_UTIL}Config.o ${A_UTIL}log/colored/ColorLogger.o \
			${O}Exception.o ${O}Image.o \
			${O_UTIL}log/Loggers.o ${O_UTIL}Util.o ${O_UTIL}SharedLibLoader.o \
			${O_MQ}TcpMqSender.o ${O_MQ}TcpMqReceiver.o ${O_MQ}MqUtil.o \
			 

$(NAME)		: ${BASE}${NAME}.o ${Other_L} 
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo ' '
	@echo 'Finished building target: $(NAME)'
	@echo ' '
	
ProjectIncludes := /*.h	
%.o 		: %.cpp ${ProjectIncludes}
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
	@echo ' '

#Clean
DIRS := . $(shell find $(BASE) -type d)
GARBAGE_PATTERNS := *.o 
#*~ core .depend .*.cmd *.ko *.mod.c
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))	
clean	:
	rm -rf $(GARBAGE) $(NAME)
