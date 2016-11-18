# the program to build
NAME		:= PluginArchiveValidator

#Path
BASE 		:= ../src/
O		:= ${BASE}opencctv/
O_UTIL		:= ${O}util/

LibVGL		:= ~/workspace/vgl/libVGL/


# Build tools and flags
CXX		:= /usr/bin/g++
LD		:= /usr/bin/g++

PKG_CONFIG_LIBS =

CPPFLAGS	:= -I /usr/local/opencv-2.4.9/include \
				-I /usr/local/opencv-2.4.9/include/opencv \
				-I /usr/include \
				-I /usr/include/ImageMagick \
				-I ${LibVGL}include/ \
				-I ./ 
				
CXXFLAGS	:=  -O3 -fPIC #e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library path
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS		:= -L/usr/local/opencv-2.4.9/lib \
				-L/usr/lib \
				-L ${LibVGL}libVGL/lib \
				-L./

LIBS		:= -ldl -lboost_filesystem -lboost_system
all		: $(NAME)
Other_L		:= ${O}Exception.o ${O_UTIL}Unzipper.o ${O_UTIL}Util.o ${O_UTIL}SharedLibLoader.o


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
