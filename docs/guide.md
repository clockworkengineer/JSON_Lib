# JSON_Lib User Guide

## Table of Contents

- [Getting started](#getting-started)
- [Parsing JSON](#parsing-json)
- [Creating JSON](#creating-json)
- [Accessing and modifying nodes](#accessing-and-modifying-nodes)
- [Serializing JSON](#serializing-json)
- [File I/O](#file-io)
- [Error handling](#error-handling)
- [Exception-free Result\<T\> workflow](#exception-free-resultt-workflow)
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

### Portability and supported toolchains

JSON_Lib is written for standard C++23 and is compatible with the primary vendor toolchains:
- GCC 11+
- Clang 14+
- MSVC 19.36+

The implementation exposes platform and compiler detection macros via `JSON_Attributes.hpp`:
`JSON_LIB_COMPILER_MSVC`, `JSON_LIB_COMPILER_CLANG`, `JSON_LIB_COMPILER_GCC`, `JSON_LIB_COMPILER_UNKNOWN`,
`JSON_LIB_PLATFORM_WINDOWS`, `JSON_LIB_PLATFORM_UNIX`, and `JSON_LIB_PLATFORM_POSIX`.

CMake enforces these minimum supported versions at configure time. If you build with an untested compiler, CMake will emit a warning and the compatibility macros will still be defined, but the library may not be fully validated.

To verify a different compiler with CMake:

```sh
mkdir build-clang && cd build-clang
cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_STANDARD=23 ..
cmake --build .
```

### Build and install

```sh
git clone <repository-url>
cd JSON_Lib
mkdir build && cd build
cmake ..
cmake --build .
```

### Public headers and internal boundaries

Use the public façade header whenever possible:

```cpp
#include "JSON_Lib.hpp"
```

`JSON_Lib.hpp` exposes the main API surface including `JSON`, `EmbeddedJSON`, and the common `Node` types. For code that only needs sources and destinations, prefer:

```cpp
#include "JSON_IO.hpp"
```

Avoid direct inclusion of `implementation/*` headers in application code. The public headers exposed by `JSON_Lib.hpp`, `JSON.hpp`, and `JSON_IO.hpp` are the stable API surface, while implementation headers are only installed to satisfy those public headers.

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
#include "JSON_Lib.hpp"

namespace js = JSON_Lib;

// From a string in memory
json.parse(R"({"name":"Alice","score":100})");

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

// Direct node construction
js::Node root(js::JSON::ObjectInitializer{
    {"name", "Charlie"},
    {"active", true},
});
json.root() = std::move(root);
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

// Safe key checks
if (json.contains("name")) {
    std::string name = js::NRef<js::String>(json.at("name")).value();
}

// Nested access
std::string city = js::NRef<js::String>(json["address"]["city"]).value();
```

---

## Serializing JSON

```cpp
#include "JSON_Lib.hpp"

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

## Custom parser and stringify injection

`JSON_Lib` supports custom parsing and serialization implementations via `IParser` and `IStringify`.

```cpp
#include "JSON_Lib.hpp"

namespace js = JSON_Lib;

struct MyParser : js::IParser {
    js::Node parse(js::ISource &source) override {
        // Implement your own parse strategy or delegate to an existing parser.
        throw js::IParser::Error("custom parser not implemented");
    }
};

struct MyStringify : js::IStringify {
    void stringify(const js::Node &jNode, js::IDestination &destination, unsigned long indent) const override {
        // Emit custom output for the JSON node tree.
        (void)jNode;
        (void)destination;
        (void)indent;
    }
};

namespace js = JSON_Lib;

js::JSON json(js::makeStringify<MyStringify>(), std::make_unique<MyParser>());
json.parse(js::BufferSource{R"({"hello":"world"})"});
js::BufferDestination out;
json.stringify(out);
```

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
if (!sr.ok()) { std::fprintf(stderr, "error: %s\n", sr.message.c_str()); }
```

To serialize a ready JSON tree into a string buffer:

```cpp
std::string compact = json.stringifyToString();
std::cout << compact << "\n";
```

---

## Exception-free Result\<T\> workflow

Every throwing method on `JSON` and `EmbeddedJSON` has a `...Result` counterpart that returns a `Result<T>` instead of throwing.
This lets you use JSON_Lib safely on targets where C++ exceptions are disabled (`-fno-exceptions`) or simply undesirable.

### `Result<T>` structure

```cpp
template<typename T>
struct Result {
    Status                status;    // Status::Ok on success
    std::unique_ptr<T>    value;     // valid only when ok()
    std::string           message;   // human-readable error description
    std::pair<long,long>  position;  // {lineNo, column} where the error occurred

    bool ok()      const noexcept;   // true iff status == Status::Ok
    T   &unwrap();                   // dereferences value (UB if !ok())
};

template<>
struct Result<void> {
    Status                status;
    std::string           message;
    std::pair<long,long>  position;

    bool ok() const noexcept;
};
```

`Status` values: `Ok`, `SyntaxError`, `OutOfMemory`, `InvalidKey`, `InvalidIndex`,
`UnsupportedEncoding`, `InvalidInput`, `NoData`, `UnknownError`.

### Parse — full workflow

```cpp
#include "JSON_Lib.hpp"

namespace js = JSON_Lib;

js::JSON json;
auto result = json.parseResult(js::BufferSource{jsonText});

// 1. Check success
if (!result.ok()) {
    std::fprintf(stderr, "parse error at line %ld col %ld: %s\n",
        result.position.first,
        result.position.second,
        result.message.c_str());
    return false;
}

// 2. Use the tree — result.value is a std::unique_ptr<Node>
//    (for parseResult the Node has already been stored in json.root();
//     result.value is unused for this overload)
std::string name = js::NRef<js::String>(json["name"]).value();
```

### Stringify / print

```cpp
js::BufferDestination dest;
auto sr = json.stringifyResult(dest);
if (!sr.ok()) {
    // sr.status, sr.message, sr.position available
}

auto pr = json.printResult(dest);
if (!pr.ok()) { /* ... */ }
```

### Traverse

```cpp
Counter counter;
auto tr = json.traverseResult(counter);
if (!tr.ok()) { /* tr.message */ }
```

### Switching on `Status`

```cpp
switch (result.status) {
    case js::Status::Ok:               /* success */ break;
    case js::Status::SyntaxError:      /* bad JSON */ break;
    case js::Status::InvalidKey:       /* key not found */ break;
    case js::Status::InvalidIndex:     /* array out of bounds */ break;
    case js::Status::UnsupportedEncoding: /* unknown BOM/encoding */ break;
    default:                           /* other */ break;
}
```

### Using `EmbeddedJSON` exception-free variants

`EmbeddedJSON` exposes `parseNoThrow`, `stringifyNoThrow`, `printNoThrow`, and `traverseNoThrow`
which are identical in behaviour to the `...Result` overloads but carry the `NoThrow` name
convention that embedded code-review tools recognise:

```cpp
js::EmbeddedJSON embedded;
js::FixedBufferDestination<256> out;

auto r = embedded.parseNoThrow(js::FixedBufferSource{kConfig, sizeof(kConfig)-1});
if (!r.ok()) { handle_error(r.message); return; }

auto sr = embedded.stringifyNoThrow(out);
if (!sr.ok()) { handle_error(sr.message); return; }
```

> **Tip:** Prefer `...Result` / `NoThrow` overloads in all new code that needs to run without
> exceptions, especially on RTOS targets where stack-unwinding tables may be stripped.

## Security and strict parse semantics

JSON_Lib treats untrusted input conservatively and makes its safety boundaries explicit:
- Raw control characters inside JSON strings are rejected.
- Invalid UTF-8 sequences in string values are rejected.
- Incomplete or unrecognized BOM sequences in JSON files are treated as unsupported encoding.
- Only a single JSON value is accepted; any trailing characters after the root value result in a syntax error.
- Nested objects and arrays are capped by `JSON_LIB_MAX_PARSER_DEPTH`.
- String content is capped by `JSON_LIB_MAX_STRING_LENGTH`.
- `JSON_LIB_NO_HEAP=ON` implies `JSON_LIB_NO_DYNAMIC_MEMORY=ON` and enforces heapless operation where supported.
- `JSON_LIB_NO_STDIO=ON` compiles out file-based source and destination helpers, leaving buffer-based I/O only.
- Use `parseResult` / `parseNoThrow` to observe parse failures through `Status::SyntaxError`, `Status::UnsupportedEncoding`, or `Status::InvalidInput`.

Use build-time overrides to harden parser behavior in constrained or security-sensitive deployments.


### Build presets

Enable the all-in-one preset for a minimal embedded build:

```sh
cmake -DJSON_LIB_EMBEDDED=ON ..
```

This automatically applies: `BUILD_TESTING=OFF`, `BUILD_EXAMPLES=OFF`, `JSON_LIB_ENABLE_LTO=OFF`, `JSON_LIB_NO_STDIO=ON`.

The embedded preset is the recommended starting point for constrained deployments. It is designed to disable file I/O and reduce runtime requirements while preserving the same public `JSON`/`EmbeddedJSON` API.

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

### Test build matrix

The library supports several verification modes. Use `BUILD_TESTING=ON` to enable unit tests.

- `JSON_LIB_NO_EXCEPTIONS=ON` disables exceptions and disables tests/examples unless `JSON_LIB_EMBEDDED` already manages those flags.
- `JSON_LIB_NO_STDIO=ON` compiles out file-based sources and destinations, leaving only buffer-based I/O.
- `JSON_LIB_NO_HEAP=ON` implies `JSON_LIB_NO_DYNAMIC_MEMORY=ON` and restricts the library to heapless operation where supported.


| Build variant | CMake flags | Notes |
|---|---|---|
| Default tests | `-DBUILD_TESTING=ON` | Standard unit test build | 
| Embedded preset + tests | `-DJSON_LIB_EMBEDDED=ON -DBUILD_TESTING=ON -DBUILD_EXAMPLES=OFF` | Embedded policies enabled, file I/O disabled | 
| No exceptions | `-DJSON_LIB_NO_EXCEPTIONS=ON -DBUILD_TESTING=ON` | Exercise `parseResult` / `stringifyResult` and `NoThrow` APIs |
| No stdio | `-DJSON_LIB_NO_STDIO=ON -DBUILD_TESTING=ON` | File I/O sources/destinations are disabled |
| No dynamic memory | `-DJSON_LIB_NO_DYNAMIC_MEMORY=ON -DBUILD_TESTING=ON` | Enforces embedded memory policy |

Run tests from the build directory:

```sh
cmake -S . -B build -DBUILD_TESTING=ON -DBUILD_EXAMPLES=OFF
cmake --build build --target JSON_Lib_Unit_Tests
ctest --test-dir build --output-on-failure
```

For an embedded-style tests build:

```sh
cmake -S . -B build-embedded -DJSON_LIB_EMBEDDED=ON -DBUILD_TESTING=ON -DBUILD_EXAMPLES=OFF -DJSON_LIB_ENABLE_LTO=OFF
cmake --build build-embedded --target JSON_Lib_Unit_Tests
ctest --test-dir build-embedded --output-on-failure
```

### Using EmbeddedJSON

```cpp
#include "JSON_Lib.hpp"

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
| `JSON_LIB_MAX_PARSER_DEPTH` | `JSON_LIB_MAX_PARSER_DEPTH` | 128 |
| `JSON_LIB_MAX_STRING_LENGTH` | `JSON_LIB_MAX_STRING_LENGTH` | 16384 |

Access at runtime:

```cpp
unsigned long depth = js::EmbeddedJSON::Limits::maxParserDepth();
uint64_t      len   = js::EmbeddedJSON::Limits::maxStringLength();
```

These values are also available as compile-time constants, which is useful for static configuration checks and buffer sizing in embedded builds:

```cpp
static_assert(js::EmbeddedJSON::Limits::kMaxParserDepth > 0);
static_assert(js::EmbeddedJSON::Limits::kMaxStringLength > 0);
```

Use `EmbeddedJSON::Limits` whenever your embedded application needs to adapt to the configured parser depth or string-size bounds.

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
| `BUILD_EXAMPLES` | `OFF` | Compile example programs |
| `JSON_LIB_ENABLE_LTO` | `ON` | Enable link-time optimization on `JSON_Lib` |
| `JSON_LIB_EXPORT_INTERFACE` | `ON` | Install only public headers by default |
| `JSON_LIB_OPTIMIZATION_LEVEL` | `O2` | Compiler optimization flag (`O0`–`Ofast`) |
| `JSON_LIB_EMBEDDED` | `OFF` | Preset: embedded-friendly build |
| `JSON_LIB_NO_EXCEPTIONS` | `OFF` | Compile with `-fno-exceptions`; disables tests/examples |
| `JSON_LIB_NO_HEAP` | `OFF` | Disable heap use; implies `NO_DYNAMIC_MEMORY` |
| `JSON_LIB_NO_DYNAMIC_MEMORY` | `OFF` | Disable dynamic memory; implies `NO_HEAP` |
| `JSON_LIB_NO_STDIO` | `OFF` | Disable `FileSource`/`FileDestination` |
| `JSON_LIB_MAX_PARSER_DEPTH` | `0` | Override parse depth limit (0 = use default 128) |
| `JSON_LIB_MAX_STRING_LENGTH` | `0` | Override string length limit (0 = use default 16384) |

> Note: `JSON_LIB_ENABLE_LTO` is enabled by default but is automatically disabled when `JSON_LIB_OPTIMIZATION_LEVEL` is set to `O0`.

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
| `FixedBufferDestination` not found | Wrong destination header | Include `JSON_IO.hpp` |
| Overflow with `FixedBufferDestination` | Buffer too small | Increase `N`, or check `overflowed()` and resize |
| Large integers parsed incorrectly | C++ type narrowing | Use `value<long long>()` or `value<double>()` explicitly |

---

For API details see [api.md](api.md).
