# JSON_Lib

[![CI](https://github.com/clockworkengineer/JSON_Lib/actions/workflows/ci.yml/badge.svg)](https://github.com/clockworkengineer/JSON_Lib/actions/workflows/ci.yml)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE.txt)
[![Buy Me a Coffee](https://img.shields.io/badge/Donate-Buy%20Me%20A%20Coffee-orange.svg)](https://www.buymeacoffee.com/clockworkengineer)

**JSON_Lib** is a modern, high-performance C++23 library for parsing, manipulating, and serializing JSON data. It provides a clean, intuitive API and is designed for efficiency, extensibility, standards compliance, and embedded-target support.

---

## Features

- C++23 with full `std::string_view`, concepts, and structured-binding support
- Strict adherence to **SOLID** architectural design principles
- Parse, create, traverse, and serialize JSON objects and arrays
- Exception-based error handling **and** an exception-free `Result<T>` API for embedded/RTOS targets
- Unicode and multi-encoding support (UTF-8, UTF-8 BOM, UTF-16 LE/BE, UTF-32 LE/BE) via extensible `IEncodingHandler` strategies
- Fine-grained node visitors (`INodeVisitor`, `IValueVisitor`, `IContainerVisitor`)
- Modular CMake build — library, tests, and examples are independent targets
- Embedded-friendly build mode: no heap, no stdio, no exceptions, configurable resource limits
- Pluggable stringify backends: compact JSON, pretty-print, Bencode, XML, YAML
- Extensive examples and [RFC 8259 / ECMA-404 Conformance Documentation](docs/conformance.md)

---

## Getting Started

### Prerequisites

- C++ compiler with C++23 support (GCC 11+, Clang 14+, MSVC 19.36+)
- CMake 3.21 or later

## Portability

JSON_Lib is designed to build on major C++23 toolchains and is portable across Windows, Linux, and macOS.
The library enforces minimum supported compiler versions at configure time:
- GCC 13 or newer
- Clang 16 or newer
- MSVC 19.36 or newer

The library exposes compatibility macros in `classes/include/implementation/common/JSON_Attributes.hpp`:
`JSON_LIB_COMPILER_MSVC`, `JSON_LIB_COMPILER_CLANG`, `JSON_LIB_COMPILER_GCC`, `JSON_LIB_COMPILER_UNKNOWN`,
`JSON_LIB_PLATFORM_WINDOWS`, `JSON_LIB_PLATFORM_UNIX`, and `JSON_LIB_PLATFORM_POSIX`.

If an untested compiler is used, CMake will continue with a warning but the library may not be fully validated.

To validate a specific compiler build:

```sh
mkdir build-clang && cd build-clang
cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_STANDARD=23 ..
cmake --build .
```

## Dependencies

`JSON_Lib` has **zero runtime dependencies** beyond the C++ standard library.

- Library runtime: C++ standard library only
- Public headers are deliberately designed to include only standard headers and stable public API headers
- Build system: CMake 3.21+
- Tests/benchmarks only: Catch2 (resolved via `find_package(Catch2 3)` or fetched via CMake `FetchContent`)
- Documentation only: Doxygen is optional for generating API docs

### CMake Integration

To consume installed `JSON_Lib` in your downstream CMake project:

```cmake
find_package(JSON_Lib CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE JSON_Lib::JSON_Lib)
```

See [docs/packaging.md](docs/packaging.md) for `FetchContent`, submodule, installation, and packaging instructions.

### Public headers

For most use cases, include the main facade:

```cpp
#include "JSON_Lib.hpp"
```

If you only need low-level I/O helpers for sources and destinations, include:

```cpp
#include "JSON_IO.hpp"
```

Avoid direct inclusion of internal `implementation/*` headers in applications unless you are extending or customizing the library internals.

Applications linking `JSON_Lib` do not need to ship or install any third-party runtime package.

### Build

```sh
git clone <repository-url>
cd JSON_Lib
mkdir build && cd build
cmake ..
cmake --build .
```

### CMake options

| Option | Default | Description |
|---|---|---|
| `BUILD_TESTING` | `ON` | Build unit tests |
| `BUILD_EXAMPLES` | `OFF` | Build example programs |
| `JSON_LIB_ENABLE_LTO` | `ON` | Enable link-time optimization |
| `JSON_LIB_EXPORT_INTERFACE` | `ON` | Install only public headers by default |
| `JSON_LIB_OPTIMIZATION_LEVEL` | `O2` | Compiler optimization (`O0`–`Ofast`) |
| `JSON_LIB_EMBEDDED` | `OFF` | Enable embedded-friendly build preset; disables tests/examples and forces no-stdio |
| `JSON_LIB_NO_EXCEPTIONS` | `OFF` | Disable C++ exceptions; disables tests/examples in non-embedded builds |
| `JSON_LIB_NO_HEAP` | `OFF` | Disable heap allocation; implies `JSON_LIB_NO_DYNAMIC_MEMORY` |
| `JSON_LIB_NO_DYNAMIC_MEMORY` | `OFF` | Disable dynamic memory use; implies `JSON_LIB_NO_HEAP` |
| `JSON_LIB_NO_STDIO` | `OFF` | Disable file / stdio support |
| `JSON_LIB_MAX_PARSER_DEPTH` | `0` | Override max parse depth (0 = library default of 128) |
| `JSON_LIB_MAX_STRING_LENGTH` | `0` | Override max string length in bytes (0 = library default of 16384) |
| `JSON_LIB_DOWNLOAD_JSON_TEST_SUITE` | `OFF` | Download and run official JSONTestSuite (`https://github.com/nst/JSONTestSuite.git`) |

> `JSON_LIB_ENABLE_LTO` is only applied when the build uses an optimization level other than `O0`. If `O0` is selected, CMake will disable LTO and keep the build faster for debug-style use.

To run the benchmark suite:

```sh
cmake --build . --target JSON_Lib_Benchmarks
./tests/JSON_Lib_Benchmarks
```

### Running tests

```sh
cmake -DBUILD_TESTING=ON -DBUILD_EXAMPLES=OFF ..
cmake --build . --target JSON_Lib_Unit_Tests
ctest --output-on-failure
```

### Running the Official JSONTestSuite

The official [nst/JSONTestSuite](https://github.com/nst/JSONTestSuite.git) is integrated with the unit test suite.
To keep the default build lightweight and offline-friendly, these tests are **not included if not downloaded**.

To download and enable the test suite:

```sh
# Option 1: Automatic download via CMake FetchContent
cmake -DBUILD_TESTING=ON -DJSON_LIB_DOWNLOAD_JSON_TEST_SUITE=ON ..
cmake --build . --target JSON_Lib_Unit_Tests

# Option 2: Pre-download via script into tests/JSONTestSuite
./scripts/download_json_test_suite.sh
cmake -DBUILD_TESTING=ON ..
cmake --build . --target JSON_Lib_Unit_Tests

# Option 3: Point to an existing JSONTestSuite directory
cmake -DBUILD_TESTING=ON -DJSON_TEST_SUITE_PATH=/path/to/JSONTestSuite/test_parsing ..
cmake --build . --target JSON_Lib_Unit_Tests
```

When included, each test file is evaluated:
- `y_*`: Valid documents must be accepted; asserts on parsing failure.
- `n_*`: Invalid documents must be rejected; asserts on erroneous acceptance.
- `i_*`: Implementation-defined documents must complete without crashing.

### Embedded and Constrained Builds

For constrained or exception-free policy builds:

```sh
mkdir build-embedded && cd build-embedded
cmake -DJSON_LIB_NO_EXCEPTIONS=ON \
      -DJSON_LIB_NO_STDIO=ON \
      -DJSON_LIB_NO_DYNAMIC_MEMORY=ON \
      -DJSON_LIB_MAX_PARSER_DEPTH=5 \
      -DJSON_LIB_MAX_STRING_LENGTH=2048 \
      -DBUILD_EXAMPLES=OFF ..
cmake --build .
```

> Note: `JSON_LIB_NO_EXCEPTIONS=ON` disables `BUILD_TESTING` and `BUILD_EXAMPLES` in non-embedded builds, so unit test targets are not compiled in this mode.

Alternatively, use the `JSON_LIB_EMBEDDED` preset for deployment builds:

```sh
cmake -DJSON_LIB_EMBEDDED=ON ..
```

This preset automatically forces:
- `BUILD_TESTING=OFF`
- `BUILD_EXAMPLES=OFF`
- `JSON_LIB_ENABLE_LTO=OFF`
- `JSON_LIB_NO_STDIO=ON`

### Link in your project

```cmake
target_link_libraries(<your-target> PRIVATE JSON_Lib)
```

---

## Basic Usage

### Custom backend injection

```cpp
#include "JSON_Lib.hpp"

namespace js = JSON_Lib;

struct MyCustomParser final : public js::IParser {
  js::Node parse(js::ISource &source) override {
    // Custom parse logic goes here.
    return js::Node(js::JSON::ObjectInitializer{{"custom", true}});
  }
};

js::JSON::Options options;
options.setStringify(js::makeStringify<js::XML_Stringify>())
       .setParser(std::make_unique<MyCustomParser>())
       .setTranslator(std::make_unique<js::Default_Translator>());
js::JSON json(std::move(options));
```

### Basic parsing and serialization

```cpp
#include "JSON_Lib.hpp"

namespace js = JSON_Lib;

// Parse
js::JSON json;
json.parse(js::BufferSource{R"({"name":"Alice","age":30})"});

// Access
std::string name = js::NRef<js::String>(json["name"]).value();
int age          = js::NRef<js::Number>(json["age"]).value<int>();

// Serialize
js::BufferDestination dest;
json.stringify(dest);
std::cout << dest.toString() << "\n";
```

### File I/O

```cpp
json.parse(js::FileSource{"input.json"});
json.stringify(js::FileDestination{"output.json"});
```

### Create JSON at runtime

```cpp
js::JSON json(js::JSON::ObjectInitializer{
    {"name",  "Bob"},
    {"score", 99},
    {"pass",  true},
});
js::BufferDestination dest;
json.stringify(dest);
```

---

## Embedded Usage

Use the `EmbeddedJSON` façade for targets where heap, exceptions, or stdio may be unavailable.

```cpp
#include "JSON_Lib.hpp"

namespace js = JSON_Lib;

js::EmbeddedJSON embedded;

// Exception-free parse
auto result = embedded.parseNoThrow(js::BufferSource{R"({"x":1})"});
if (!result.ok()) { /* handle result.message */ }

// Stack-allocated output — no heap required
js::FixedBufferDestination<256> dest;
auto sr = embedded.stringifyNoThrow(dest);
if (sr.ok()) { puts(dest.data()); }

// Compile-time policy queries
static_assert(!js::EmbeddedJSON::isEmbeddedBuild() || !js::EmbeddedJSON::isNoStdIoBuild());
```

Build with embedded preset:

```sh
cmake -DJSON_LIB_EMBEDDED=ON ..
```

Or selectively:

```sh
cmake -DJSON_LIB_NO_EXCEPTIONS=ON -DJSON_LIB_NO_STDIO=ON -DJSON_LIB_MAX_PARSER_DEPTH=5 ..
```

---

## Documentation & Examples

- **API Reference:** [docs/api.md](docs/api.md)
- **User Guide:** [docs/guide.md](docs/guide.md)
- **Integration & Packaging:** [docs/packaging.md](docs/packaging.md)
- **Standards Conformance:** [docs/conformance.md](docs/conformance.md)
- **Architecture Overview:** [docs/architecture.md](docs/architecture.md)
- **SOLID Design & Architecture:** [docs/solid_architecture.md](docs/solid_architecture.md)
- **Changelog:** [CHANGELOG.md](CHANGELOG.md)
- **Security Policy:** [SECURITY.md](SECURITY.md)
- **Examples:** `examples/source/` — file conversion, schema validation, embedded API, custom serialization, streaming, and more.

---

## Compliance & Standards

JSON_Lib complies with RFC 8259 and ECMA-404 and is tested against the official [nst/JSONTestSuite](https://github.com/nst/JSONTestSuite.git). For a detailed compliance matrix and description of safety-oriented dialect choices (e.g. duplicate key rejection), see [docs/conformance.md](docs/conformance.md).

---

## Thread Safety

`JSON_Lib` does **not** provide internal synchronisation. Follow these rules when using the library from multiple threads:

| Rule | Rationale |
|---|---|
| Create one `JSON` instance **per thread**. | `JSON` is non-copyable and non-movable; sharing a single instance between threads without external locking causes data races. |
| Do not modify `Default_Parser::maxParserDepth` or `String::maxStringLength` after startup. | These are `inline static` values shared across all instances and all threads. Concurrent writes are a data race. |
| Read-only access to a `JSON` object from multiple threads is safe **only** if no thread is simultaneously writing. | Standard read-sharing rules apply. |

> **Recommendation for multi-threaded code:** Construct one `JSON` instance per thread (or per task on an RTOS). Set any global limits once at program startup, before spawning threads.

---

## Contributing

Contributions are welcome. Please:
- Follow the existing code style
- Add test cases for new features
- Open an issue before submitting large changes

---

## License

MIT License — see [LICENSE.txt](LICENSE.txt).


