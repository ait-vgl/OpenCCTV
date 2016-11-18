# Installing Zero MQ
## Version 4.0.5
### Linux

#### 1. Install dependencies of zmq
```sh
$ sudo apt-get update
$ sudo apt-get -y install build-essential g++ autoconf automake libtool uuid-dev
$ sudo apt-get -y install unzip
```

#### 2. Download zmq
```sh
$ cd ~/Downloads/
$ mkdir zmq
$ cd zmq/
$ wget https://github.com/zeromq/zeromq4-1/releases/download/v4.1.6/zeromq-4.1.6.tar.gz
$ tar -xvzf zeromq-4.1.6.tar.gz
$ cd zeromq-4.1.6

```

#### 3. Install zmq
```sh
$ ./configure
$ make -j4
$ sudo make install
$ sudo ldconfig
```

#### 4. Get zmq.hpp
We should download `zmq.hpp` from zeromq github repository and copy it into `/usr/local/include`.
```sh
$ cd ..
$ wget https://github.com/zeromq/cppzmq/archive/master.zip
$ unzip master.zip
$ cd cppzmq-master
$ sudo cp zmq.hpp /usr/local/include
```

#### 5. Check
Includes must be in `/usr/local/include` and Libraries must be in `/usr/local/lib`.
```sh
$ ls /usr/local/include/zmq*
$ ls /usr/local/lib/libzmq*
```
