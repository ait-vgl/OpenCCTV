# the program to build
NAME		:= AnalyticStarter

#Path
BASE 		:= ../src/
A		:= ${BASE}analytic/
O		:= ${BASE}opencctv/
R       := ${BASE}result/
A_UTIL		:= ${A}util/
A_XML		:= ${A}xml/
O_MQ		:= ${O}mq/
O_UTIL		:= ${O}util/
O_UTIL_LOG	:= ${O_UTIL}log/
R_DB		:= ${R}db/
R_DB_DTO	:= ${R_DB}dto/




# Build tools and flags
CXX			:= /usr/bin/g++
LD			:= /usr/bin/g++

PKG_CONFIG_LIBS =

CPPFLAGS	:= -I /usr/include \
				-I ./ 
				
CXXFLAGS	:=  -O3 -fPIC #e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library path
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS		:= -L/usr/lib \
				-L./

LIBS		:= -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread -lzmq -lmysqlclient -lmysqlcppconn -ldl
all		: $(NAME)
Other_L		:= ${A}Process.o ${A}AnalyticProcess.o ${A}ApplicationModel.o ${A}AnalyticServerController.o \
		${A_UTIL}Config.o ${A_UTIL}SharedLibLoader.o ${A_XML}AnalyticMessage.o \
		${O}Exception.o ${O_MQ}MqUtil.o ${O_UTIL}Util.o \
		${O_UTIL_LOG}Loggers.o ${O_UTIL_LOG}TempLogger.o ${O_UTIL_LOG}colored/ColorLogger.o \
        ${R}ResultAppInstController.o ${R}AnalyticInstController.o ${R}ResultsTxThread.o \
        ${R_DB}DbConnector.o ${R_DB}ResultsAppInstanceGateway.o ${R_DB}AnalyticServerGateway.o \
        ${R_DB}AnalyticResultGateway.o ${R_DB}AnalyticInstanceGateway.o \
        ${R_DB_DTO}ResultsAppInstance.o ${R_DB_DTO}AnalyticInstance.o ${R_DB_DTO}AnalyticResult.o

$(NAME)		: ${BASE}${NAME}.o ${Other_L} 
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo ' '
	@echo 'Finished building target: $(NAME)'
	@echo ' '
	
ProjectIncludes := /*.hpp
%.o 		: %.cpp ${ProjectIncludes }
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
	@echo ' '
	
#To remove object files and do a full build
full	: clean $(NAME) clean_obj

#Clean
DIRS := . $(shell find $(BASE) -type d)
GARBAGE_PATTERNS := *.o 
#*~ core .depend .*.cmd *.ko *.mod.c
GARBAGE := $(foreach DIR,$(DIRS),$(addprefix $(DIR)/,$(GARBAGE_PATTERNS)))	
clean	:
	@echo ' '
	@echo 'make clean'
	rm -rf $(GARBAGE) $(NAME)
	@echo ' '

#Clean only the object files	
clean_obj : 
	@echo ' '
	@echo 'Cleaning object files ..... '
	rm -rf $(GARBAGE)
	@echo ' '
