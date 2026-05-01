# JSON_Lib

**JSON_Lib** is a modern, high-performance C++23 library for parsing, manipulating, and serializing JSON data. It provides a clean, intuitive API and is designed for efficiency, extensibility, standards compliance, and embedded-target support.

---

## Features

- C++23 with full `std::string_view`, concepts, and structured-binding support
- Parse, create, traverse, and serialize JSON objects and arrays
- Exception-based error handling **and** an exception-free `Result<T>` API for embedded/RTOS targets
- Unicode and multi-encoding support (UTF-8, UTF-8 BOM, UTF-16 LE/BE, UTF-32 LE/BE)
- Modular CMake build — library, tests, and examples are independent targets
- Embedded-friendly build mode: no heap, no stdio, no exceptions, configurable resource limits
- Pluggable stringify backends: compact JSON, pretty-print, Bencode, XML, YAML
- Extensive examples and RFC 8259 / ECMA-404 compliance documentation

---

## Getting Started

### Prerequisites

- C++ compiler with C++23 support (GCC 13+, Clang 16+, MSVC 19.36+)
- CMake 3.21 or later

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
| `BUILD_EXAMPLES` | `ON` | Build example programs |
| `JSON_LIB_ENABLE_LTO` | `ON` | Enable link-time optimization |
| `JSON_LIB_OPTIMIZATION_LEVEL` | `O2` | Compiler optimization (`O0`–`Ofast`) |
| `JSON_LIB_EMBEDDED` | `OFF` | Enable embedded-friendly build preset |
| `JSON_LIB_NO_EXCEPTIONS` | `OFF` | Disable C++ exceptions (`-fno-exceptions`) |
| `JSON_LIB_NO_HEAP` | `OFF` | Disable heap allocation |
| `JSON_LIB_NO_DYNAMIC_MEMORY` | `OFF` | Disable dynamic memory use |
| `JSON_LIB_NO_STDIO` | `OFF` | Disable file / stdio support |
| `JSON_LIB_ALLOCATOR_INTERFACE` | `OFF` | Enable custom allocator hook |
| `JSON_LIB_MAX_PARSER_DEPTH` | `0` | Override max parse depth (0 = library default of 10) |
| `JSON_LIB_MAX_STRING_LENGTH` | `0` | Override max string length in bytes (0 = library default of 16384) |

Enabling `JSON_LIB_EMBEDDED` automatically sets `BUILD_TESTING=OFF`, `BUILD_EXAMPLES=OFF`, `JSON_LIB_ENABLE_LTO=OFF`, and `JSON_LIB_NO_STDIO=ON`.

### Link in your project

```cmake
target_link_libraries(<your-target> PRIVATE JSON_Lib)
```

---

## Basic Usage

```cpp
#include "JSON.hpp"
#include "implementation/io/JSON_Sources.hpp"
#include "implementation/io/JSON_Destinations.hpp"

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
#include "JSON.hpp"
#include "implementation/io/JSON_Sources.hpp"
#include "implementation/io/JSON_Destinations.hpp"

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
- **Compliance Report:** [docs/JSON_Lib_Compliance_Report.md](docs/JSON_Lib_Compliance_Report.md)
- **Examples:** `examples/source/` — file conversion, schema validation, embedded API, custom serialization, streaming, and more.

---

## Compliance & Deviations

JSON_Lib is largely compliant with RFC 8259/ECMA-404 with a few stricter behaviors:
- Strict BOM/file format detection (errors on unrecognized BOMs)
- CRLF normalized to LF during parsing
- C++ native number types (behavior for very large integers may differ from the standard)
- Strict Unicode error handling

See the compliance report for full details.

---

## Contributing

Contributions are welcome. Please:
- Follow the existing code style
- Add test cases for new features
- Open an issue before submitting large changes

---

## License

MIT License — see [LICENSE.txt](LICENSE.txt).