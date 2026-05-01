# JSON_Lib API Reference

## Table of Contents

- [Namespace and headers](#namespace-and-headers)
- [JSON class](#json-class)
- [EmbeddedJSON class](#embeddedjson-class)
- [Node and accessors](#node-and-accessors)
- [Sources (input)](#sources-input)
- [Destinations (output)](#destinations-output)
- [Result\<T\> — exception-free API](#resultt--exception-free-api)
- [Error handling](#error-handling)
- [Traversal — IAction](#traversal--iaction)
- [Stringify backends](#stringify-backends)
- [Build config macros](#build-config-macros)

---

## Namespace and headers

All public types live in `namespace JSON_Lib`.

```cpp
#include "JSON.hpp"                                   // JSON, EmbeddedJSON, Node
#include "implementation/io/JSON_Sources.hpp"         // BufferSource, FixedBufferSource, FileSource
#include "implementation/io/JSON_Destinations.hpp"    // BufferDestination, FixedBufferDestination, FileDestination
#include "JSON_Config.hpp"                            // generated compile-time macros (auto-included via JSON.hpp)
```

---

## JSON class

`JSON` is the primary façade for parsing, manipulating, and serializing JSON.

### Constructors

| Constructor | Description |
|---|---|
| `JSON(IStringify* = nullptr, IParser* = nullptr)` | Default; accepts optional custom stringify/parser |
| `JSON(std::string_view jsonString)` | Construct and parse a JSON string immediately |
| `JSON(ArrayInitializer)` | Construct a JSON array from an initializer list |
| `JSON(ObjectInitializer)` | Construct a JSON object from an initializer list |

Copy and move are deleted. `JSON` is non-copyable, non-movable.

### Static methods

```cpp
static std::string version();
static void setIndent(long indent);                            // indent for print()

// File helpers (disabled when JSON_LIB_NO_STDIO == 1)
static std::string fromFile(std::string_view fileName);
static void        toFile(std::string_view fileName,
                          std::string_view jsonString,
                          Format format = Format::utf8);
static Format      getFileFormat(std::string_view fileName);
```

### Parse

```cpp
void parse(ISource &source) const;
void parse(ISource &&source) const;
Result<Node> parseResult(ISource &source) const;   // exception-free
Result<Node> parseResult(ISource &&source) const;
```

### Stringify (compact)

```cpp
void stringify(IDestination &destination) const;
void stringify(IDestination &&destination) const;
Result<void> stringifyResult(IDestination &destination) const;
Result<void> stringifyResult(IDestination &&destination) const;
```

### Print (pretty)

```cpp
void print(IDestination &destination) const;
void print(IDestination &&destination) const;
Result<void> printResult(IDestination &destination) const;
Result<void> printResult(IDestination &&destination) const;
```

### Strip whitespace

```cpp
static void strip(ISource &source,  IDestination &destination);
static void strip(ISource &source,  IDestination &&destination);
static void strip(ISource &&source, IDestination &destination);
static void strip(ISource &&source, IDestination &&destination);
```

### Traverse

```cpp
void traverse(IAction &action);
void traverse(IAction &action) const;
Result<void> traverseResult(IAction &action);
Result<void> traverseResult(IAction &action) const;
```

### Node access

```cpp
Node &root();
const Node &root() const;
Node &operator[](std::string_view key);         // object member
Node &operator[](std::size_t index);            // array element
```

### File format enum

```cpp
enum class Format : uint8_t { utf8, utf8BOM, utf16BE, utf16LE, utf32BE, utf32LE };
```

---

## EmbeddedJSON class

`EmbeddedJSON` inherits from `JSON` and adds:

- Compile-time policy queries (all `constexpr`, `noexcept`)
- Exception-free (`NoThrow`) variants of every mutable operation
- `Limits` struct for resource-bound introspection

```cpp
class EmbeddedJSON final : public JSON { ... };
```

### Compile-time policy queries

```cpp
static constexpr bool isEmbeddedBuild()       noexcept;  // JSON_LIB_EMBEDDED
static constexpr bool isExceptionFreeBuild()  noexcept;  // JSON_LIB_NO_EXCEPTIONS
static constexpr bool isNoStdIoBuild()        noexcept;  // JSON_LIB_NO_STDIO
static constexpr bool isNoDynamicMemoryBuild()noexcept;  // JSON_LIB_NO_DYNAMIC_MEMORY
```

### Exception-free API

```cpp
Result<Node> parseNoThrow(ISource &source) const;
Result<Node> parseNoThrow(ISource &&source) const;
Result<void> stringifyNoThrow(IDestination &destination) const;
Result<void> stringifyNoThrow(IDestination &&destination) const;
Result<void> printNoThrow(IDestination &destination) const;
Result<void> printNoThrow(IDestination &&destination) const;
Result<void> traverseNoThrow(IAction &action);
Result<void> traverseNoThrow(IAction &action) const;
```

### Limits

```cpp
struct Limits {
    // Compile-time values (reflect CMake overrides; 0 = library default)
    static constexpr unsigned long kMaxParserDepth  = ...;  // default 10
    static constexpr uint64_t      kMaxStringLength = ...;  // default 16384

    // Runtime accessors (authoritative; may be changed at startup)
    static uint64_t      maxStringLength() noexcept;
    static unsigned long maxParserDepth()  noexcept;
};
```

---

## Node and accessors

`Node` is a variant that holds one of: `Object`, `Array`, `String`, `Number`, `Boolean`, `Null`, or `Hole`.

Access node values via `NRef<T>`:

```cpp
namespace js = JSON_Lib;

js::JSON json;
json.parse(js::BufferSource{R"({"name":"Alice","score":42,"active":true})"});

std::string name = js::NRef<js::String> (json["name"]).value();
int         score= js::NRef<js::Number> (json["score"]).value<int>();
bool        active=js::NRef<js::Boolean>(json["active"]).value();
```

Array access:

```cpp
json.parse(js::BufferSource{R"([10,20,30])"});
int first = js::NRef<js::Number>(json[0]).value<int>();
```

---

## Sources (input)

All sources implement `ISource`.

| Class | Description |
|---|---|
| `BufferSource(std::string)` | Heap-owned string buffer |
| `BufferSource(std::string_view)` | Borrowed string view |
| `FixedBufferSource(const char*, size_t)` | Zero-heap; borrows a raw byte region (ROM-safe) |
| `FileSource(std::string_view)` | Reads from a file (disabled when `JSON_LIB_NO_STDIO == 1`) |

Include `"implementation/io/JSON_Sources.hpp"` for all of the above.

---

## Destinations (output)

All destinations implement `IDestination`.

| Class | Description |
|---|---|
| `BufferDestination` | Heap-backed `std::string`; `toString()` returns result |
| `FixedBufferDestination<N>` | Stack-allocated array of `N` bytes; no heap required |
| `FileDestination(std::string_view)` | Writes to a file (disabled when `JSON_LIB_NO_STDIO == 1`) |

Include `"implementation/io/JSON_Destinations.hpp"` for all of the above.

### FixedBufferDestination<N> notes

```cpp
js::FixedBufferDestination<256> dest;
json.stringify(dest);

const char*  raw  = dest.data();
std::size_t  len  = dest.size();
std::string  str  = dest.toString();

// When JSON_LIB_NO_EXCEPTIONS == 1, overflow sets a flag instead of throwing
#if JSON_LIB_NO_EXCEPTIONS
if (dest.overflowed()) { /* handle */ }
dest.clearOverflow();
#endif
```

---

## Result\<T\> — exception-free API

`Result<T>` is returned by all `...Result` and `...NoThrow` methods.

```cpp
template<typename T>
struct Result {
    Status      status;           // Status::Ok on success
    std::unique_ptr<T> value;     // valid when ok()
    std::string message;          // error description when !ok()
    std::pair<long,long> position;// {line, column} on parse errors

    bool ok()      const noexcept;
    T&   unwrap();                // asserts ok()
};

template<>
struct Result<void> {
    Status      status;
    std::string message;
    std::pair<long,long> position;
    bool ok() const noexcept;
};
```

`Status` values: `Ok`, `SyntaxError`, `OutOfMemory`, `InvalidKey`, `InvalidIndex`, `UnsupportedEncoding`, `InvalidInput`, `NoData`, `UnknownError`.

---

## Error handling

Default (exceptions enabled) builds throw on any parse, type, or I/O error:

```cpp
try {
    js::JSON json;
    json.parse(js::FileSource{"input.json"});
} catch (const std::exception &ex) {
    std::cerr << ex.what() << "\n";
}
```

Exception-free builds (or when using `EmbeddedJSON`):

```cpp
js::EmbeddedJSON embedded;
auto result = embedded.parseNoThrow(js::BufferSource{jsonText});
if (!result.ok()) {
    // result.message contains the error description
    // result.position.first / .second give the line/column
}
```

---

## Traversal — IAction

Implement `IAction` to walk the JSON tree without materializing copies:

```cpp
struct MyAction : JSON_Lib::IAction {
    void onString(const JSON_Lib::String &s) override { ... }
    void onNumber(const JSON_Lib::Number &n) override { ... }
    // onBoolean, onNull, onArray, onObject overrides as needed
};

MyAction action;
json.traverse(action);
```

See `examples/include/JSON_Analyzer.hpp` and `JSON_Convert.hpp` for ready-made implementations.

---

## Stringify backends

The library ships four stringify headers (header-only, no extra link step):

| Header | Output format |
|---|---|
| `implementation/stringify/Default_Stringify.hpp` | Compact JSON (default) |
| `implementation/stringify/Bencode_Stringify.hpp` | Bencode |
| `implementation/stringify/XML_Stringify.hpp` | XML |
| `implementation/stringify/YAML_Stringify.hpp` | YAML |

Pass a custom `IStringify*` to the `JSON` constructor, or use the dedicated example programs (`JSON_Files_To_Bencode.cpp`, `JSON_Files_To_XML.cpp`, `JSON_Files_To_YAML.cpp`).

---

## Build config macros

Generated into `JSON_Config.hpp` (in the CMake binary dir) at configure time.

| Macro | Type | Source |
|---|---|---|
| `JSON_VERSION_MAJOR/MINOR/PATCH` | `int` | CMake project version |
| `JSON_LIB_EMBEDDED` | `0` or `1` | `-DJSON_LIB_EMBEDDED=ON` |
| `JSON_LIB_NO_EXCEPTIONS` | `0` or `1` | `-DJSON_LIB_NO_EXCEPTIONS=ON` |
| `JSON_LIB_NO_HEAP` | `0` or `1` | `-DJSON_LIB_NO_HEAP=ON` |
| `JSON_LIB_NO_DYNAMIC_MEMORY` | `0` or `1` | `-DJSON_LIB_NO_DYNAMIC_MEMORY=ON` |
| `JSON_LIB_NO_STDIO` | `0` or `1` | `-DJSON_LIB_NO_STDIO=ON` |
| `JSON_LIB_ALLOCATOR_INTERFACE` | `0` or `1` | `-DJSON_LIB_ALLOCATOR_INTERFACE=ON` |
| `JSON_LIB_MAX_PARSER_DEPTH` | `unsigned long` | `-DJSON_LIB_MAX_PARSER_DEPTH=N` |
| `JSON_LIB_MAX_STRING_LENGTH` | `uint64_t` | `-DJSON_LIB_MAX_STRING_LENGTH=N` |

`JSON_LIB_NO_HEAP` and `JSON_LIB_NO_DYNAMIC_MEMORY` are kept in sync by CMake — setting either sets both.

---

For usage walkthroughs see [guide.md](guide.md).
