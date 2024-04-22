from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import apply_conandata_patches, copy, export_conandata_patches, get, replace_in_file, rm, rmdir
import os

class UpClientVSomeip(ConanFile):
    name = "up-client-vsomeip-cpp"
    package_type = "library"
    license = "Apache-2.0 license"
    homepage = "https://github.com/eclipse-uprotocol"
    url = "https://github.com/conan-io/conan-center-index"
    description = "C++ vSomeip library for GM-SDV-UP"
    topics = ("ulink client", "transport")
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    conan_version = None
    generators = "CMakeDeps"
    version = "0.1.1"
    exports_sources = "CMakeLists.txt", "conaninfo/*" ,"src/*" , "include/*"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_cross_compiling": [True, False],
        "build_testing": [True, False],
        "build_unbundled": [True, False],
    }

    default_options = {
        "shared": True,
        "fPIC": False,
        "build_cross_compiling": False,
        "build_testing": True,
        "build_unbundled": False,
    }
    def configure(self):
        self.options["vsomeip"].shared = True
        self.options["libuuid"].shared = True

    def requirements(self):
        self.requires("up-cpp/0.1.1-dev")
        self.requires("spdlog/1.13.0")
        self.requires("fmt/10.2.1")
        self.requires("boost/1.84.0")
        self.requires("protobuf/3.21.12")
        self.requires("gtest/1.14.0")
        self.requires("rapidjson/cci.20230929")
        # self.requires("vsomeip/3.4.10") // This shall be enabled once, conan.io starts supporting vsomeip library

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        if os.environ.get("BUILD_DIR_FULL_PATH") is not None:
            return
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        if os.environ.get("BUILD_DIR_FULL_PATH") is not None:
            self.output.info("BUILD_DIR_FULL_PATH: " + os.environ.get("BUILD_DIR_FULL_PATH"))
            build_folder_path = os.environ.get("BUILD_DIR_FULL_PATH")
            self.output.info("BUILD_DIR_FULL_PATH: " + build_folder_path)
            self.copy(pattern='*', dst='.', src=('%s/install') % (build_folder_path), symlinks=True, ignore_case=False)
        else:
            cmake = CMake(self)
            cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["up-client-vsomeip-cpp"]
