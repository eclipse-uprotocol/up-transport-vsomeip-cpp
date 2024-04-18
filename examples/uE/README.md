**********How to compile and build examples using build script**********
Step 1:
Make sure gmultifi_client-vsomeip-cpp is compiled/built and conan package is created locally using following command:
source build.sh -r -install -conan x.y.z -rebuild-yes -s
Step 2:
Add following line : add_repo(gmultifi_client-vsomeip-cpp/examples/uE) in ultifi CMakeLists.txt file
Step 3:
Replace correct conanfile.py soft link from path ultifi/gmultifi_client-vsomeip-cpp/examples/uE/conanfile.py to ultifi/conanfile.py
Step 4:
Run following command
source build.sh -r -install -b vsomeipExamples_x86_64
Step 5:
Go to folder vsomeipExamples_x86_64/bin and run uE test cases mentioned below


**********Please run following commands from vsomeipExamples_x86_64/bin folder to run different uE test cases*********

Test case 1: uE subscribes and mE publish
env VSOMEIP_CONFIGURATION=./src/ustreamer-config.json  ./uE_subscriber

Test case 2: uE sends rpc request and mE responds.
env VSOMEIP_CONFIGURATION=./src/ustreamer-config.json  ./uE_rpcClient

Test case 3: mE sends rpc request and uE responds.
env VSOMEIP_CONFIGURATION=./src/ustreamer-config.json  ./uE_rpcServer

Test case 4: mE subscribes and uE publish
env VSOMEIP_CONFIGURATION=./src/ustreamer-config.json ./uE_publisher