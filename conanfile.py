from conan import ConanFile
from conan.tools.cmake import cmake_layout

class PokerEngineConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("gtest/1.15.0")
        self.requires("pybind11/2.13.6")

    def layout(self):
        cmake_layout(self)
        # Place everything directly under the `build/` folder
        self.folders.build = "build"
        self.folders.generators = "build"

