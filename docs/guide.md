# JSON_Lib User Guide

## Table of Contents

- [Getting started](#getting-started)
- [Parsing JSON](#parsing-json)
- [Creating JSON](#creating-json)
- [Accessing and modifying nodes](#accessing-and-modifying-nodes)
- [Serializing JSON](#serializing-json)
- [File I/O](#file-io)
- [Error handling](#error-handling)
- [Embedded and resource-constrained builds](#embedded-and-resource-constrained-builds)
- [Custom struct serialization](#custom-struct-serialization)
- [Traversal and custom actions](#traversal-and-custom-actions)
- [Stringify backends (Bencode, XML, YAML)](#stringify-backends)
- [CMake build options reference](#cmake-build-options-reference)
- [Examples directory](#examples-directory)
- [Troubleshooting](#troubleshooting)

---

## Getting started

### Requirements

- C++ compiler with C++23 support (GCC 13+, Clang 16+, MSVC 19.36+)
- CMake 3.21+

### Build and install

```sh
git clone <repository-url>
cd JSON_Lib
mkdir build && cd build
cmake ..
cmake --build .
```

### Link against your project

```cmake
add_subdirectory(JSON_Lib)          # or use find_package after installing
target_link_libraries(my_app PRIVATE JSON_Lib)
target_include_directories(my_app PRIVATE
    ${JSON_Lib_SOURCE_DIR}/classes/include
    ${JSON_Lib_BINARY_DIR}          # for JSON_Config.hpp
)
```

---

## Parsing JSON

```cpp
#include "JSON.hpp"
#include "implementation/io/JSON_Sources.hpp"

namespace js = JSON_Lib;

// From a string in memory
js::JSON json;
json.parse(js::BufferSource{R"({"name":"Alice","score":100})"});

// From a file
json.parse(js::FileSource{"data.json"});

// From a raw byte region (zero-heap; suitable for ROM buffers)
static constexpr char kData[] = R"({"x":1})";
json.parse(js::FixedBufferSource{kData, sizeof(kData) - 1});
```

---

## Creating JSON

```cpp
// Object literal
js::JSON obj(js::JSON::ObjectInitializer{
    {"name",  "Bob"},
    {"age",   42},
    {"admin", false},
});

// Array literal
js::JSON arr(js::JSON::ArrayInitializer{1, 2.5, "three", true});

// Assign fields after construction
js::JSON json;
json.parse(js::BufferSource{"{}"}); // start with empty object
json["count"] = 7;
```

---

## Accessing and modifying nodes

```cpp
// String value
std::string name = js::NRef<js::String>(json["name"]).value();

// Numeric value — template parameter is the C++ type to convert to
int   score  = js::NRef<js::Number>(json["score"]).value<int>();
float weight = js::NRef<js::Number>(json["weight"]).value<float>();

// Boolean
bool admin = js::NRef<js::Boolean>(json["admin"]).value();

// Array element
int first = js::NRef<js::Number>(json[0]).value<int>();

// Nested access
std::string city = js::NRef<js::String>(json["address"]["city"]).value();
```

---

## Serializing JSON

```cpp
#include "implementation/io/JSON_Destinations.hpp"

// Compact JSON into a heap string
js::BufferDestination buf;
json.stringify(buf);
std::cout << buf.toString() << "\n";

// Pretty-printed
js::JSON::setIndent(2);
json.print(buf);

// Stack-allocated output (no heap; fixed capacity)
js::FixedBufferDestination<512> fixed;
json.stringify(fixed);
std::cout.write(fixed.data(), static_cast<std::streamsize>(fixed.size()));

// To a file
json.stringify(js::FileDestination{"output.json"});
```

---

## File I/O

```cpp
// Read raw text
std::string text = js::JSON::fromFile("input.json");

// Write with explicit encoding
js::JSON::toFile("out_utf16.json", text, js::JSON::Format::utf16LE);

// Detect encoding
js::JSON::Format fmt = js::JSON::getFileFormat("input.json");
```

File helpers are compiled out when `JSON_LIB_NO_STDIO == 1`.

---

## Error handling

### Exception-based (default)

```cpp
try {
    js::JSON json;
    json.parse(js::FileSource{"missing.json"});
} catch (const std::exception &ex) {
    std::cerr << "Parse error: " << ex.what() << "\n";
}
```

### Exception-free — `Result<T>`

Use `EmbeddedJSON` or the `...Result` overloads on `JSON` when exceptions are disabled or undesirable:

```cpp
js::EmbeddedJSON embedded;
auto result = embedded.parseNoThrow(js::BufferSource{jsonText});
if (!result.ok()) {
    std::fprintf(stderr, "error at line %ld col %ld: %s\n",
        result.position.first,
        result.position.second,
        result.message.c_str());
    return;
}
// result.value is a std::unique_ptr<Node>
```

`Result<void>` is returned by `stringifyNoThrow`, `printNoThrow`, and `traverseNoThrow`:

```cpp
auto sr = embedded.stringifyNoThrow(js::FixedBufferDestination<256>{});
if (!sr.ok()) { /* sr.message */ }
```

---

## Embedded and resource-constrained builds

### Build presets

Enable the all-in-one preset for a minimal embedded build:

```sh
cmake -DJSON_LIB_EMBEDDED=ON ..
```

This automatically applies: `BUILD_TESTING=OFF`, `BUILD_EXAMPLES=OFF`, `JSON_LIB_ENABLE_LTO=OFF`, `JSON_LIB_NO_STDIO=ON`.

Or pick individual flags:

```sh
cmake \
  -DJSON_LIB_NO_EXCEPTIONS=ON \
  -DJSON_LIB_NO_HEAP=ON \
  -DJSON_LIB_NO_STDIO=ON \
  -DJSON_LIB_MAX_PARSER_DEPTH=5 \
  -DJSON_LIB_MAX_STRING_LENGTH=2048 \
  ..
```

### Using EmbeddedJSON

```cpp
#include "JSON.hpp"
#include "implementation/io/JSON_Sources.hpp"
#include "implementation/io/JSON_Destinations.hpp"

namespace js = JSON_Lib;

// Inspect build policy at compile time
static_assert(!js::EmbeddedJSON::isNoDynamicMemoryBuild(),
              "Heap is required by this component");

// Parse from a raw pointer (no std::string allocation)
static constexpr char kConfig[] = R"({"baud":9600,"parity":false})";
js::EmbeddedJSON embedded;
auto r = embedded.parseNoThrow(js::FixedBufferSource{kConfig, sizeof(kConfig)-1});
if (!r.ok()) { handle_error(r.message); return; }

// Serialize into a stack buffer
js::FixedBufferDestination<128> out;
auto sr = embedded.stringifyNoThrow(out);
if (sr.ok()) {
    transmit(out.data(), out.size());
}

// Check for overflow when JSON_LIB_NO_EXCEPTIONS is active
#if JSON_LIB_NO_EXCEPTIONS
if (out.overflowed()) { handle_overflow(); }
#endif
```

### Compile-time resource limits

| CMake variable | Macro | Default |
|---|---|---|
| `JSON_LIB_MAX_PARSER_DEPTH` | `JSON_LIB_MAX_PARSER_DEPTH` | 10 |
| `JSON_LIB_MAX_STRING_LENGTH` | `JSON_LIB_MAX_STRING_LENGTH` | 16384 |

Access at runtime:

```cpp
unsigned long depth = js::EmbeddedJSON::Limits::maxParserDepth();
uint64_t      len   = js::EmbeddedJSON::Limits::maxStringLength();
```

---

## Custom struct serialization

```cpp
struct Sensor {
    std::string id;
    double      value;
    bool        active;
};

// Serialize
Sensor s{"temp01", 23.5, true};
js::JSON json(js::JSON::ObjectInitializer{
    {"id",     s.id},
    {"value",  s.value},
    {"active", s.active},
});
js::BufferDestination dest;
json.stringify(dest);

// Deserialize
js::JSON in;
in.parse(js::BufferSource{dest.toString()});
Sensor s2;
s2.id     = js::NRef<js::String> (in["id"]).value();
s2.value  = js::NRef<js::Number> (in["value"]).value<double>();
s2.active = js::NRef<js::Boolean>(in["active"]).value();
```

See `examples/source/JSON_Custom_Struct_Serialization.cpp` for a fuller pattern.

---

## Traversal and custom actions

Implement `IAction` to walk the tree without constructing intermediate strings:

```cpp
#include "interface/IAction.hpp"

struct Counter : js::IAction {
    int strings = 0;
    void onString(const js::String &) override { ++strings; }
};

Counter c;
json.traverse(c);
std::cout << c.strings << " string values\n";
```

Exception-free traversal:

```cpp
auto result = embedded.traverseNoThrow(c);
if (!result.ok()) { ... }
```

Ready-made action implementations: `examples/include/JSON_Analyzer.hpp`, `JSON_Convert.hpp`, `JSON_Indexer.hpp`.

---

## Stringify backends

Link or include the desired backend and pass it to the `JSON` constructor:

```cpp
#include "implementation/stringify/XML_Stringify.hpp"

js::XML_Stringify xmlStringify;
js::JSON json(&xmlStringify);
json.parse(js::FileSource{"data.json"});
js::BufferDestination dest;
json.stringify(dest);
std::cout << dest.toString();
```

| Backend header | Format |
|---|---|
| `Default_Stringify.hpp` | Compact JSON |
| `Bencode_Stringify.hpp` | Bencode |
| `XML_Stringify.hpp` | XML |
| `YAML_Stringify.hpp` | YAML |

Example programs: `JSON_Files_To_Bencode.cpp`, `JSON_Files_To_XML.cpp`, `JSON_Files_To_YAML.cpp`.

---

## CMake build options reference

| Option | Default | Effect |
|---|---|---|
| `BUILD_TESTING` | `ON` | Compile unit tests (Catch2, fetched automatically) |
| `BUILD_EXAMPLES` | `ON` | Compile example programs |
| `JSON_LIB_ENABLE_LTO` | `ON` | Enable link-time optimization on `JSON_Lib` |
| `JSON_LIB_OPTIMIZATION_LEVEL` | `O2` | Compiler optimization flag (`O0`–`Ofast`) |
| `JSON_LIB_EMBEDDED` | `OFF` | Preset: embedded-friendly build |
| `JSON_LIB_NO_EXCEPTIONS` | `OFF` | Compile with `-fno-exceptions`; disables tests/examples |
| `JSON_LIB_NO_HEAP` | `OFF` | Disable heap use; implies `NO_DYNAMIC_MEMORY` |
| `JSON_LIB_NO_DYNAMIC_MEMORY` | `OFF` | Disable dynamic memory; implies `NO_HEAP` |
| `JSON_LIB_NO_STDIO` | `OFF` | Disable `FileSource`/`FileDestination` |
| `JSON_LIB_ALLOCATOR_INTERFACE` | `OFF` | Expose custom allocator hook |
| `JSON_LIB_MAX_PARSER_DEPTH` | `0` | Override parse depth limit (0 = use default 10) |
| `JSON_LIB_MAX_STRING_LENGTH` | `0` | Override string length limit (0 = use default 16384) |

### CMake build targets

| Target | Type | Description |
|---|---|---|
| `JSON_Lib` | STATIC | Public library target to link against |
| `JSON_Lib_Core` | OBJECT | Core sources (JSON_Impl, converters, file helpers) |
| `JSON_Lib_Parser` | OBJECT | Default parser |
| `JSON_Lib_Stringify` | INTERFACE | Stringify headers and embedded flag propagation |
| `JSON_Lib_Embedded` | INTERFACE | Compile definitions for embedded flags |

---

## Examples directory

| File | Description |
|---|---|
| `JSON_Parse_File.cpp` | Parse and re-stringify a JSON file |
| `JSON_Pretty_Print.cpp` | Pretty-print JSON with configurable indent |
| `JSON_Strip_Whitespace.cpp` | Strip whitespace from JSON |
| `JSON_Create_At_Runtime.cpp` | Build a JSON tree programmatically |
| `JSON_Custom_Struct_Serialization.cpp` | Serialize/deserialize a C++ struct |
| `JSON_Analyze_File.cpp` | Walk a JSON tree with a custom `IAction` |
| `JSON_Index_File.cpp` | Index JSON keys with a custom action |
| `JSON_Deep_Search.cpp` | Recursive value search |
| `JSON_Files_To_Bencode.cpp` | Convert JSON → Bencode |
| `JSON_Files_To_XML.cpp` | Convert JSON → XML |
| `JSON_Files_To_YAML.cpp` | Convert JSON → YAML |
| `JSON_Embedded_Buffer_API.cpp` | `EmbeddedJSON` with buffer sources and `FixedBufferDestination` |
| `JSON_Error_Handling.cpp` | Exception and `Result<T>` error patterns |
| `JSON_Schema_Validation.cpp` | Load and inspect a JSON schema |
| `JSON_Stream_Large_File.cpp` | Streaming pattern for large files |
| `JSON_HTTP_Integration.cpp` | Parse JSON from an HTTP response string |
| `JSON_Unicode_Support.cpp` | Multi-encoding source handling |
| `JSON_Patch_Merge.cpp` | JSON merge-patch pattern |
| `JSON_Fibonacci.cpp` | Recursive JSON structure generation |
| `JSON_Config_Loader.cpp` | Read configuration from a JSON file |
| `JSON_Display_Settings.cpp` | Display and toggle boolean settings |

---

## Troubleshooting

| Symptom | Likely cause | Fix |
|---|---|---|
| `file not found` on `JSON_Interfaces.hpp` | Missing impl include dirs | Add `${JSON_IMPL_INCLUDE_DIRS}` as PRIVATE include to your target |
| PCH error `Unable to resolve full path` | `REUSE_FROM JSON_Lib` on examples | Use `REUSE_FROM JSON_Lib_Core` (the static wrapper has no sources) |
| Link error on `FileSource`/`FileDestination` | `JSON_LIB_NO_STDIO=ON` | Disable the flag, or use `BufferSource`/`BufferDestination` |
| `FixedBufferDestination` not found | Wrong destination header | Include `implementation/io/JSON_Destinations.hpp` |
| Overflow with `FixedBufferDestination` | Buffer too small | Increase `N`, or check `overflowed()` and resize |
| Large integers parsed incorrectly | C++ type narrowing | Use `value<long long>()` or `value<double>()` explicitly |

---

For API details see [api.md](api.md).
