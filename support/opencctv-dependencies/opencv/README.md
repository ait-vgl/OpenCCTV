# Installing OpenCV
## Version 2.4.9
### Linux
This guide instructs installing only the required modules of OpenCV. If you need to install the full package, download following shell script and run it on your system.
- A shell script to install OpenCV: https://github.com/jayrambhia/Install-OpenCV/blob/master/Ubuntu/2.4/opencv2_4_9.sh

#### 1. Install dependencies of opencv
```sh
$ sudo apt-get update
$ sudo apt-get -y install build-essential g++
$ sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
```

#### 2. Download opencv
```sh
$ cd ~/Downloads/
$ mkdir opencv
$ cd opencv/
$ wget http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.9/opencv-2.4.9.zip
$ unzip opencv-2.4.9.zip
$ cd opencv-2.4.9/
```

#### 3. Init installation
```sh
$ mkdir release
$ cd release
$ cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
```

#### 4. Install opencv
Build OpenCV in N number of processes in parallel using `-jN` (replace N by number of CPU cores your computer has).
```sh
$ make -j4
$ sudo make install
$ sudo ldconfig
```

#### 5. Check
Includes must be in `/usr/local/include` and Libraries must in `/usr/local/lib`.
```sh
$ ls /usr/local/include
$ ls /usr/local/lib/libopencv*
```
