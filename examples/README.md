# up-client-vsomeip-cpp examples
## 1. Overview
These example are designed to run as a pair, and there are 4 pairs:
```
Pair-1: uE_publisher  <--> up-client-vsomeip-cpp <--> mE_subscriber
Pair-2: uE_rpcServer  <--> up-client-vsomeip-cpp <--> mE_rpcClient
Pair-3: uE_subscriber <--> up-client-vsomeip-cpp <--> mE_publisher
Pair-4: uE_rpcClient  <--> up-client-vsomeip-cpp <--> mE_rpcServer

```
- uE is that side of the SOME/IP bridge that uses uProtocol
- mE is that side of the SOME/IP bridge that uses SOME/IP protocol

## 2. How to build examples
### 2.1. Build uE side
Assuming library was built as a conan package
```
$ cd up-client-vsomeip-cpp/examples/uE
$ mkdir build
$ cd build

$ conan install .. -of .
$ cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

### 2.2. Build mE side
```
$ cd up-client-vsomeip-cpp/examples/mE
$ mkdir build
$ cd build
$ cmake -G Ninja ..
$ ninja
```

## 3. How to run examples
### 3.1. Setup & Overview
- Setup run environment for boost and vsomeip libraries
- Assuming both boost and vsomeip libraries were installed in /usr/local/lib (custom paths also work):
```
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
```
Some points to remember:

- For pub/sub examples, execute the publisher before subscriber
- For rpc examples, execute the rpcServer before rpcClient
- These examples can either be run on the same machine or different machines connected over ethernet. If running on different machines, make sure they are on same subnet (can reach each other), a good test would be to check if they can ping each other. Also, their IP addesses need to be updated in corresponding config.json files under unicast tag.
- It is required to enable multicasting on network interface, use the provided script [setupNwForSomeip.sh](./setupNwForSomeip.sh). If using two separate machines, then it should be run for both of them. Make sure to update the interface name correctly in that script.

```
$ chmod +x setupNwForSomeip.sh
$ ./setupNwForSomeip.sh
```


### 3.2. Running uE
- Execute examples located inside uE/build folder, using command:
```env VSOMEIP_CONFIGURATION=<someip-config.json location> ./uE_<X>```

for example:
```
$ env VSOMEIP_CONFIGURATION=./someip-config.json ./uE_subscriber
```
Note: make sure ```someip-config.json``` file is correctly located.

### 3.3. Running mE
Execute examples located inside mE/build folder, using commands:
- Test case 1:
```env VSOMEIP_CONFIGURATION=../config/mE_publisher-config.json VSOMEIP_APPLICATION_NAME=mE_publisher ./mE_publisher```

- Test case 2:
```env VSOMEIP_CONFIGURATION=../config/mE_rpcServer-config.json VSOMEIP_APPLICATION_NAME=mE_rpcServer ./mE_rpcServer```

- Test case 3:
```env VSOMEIP_CONFIGURATION=../config/mE_rpcClient-config.json VSOMEIP_APPLICATION_NAME=mE_rpcClient ./mE_rpcClient```

- Test case 4:
```env VSOMEIP_CONFIGURATION=../config/mE_subscriber-config.json VSOMEIP_APPLICATION_NAME=mE_subscriber ./mE_subscriber```

Note: make sure ```mE_<X>-config.json``` file is correctly located.
