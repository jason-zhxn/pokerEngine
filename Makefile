.PHONY: build cppinstall test cpptest clean lint cpplint cppformat format gdb

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    RELEASE_TYPE = Debug
else
    RELEASE_TYPE = Release
endif

build: cppinstall
	mkdir -p build
	cd build && cmake .. \
		-DCMAKE_TOOLCHAIN_FILE=../build/conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=$(RELEASE_TYPE) \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-G Ninja
	cd build && cmake --build .

cppinstall:
	pip install --upgrade pip
	pipx install --force conan
	conan profile detect --force
	pipx install --force ninja
	conan install . --build=missing --output-folder=build -s build_type=$(RELEASE_TYPE)

clean:
	@rm -rf build

test: cpptest

cpptest: build
	@cd build && ctest --output-on-failure

lint: cpplint

cpplint: build
	run-clang-tidy -p build
	find src -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run --Werror

format: cppformat

cppformat:
	find src -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
	run-clang-tidy -fix -j $(shell nproc 2>/dev/null || sysctl -n hw.ncpu) -p build

gdb:
	$(MAKE) build DEBUG=1
	cd build/bin && gdb poker_solver
