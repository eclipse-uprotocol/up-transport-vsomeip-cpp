**********How to compile and build inside mE folder**********
mkdir build
cd build
cmake -G Ninja ..
ninja

**********Please run following commands from build folder to run different mE test cases*********

Test case 1: uE subscribes and mE publish
env VSOMEIP_CONFIGURATION=../config/mE_publisher-config.json VSOMEIP_APPLICATION_NAME=mE_publisher ./mE_publisher

Test case 2: uE sends rpc request and mE responds.
env VSOMEIP_CONFIGURATION=../config/mE_rpcServer-config.json VSOMEIP_APPLICATION_NAME=mE_rpcServer ./mE_rpcServer

Test case 3: mE sends rpc request and uE responds.
env VSOMEIP_CONFIGURATION=../config/mE_rpcClient-config.json VSOMEIP_APPLICATION_NAME=mE_rpcClient ./mE_rpcClient

Test case 4: mE subscribes and uE publish
env VSOMEIP_CONFIGURATION=../config/mE_subscriber-config.json VSOMEIP_APPLICATION_NAME=mE_subscriber ./mE_subscriber