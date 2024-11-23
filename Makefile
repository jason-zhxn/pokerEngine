.PHONY: build install cppinstall test cpptest clean lint cpplint cppformat dependencies

RELEASE_TYPE = Release
CPP_SRC = src/cppsrc

# Default build target
build: cppinstall
	mkdir -p build
	cd build && cmake .. \
		-DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=$(RELEASE_TYPE) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-G Ninja
	cd build && cmake --build .

# Install dependencies
install: dependencies cppinstall

dependencies:
	python -m pip install --upgrade pip
	python -m pip install conan ninja
	conan profile detect

cppinstall:
	conan install . --build=missing --output-folder=build

# Run all tests
test: build cpptest

# Run C++ tests
cpptest: build
	@cd build && ctest --output-on-failure

# Clean build directory
clean:
	@rm -rf build

# Linting
lint: cpplint

# Lint C++ code with Clang-Tidy
cpplint: build
	run-clang-tidy -p build
	find $(CPP_SRC) -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run --Werror

# Format C++ code
cppformat:
	find $(CPP_SRC) -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i

