# Integrating and Packaging JSON_Lib

This guide explains how to consume, install, and package **JSON_Lib** in downstream C++ projects using CMake and standard package managers.

---

## 1. CMake Integration

### 1.1 Installed Package (`find_package`)

Once `JSON_Lib` is installed on your system or in a dedicated prefix, you can consume it directly with standard CMake configuration:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyApplication CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate the installed JSON_Lib package
find_package(JSON_Lib CONFIG REQUIRED)

add_executable(my_app main.cpp)

# Links headers, includes, and compilation flags automatically
target_link_libraries(my_app PRIVATE JSON_Lib::JSON_Lib)
```

In your C++ code:
```cpp
#include <JSON_Lib.hpp>
// Or namespaced include:
// #include <JSON_Lib/JSON_Lib.hpp>

int main() {
    JSON_Lib::JSON json;
    json.parse(JSON_Lib::BufferSource{ R"({"status": "ready"})" });
    return 0;
}
```

### 1.2 CMake `FetchContent` (No Pre-installation)

To fetch and compile `JSON_Lib` as part of your build without pre-installing:

```cmake
include(FetchContent)

FetchContent_Declare(
    JSON_Lib
    GIT_REPOSITORY https://github.com/clockworkengineer/JSON_Lib.git
    GIT_TAG        v1.0.0
)

# Optional: configure build flags before loading
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(JSON_Lib)

target_link_libraries(my_app PRIVATE JSON_Lib::JSON_Lib)
```

### 1.3 Git Submodule

If vendoring as a git submodule in `extern/JSON_Lib`:

```cmake
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/JSON_Lib)

target_link_libraries(my_app PRIVATE JSON_Lib::JSON_Lib)
```

---

## 2. Installing JSON_Lib

### Local / System Installation

To install `JSON_Lib` to standard system locations (e.g. `/usr/local` on Linux/macOS or `Program Files` on Windows):

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
sudo cmake --install build
```

### Custom Prefix Installation

To install to a non-standard prefix (e.g. for packaging or isolated environments):

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/json_lib
cmake --build build
cmake --install build
```

Then in downstream projects:
```sh
cmake -B build -DCMAKE_PREFIX_PATH=/opt/json_lib
```

---

## 3. CMake Configuration Options & Build Profiles

JSON_Lib supports a wide array of compile-time configuration flags:

| Option | Default | Description |
|---|---|---|
| `BUILD_SHARED_LIBS` | `OFF` | Build shared library (`.so`, `.dylib`, `.dll`) instead of static |
| `BUILD_TESTING` | `ON` | Build Catch2 unit test suite |
| `BUILD_EXAMPLES` | `ON` | Build example programs |
| `JSON_LIB_ENABLE_LTO` | `OFF` | Enable Link-Time Optimization (IPO) |
| `JSON_LIB_ENABLE_ASAN` | `OFF` | Enable AddressSanitizer |
| `JSON_LIB_ENABLE_UBSAN` | `OFF` | Enable UndefinedBehaviorSanitizer |
| `JSON_LIB_ENABLE_TSAN` | `OFF` | Enable ThreadSanitizer |
| `JSON_LIB_DOWNLOAD_JSON_TEST_SUITE` | `OFF` | Fetch and execute the official `nst/JSONTestSuite` |
| `JSON_LIB_BUILD_FUZZERS` | `OFF` | Build LLVM `libFuzzer` harnesses |

### Constrained & Embedded Profiles

| Profile Flag | Description |
|---|---|
| `-DJSON_LIB_EMBEDDED=ON` | Enables embedded mode (disables stdio/file I/O, minimizes memory usage) |
| `-DJSON_LIB_NO_EXCEPTIONS=ON` | Disables C++ exceptions; errors abort or trigger error callbacks |
| `-DJSON_LIB_NO_STDIO=ON` | Strips all `std::fstream` and file operations; purely memory/buffer based |
| `-DJSON_LIB_NO_DYNAMIC_MEMORY=ON` | Disables dynamic heap allocations (uses fixed-size buffers) |

---

## 4. Binary Packaging with CPack

JSON_Lib is configured with CPack to generate distributable archive packages:

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cd build
cpack
```

This generates:
- `JSON_Lib-<version>-<system>.tar.gz`
- `JSON_Lib-<version>-<system>.zip`

Each package contains:
- Headers under `include/` and `include/JSON_Lib/`
- Static or shared library under `lib/`
- CMake configuration targets under `lib/cmake/JSON_Lib/`
