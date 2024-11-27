.PHONY: build install cppinstall test pytest cpptest clean lint pylint cpplint format pyformat cppformat

RELEASE_TYPE = Release
PY_SRC = src/pysrc
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
	@cp -f build/bin/*.so $(PY_SRC)

install:
	poetry install

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
	conan install . --build=missing --output-folder=build

# Clean build
clean:
	@rm -rf build
	@rm -f $(PY_SRC)/*.so

# Run all tests
test: install build
	@poetry run pytest $(PY_SRC)/test/unit
	@cd build && ctest --output-on-failure

pytest: install build
	@poetry run pytest $(PY_SRC)/test/unit

# C++ testing
cpptest: build
	@cd build && ctest --output-on-failure

# Run all linting
lint: pylint cpplint

pylint:
	poetry run mypy $(PY_SRC)
	poetry run ruff check $(PY_SRC)
	poetry run ruff format --check $(PY_SRC)

# C++ linting
cpplint: build
	run-clang-tidy -p build
	find src -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run --Werror

# Run all formatting
format: pyformat cppformat

pyformat:
	poetry run ruff format $(PY_SRC)
	poetry run ruff check --fix $(PY_SRC)

# C++ formatting
cppformat:
	find src -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
	run-clang-tidy -fix -j $(shell nproc) -p build
