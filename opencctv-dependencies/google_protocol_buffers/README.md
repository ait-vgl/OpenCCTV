# Installing Google Protocol Buffers
## Version 2.6.1
### Linux

#### 1. Download protobuf
```sh
$ cd ~/Downloads/
$ mkdir protobuf
$ cd protobuf/
$ wget https://github.com/google/protobuf/releases/download/v2.6.1/protobuf-2.6.1.tar.gz
$ tar -xvzf protobuf-2.6.1.tar.gz
$ cd protobuf-2.6.1
```

#### 2. Install protobuf
```sh
$ ./configure --prefix=/usr/local
$ make -j4
$ make check
$ sudo make install
$ sudo ldconfig
```

#### 3. Check
Includes must be in `/usr/local/include` and Libraries must be in `/usr/local/lib`.
```sh
$ ls /usr/local/include/google
$ ls /usr/local/lib/libprotobuf*
```
