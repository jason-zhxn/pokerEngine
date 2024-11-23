.PHONY: build cppinstall test cpptest clean lint cpplint format cppformat dependencies

RELEASE_TYPE = Release
CPP_SRC = src/cppsrc

build: cppinstall
	mkdir -p build
	cd build && cmake .. \
		-DCMAKE_TOOLCHAIN_FILE=$(RELEASE_TYPE)/generators/conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=$(RELEASE_TYPE) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-G Ninja
	cd build && cmake --build .

cppinstall:
	conan install . --build=missing

test: build cpptest

cpptest: build
	@cd build && ./intern_tests

clean:
	@rm -rf build

lint: cpplint

cpplint: build
	run-clang-tidy -j $(shell nproc) -p build
	find $(CPP_SRC) -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i

format: cppformat

cppformat: build
	find $(CPP_SRC) -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
	run-clang-tidy -fix -j $(shell nproc) -p build

dependencies:
	# Ensure required tools are installed
	pip install --upgrade pip
	pipx install conan
	conan profile detect
	bash < .github/scripts/conan-profile.sh
	pipx install ninja

