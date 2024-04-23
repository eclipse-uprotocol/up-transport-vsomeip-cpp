from conan import ConanFile

class gmultifi_client_vsomeip_uE(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain", "PkgConfigDeps", "VirtualRunEnv", "VirtualBuildEnv"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_testing": [True, False],
        "build_unbundled": [True, False],
        "build_cross_compiling": [True, False],
    }

    default_options = {
        "shared": False,
        "fPIC": False,
        "build_testing": False,
        "build_unbundled": True,
        "build_cross_compiling": False,
    }

    def requirements(self):
        self.requires("up-client-vsomeip-cpp/0.1.1")
        self.requires("spdlog/1.13.0")
        self.requires("fmt/10.2.1")
        self.requires("up-cpp/0.1.1-dev")
        self.requires("protobuf/3.21.12" + ("@cross/cross" if self.options.build_cross_compiling else ""))

    def imports(self):
        if self.options.build_testing:
            self.copy("*.so*", dst="lib", keep_path=False)
