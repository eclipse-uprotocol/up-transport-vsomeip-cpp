# gmultifi_uplat_client-vsomeip-cpp
prereq libs (not available in conan central, require manual builds):
vsomeip

Compilation steps:
1. compile up-cpp: follow their github page for installation

2. compile boost-1.84.0: write instructions from streamer/doc folder (boost required for vsomeip)
install in /usr/local/lib

3. compile vsomeip: write instructions from streamer/doc folder
can also be installed in /usr/local/lib
not required though, can be stored some place locally
for example /tmp/vsomeip/install
this install folder will have lib and include folders

4. now compile this library using following steps
two ways:
4.1
cd up-client-vsomeip-cpp
mkdir build
cd build

export LOCAL_VSOMEIP_PATH=/tmp/vsomeip/install
conan install .. -of .
cmake -S .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .

4.2
cd up-client-vsomeip-cpp
conan create .


5. compile examples
cd up-client-vsomeip-cpp/examples/uE
mkdir build
cd build

conan install .. -of .
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .

6. execute examples
for vsomeip lib:
export LD_LIBRARY_PATH=/tmp/vsomeip/install/lib
for boost lib:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

execute examples located inside build folder
./uE_publisher

7. trouble shoot
if not able to find a library
ldconfig -p |grep boost