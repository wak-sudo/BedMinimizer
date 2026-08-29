OPT_FLAGS := -O3 -march=native -flto=auto -Wall -Wextra -Wpedantic -Werror

.PHONY: all build test run clean perf perf-record

all: build

build:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$(OPT_FLAGS)" -DBUILD_TESTS=OFF
	cmake --build build -j

test:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$(OPT_FLAGS)" -DBUILD_TESTS=ON
	cmake --build build -j
	ctest --test-dir build --output-on-failure -j

test_verbose:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$(OPT_FLAGS)" -DBUILD_TESTS=ON
	cmake --build build -j
	ctest --test-dir build -V -I 12,12

run: build
	./build/Main

perf:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_FLAGS="$(OPT_FLAGS) -fno-omit-frame-pointer" -DBUILD_TESTS=OFF
	cmake --build build -j
	perf record -g ./build/Main
	perf report -g

perf-record:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_FLAGS="$(OPT_FLAGS) -fno-omit-frame-pointer" -DBUILD_TESTS=OFF
	cmake --build build -j
	perf record -g ./build/Main

clean:
	rm -rf build