# the program to build
NAME            := libfake-face-detection.so

# Build tools and flags
CXX                     := /usr/bin/g++
LD                      := /usr/bin/g++

PKG_CONFIG_LIBS = libavcodec libavformat

export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

CPPFLAGS        := -I /usr/local/opencv-2.4.9/include \
                                -I /usr/local/opencv-2.4.9/include/opencv \
                                -I /usr/include \
                                `pkg-config --cflags $(PKG_CONFIG_LIBS)` \
                                -I ./ 
                             
CXXFLAGS        :=  -O3 -fPIC #e.g., CXXFLAGS=-g -O0 for debugging

# To keep the makefile for 32 and 64 in sync we add 64 and 32-bit library paths
# If you are targeting only 32 bit for you can remove the entries containing "64"
LDFLAGS         := -L/usr/local/opencv-2.4.9/lib \
                                -L/usr/lib \
                                -L.

LIBS            := -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui \
			-lopencv_ml -lopencv_video -lopencv_legacy -lopencv_calib3d \
			-lopencv_flann -lboost_program_options -lopencv_objdetect \
			-lopencv_contrib -lopencv_features2d \
			`pkg-config --libs $(PKG_CONFIG_LIBS)`

all		    : $(NAME)

$(NAME)		: 
	$(LD) $(LDFLAGS) -shared -o $@ $^ $(LIBS)
	
ProjectIncludes := ./*.h	
%.o 		: %.cpp ${ProjectIncludes}
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean		:
	$(RM) *.o $(NAME)
