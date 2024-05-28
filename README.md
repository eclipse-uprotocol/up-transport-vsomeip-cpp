# uProtocol C++ VSOMEIP Transport (up-client-vsomeip-cpp)

## Welcome!

This library provides a vsomeip-based uProtocol transport for C++ uEntities.

*_IMPORTANT NOTE:_ This project is under active development*

This module contains the vsomeip implementation of the Layer 1 `UTransport` API
from [up-cpp][cpp-api-repo].

## Getting Started

### Requirements:
- Compiler: GCC/G++ 11 or Clang 13
- Conan : 1.59 or latest 2.X

#### Conan packages and dependencies

Using the recipes found in [up-conan-recipes][conan-recipe-repo], build these Conan packages:

1. [up-core-api][spec-repo] - `conan create --version 1.5.8 --build=missing up-core-api/developer`
2. [up-cpp][cpp-api-repo] - `conan create --version 0.2.0 --build=missing up-cpp/developer`

Additionally, this project depends on the [vsomeip](https://github.com/COVESA/vsomeip) library, which is not a Conan package. 

**Note:** Do not enable signal handling in the vsomeip library.

Follow these steps to clone and build the vsomeip library:

```bash

# Install boost (version: 1.84.0): https://www.boost.org/users/history/version_1_84_0.html

$ cd boost
$ ./bootstrap.sh --prefix=<installation-directory> # can be left as default /usr/local
$ ./b2
$ sudo ./b2 install

# Clone the vsomeip repository
git clone https://github.com/COVESA/vsomeip.git

# Navigate into the cloned repository
cd vsomeip

# Checkout the desired version
git checkout 3.4.10

# Create a new directory for the build
mkdir build
cd build

# Run CMake to configure the build. Note: Replace "/usr/local/" with your desired install path.
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr/local/ -G Ninja ..

# Build the project using Ninja
ninja

# Install the library (you might need sudo privileges)
sudo ninja install

```

## How to Use the Library

To add up-client-vsomeip-cpp to your conan build dependencies, place following
in your conanfile.txt:

```
[requires]
up-client-vsomeip-cpp/[>=1.0.0 <2.0.0]

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

**NOTE:** If using conan version 1.59 Ensure that the conan profile is
configured to use ABI 11 (libstdc++11: New ABI) standards according to
[the Conan documentation for managing gcc ABIs][conan-abi-docs].

## Building locally

The following steps are only required for developers to locally build and test
up-client-vsomeip-cpp, If you are making a project that uses
up-client-vsomeip-cpp, follow the steps in the
[How to Use the Library](#how-to-use-the-library) section above.

### With Conan for dependencies

```
cd up-client-vsomeip-cpp
conan install .
cd build
cmake ../ -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . -- -j
```

Once the build completes, tests can be run with `ctest`.

### With dependencies installed as system libraries

**TODO** Verify steps for pure cmake build without Conan.

### Creating the Conan package

See: [up-conan-recipes][conan-recipe-repo]

## Show your support

Give a ⭐️ if this project helped you!

[conan-recipe-repo]: https://github.com/gregmedd/up-conan-recipes
[spec-repo]: https://github.com/eclipse-uprotocol/up-spec
[cpp-api-repo]: https://github.com/eclipse-uprotocol/up-cpp
[zenoh-repo]: https://github.com/eclipse-zenoh/zenoh-cpp
[conan-abi-docs]: https://docs.conan.io/en/1.60/howtos/manage_gcc_abi.html