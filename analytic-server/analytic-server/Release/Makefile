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

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

CPPFLAGS	:= -I /usr/local/opencv-2.4.9/include \
				-I /usr/local/opencv-2.4.9/include/opencv \
				-I /usr/include \
				-I /usr/include/mysql \
				-I /usr/include/ImageMagick \
				-I /media/psf/Home/workspace/vgl/libVGL/include \
				-I /media/psf/Home/workspace/vgl/OpenCCTV/Ramesh/LibraryCreation/FrameGrabber/src/ \
				-I /media/psf/Home/workspace/vgl/OpenCCTV/Ramesh/LibraryCreation/FrameGrabber/src/analytic/api \
				-I ./ 
				
CXXFLAGS	:=  -O3 -fPIC #e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library path
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS		:= -L/usr/local/opencv-2.4.9/lib \
				-L/usr/local/lib \
				-L/usr/lib \
				-L/media/psf/Home/workspace/vgl/libVGL/lib \
				-L./

LIBS		:= -lFrameGrabber -lVGL -lopencv_core  -lboost_program_options \
			-lboost_filesystem -lopencv_contrib \
			-lopencv_calib3d -lopencv_highgui \
			-lopencv_imgproc -lopencv_ml -lopencv_legacy \
			-lopencv_objdetect -lopencv_gpu -lopencv_flann -lopencv_photo \
			-lopencv_video -lopencv_videostab -lopencv_features2d \
			-lopencv_superres -lopencv_stitching -lopencv_ocl -lopencv_nonfree \
			-ldl -lprotobuf -lboost_system -lboost_thread -lzmq -lpthread -lmysqlcppconn   \

all		: $(NAME)
Other_L		:=${A}ConsumerThread.o ${A}AnalyticResult.o ${A_DB}AnalyticInstanceConfigGateway.o ${A_DB}AnalyticInstanceStreamGateway.o ${A_DB}DbConnector.o \
		${A_DTO}AnalyticInstanceConfig.o ${A_DTO}AnalyticInstanceStream.o ${A_UTIL}/Config.o ${A_UTIL}log/colored/ColorLogger.o \
		${O}Exception.o ${O}Image.o \
		${O_UTIL}log/Loggers.o ${O_UTIL}Util.o ${O_UTIL}SharedLibLoader.o ${O_UTIL}serialization/gpb/ProtoBuf.o ${O_UTIL}serialization/gpb/analytic_result.pb.o ${O_UTIL}serialization/gpb/image.pb.o \
		${O_MQ}/TcpMqSender.o ${O_MQ}/TcpMqReceiver.o ${O_MQ}MqUtil.o \
			 

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
