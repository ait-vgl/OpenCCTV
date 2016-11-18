# Installing Boost Libraries
## Version 1.54.0
### Linux

#### 1. Install dependencies of boost
```sh
$ sudo apt-get update
$ sudo apt-get -y install build-essential g++ python-dev autotools-dev libicu-dev libbz2-dev libzip-dev
```

#### 2. Download boost
```sh
$ cd ~/Downloads/
$ mkdir boost
$ cd boost/
$ wget http://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.gz
$ tar -xvzf boost_1_54_0.tar.gz
$ cd boost_1_54_0
```

#### 3. Init installation
```sh
$ ./bootstrap.sh --prefix=/usr/local
$ ./b2 --show-libraries
```

#### 4. Install boost
For OpenCCTV, Boost's `system`, `thread`, `program_options`, `filesystem` `chrono` libraries are needed. Set the libraries that you need to build using `--with-<library_name>`.
Build the Boost in N number of processes in parallel using `-jN` (replace N by number of CPU cores your computer has).
```sh
$ sudo ./b2 --with-system --with-thread --with-program_options --with-filesystem --with-chrono -j4 --target=shared,static install
$ sudo ldconfig
```

#### 5. Check
Includes must be in `/usr/local/include/boost` and Libraries must in `/usr/local/lib`.
```sh
$ ls /usr/local/include
$ ls /usr/local/lib/libboost*
```

###### Further info
If you have already installed an older Boost version and want to uninstall it and install the new version, follow these steps:-
- If Boost was installed using `apt-get`, uninstall it using `apt-get`.
```sh
$ sudo apt-get --purge remove libboost-dev-all libboost-dev libboost-doc
$ sudo apt-get autoremove
```
- If Boost was installed from the source, delete all Boost include and library files.
```sh
$ sudo rm -rf /usr/local/include/boost
$ sudo rm -rf /usr/local/lib/libboost*
```
