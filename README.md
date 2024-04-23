# up-client-vsomeip-cpp
## 1. Overview
This library provides a C++ vsomeip based implementation of uTransport

*_IMPORTANT NOTE:_ This project is under active development*

This module contains the implementation for pub-sub and RPC API`s defined in the uProtocol spec

## 2. Getting Started
### 2.1. Requirements:
- Compiler: GCC/G++ 11 or Clang 13
- Ubuntu 22.04
- conan : 1.59 or latest 2.X
- cmake: 3.20.0+
- ninja: 1.10.0+ (optional, can instead use 'make' as well)

### 2.2. Dependencies
#### 2.2.1. up-cpp
Install up-cpp library: https://github.com/eclipse-uprotocol/up-cpp

#### 2.2.1. vsomeip
- Install boost (version: 1.84.0): https://www.boost.org/users/history/version_1_84_0.html
```
$ cd boost
$ ./bootstrap.sh
$ ./b2
$ sudo ./b2 install
```

- Install vsomeip (version: 3.4.10):

Note: Do not enable signal handlling in vsomeip library
```
$ git clone https://github.com/COVESA/vsomeip.git
$ git cd vsomeip
$ git checkout 3.4.10
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/ -G Ninja ..
$ ninja
$ sudo ninja install
```

## 3. How to build library
Two ways:

Note: If you are planning on running the examples, follow, 3.2 Build as conan package instructions
### 3.1. Build locally
```
$ git clone <repo_link>
$ cd up-client-vsomeip-cpp
$ mkdir build
$ cd build
$ export LOCAL_VSOMEIP_PATH=/usr/local
$ conan install .. -of .
$ cmake -S .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

### 3.2. Build as conan package
```
$ cd up-client-vsomeip-cpp
$ export LOCAL_VSOMEIP_PATH=/usr/local
$ conan create .
```

## 4. Examples
Refer to [EXAMPLES](examples/README.md)

## 5. Troubleshooting for common issues
- If not able to find a library, check using:
```
ldconfig -p |grep <lib_name>
```