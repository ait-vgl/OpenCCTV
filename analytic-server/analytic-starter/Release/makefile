# the program to build
NAME		:= AnalyticStarter

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

CPPFLAGS	:= -I /usr/local/opencv-2.4.9/include \
				-I /usr/local/opencv-2.4.9/include/opencv \
				-I /usr/include \
				-I ./ 
				
CXXFLAGS	:=  -O3 -fPIC #e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library path
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS		:= -L/usr/local/opencv-2.4.9/lib \
				-L/usr/lib \
				-L./

LIBS		:= -lboost_system -lboost_filesystem -lboost_program_options -lzmq
all		: $(NAME)
Other_L		:= ${A}AnalyticProcess.o ${A}ApplicationModel.o \
		${A_UTIL}Config.o ${A}xml/AnalyticMessage.o \
		${O}Process.o ${O}Exception.o \
		${O_UTIL}log/Loggers.o ${O_UTIL}log/colored/ColorLogger.o ${O_UTIL}Util.o  ${O_MQ}MqUtil.o\
			 

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
