from conan import ConanFile

class PokerEngineConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("gtest/1.15.0")  # Google Test
        self.requires("pybind11/2.13.6")  # Python bindings

