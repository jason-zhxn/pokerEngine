from conan import ConanFile
from conan.tools.cmake import cmake_layout


class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self) -> None:
        self.requires("gtest/1.15.0")
        self.requires("pybind11/2.13.6")

    def layout(self) -> None:
        cmake_layout(self)
