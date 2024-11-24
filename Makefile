.PHONY: build cppinstall test cpptest clean lint cpplint cppformat format

RELEASE_TYPE = Release
CPP_SRC = src/cppsrc

# Build C++ project
build: cppinstall
	mkdir -p build
	cd build && cmake .. \
		-DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=$(RELEASE_TYPE) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-G Ninja
	cd build && cmake --build .

# Install Conan
cppinstall:
	pip install --upgrade pip
	pipx install --force conan
	@if [ ! -f ~/.conan2/profiles/default ]; then \
		conan profile detect; \
	else \
		echo "Conan default profile already exists, skipping profile detection."; \
	fi
	pipx install --force ninja
	conan install . --build=missing

# Clean current build
clean:
	@rm -rf build

# Run all tests
test: cpptest

# C++ testing
cpptest: build
	@cd build && ctest --output-on-failure


# Run all linting
lint: cpplint

# C++ linting
cpplint: build
	run-clang-tidy -p build
	find src -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run --Werror

# Run all formatting
format: cppformat

# C++ formatting
cppformat:
	find src -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
	run-clang-tidy -fix -j $(shell nproc) -p build
