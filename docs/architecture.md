# JSON_Lib Architecture Overview

This document describes the major layers and modules in `JSON_Lib`, how the public interface is separated from implementation details, and the recommended integration points for library users.

## Public API Layer

The public API is centered on `JSON` and `EmbeddedJSON` in `classes/include/JSON.hpp`.
Library consumers should include the facade header:

```cpp
#include "JSON_Lib.hpp"
```

This header exposes:
- `JSON` — the primary façade for parsing, traversing, and serializing JSON
- `EmbeddedJSON` — the embedded-friendly exception-free façade
- `Node` and JSON value variants
- `JSON::Format` for file encoding options
- `JSON_IO.hpp` helpers for `BufferSource`, `FixedBufferSource`, `FileSource`, `BufferDestination`, `FixedBufferDestination`, and `FileDestination`

### Recommended public includes

- `JSON_Lib.hpp` — main public façade
- `JSON_IO.hpp` — optional I/O helper header
- `JSON_Config.hpp` — generated compile-time flags (automatically included by `JSON.hpp`)

The library exposes a single public include root at `classes/include`. Applications should not rely on `classes/include/implementation/*` headers directly; those are internal implementation details and are not part of the supported public API.

`JSON_Config.hpp` is generated from `JSON_Config.h.in` and contains only public feature macros required by the installed headers. It does not expose internal build machinery or implementation-specific configuration details.

## Core library modules

### JSON façade

`classes/include/JSON.hpp` defines the stable public API:
- parsing methods (`parse`, `parseResult`)
- serialization (`stringify`, `print`, `stringifyToString`)
- safe access helpers (`contains`, `at`)
- file helpers (`fromFile`, `toFile`, `getFileFormat`)
- embedded-friendly aliases (`prettyPrint`, `prettyPrintResult`)

The façade delegates tree storage and parsing to `JSON_Impl`, while disk file I/O operations are delegated to `JSON_FileIO` and `IEncodingHandler` strategies, enforcing the Single Responsibility Principle.

### Implementation layer

Internal implementation headers are located under `classes/include/implementation` and include:
- `implementation/file` — `JSON_FileIO` dedicated file reading/writing service
- `implementation/io/encoding` — `IEncodingHandler` strategy implementations (`Utf8EncodingHandler`, `Utf8BOMEncodingHandler`, `Utf16BEEncodingHandler`, `Utf16LEEncodingHandler`, `EncodingHandlerFactory`)
- `implementation/common` — shared utilities such as `JSON_Attributes.hpp`, error handling, and macro definitions
- `implementation/io` — source and destination abstractions for file, buffer, and fixed-size I/O
- `implementation/node` — node indexing, references, and variant wrapper logic
- `implementation/parser` — parsing strategies and translator integration
- `implementation/stringify` — default and alternate serialization backends

These headers are not part of the public installed API and should not be included directly by consumers.

### Parser, stringify, and translator plug-ins

The library supports custom backends via strategy abstractions:
- `IParser` allows custom JSON parsing strategies
- `IStringify` allows custom serialization formats or output behavior
- `ITranslator` allows custom string escaping and decoding strategies

Custom backends are injected through `JSON::Options` or the `JSON` constructor:

```cpp
js::JSON::Options options;
options.setStringify(js::makeStringify<js::XML_Stringify>())
       .setParser(std::make_unique<MyCustomParser>())
       .setTranslator(std::make_unique<MyCustomTranslator>());
js::JSON json(std::move(options));
```

## Embedded and policy layer

`EmbeddedJSON` extends `JSON` with compile-time policy query methods and exception-free workflows.

Key compile-time queries:
- `isEmbeddedBuild()`
- `isExceptionFreeBuild()`
- `isNoStdIoBuild()`
- `isNoDynamicMemoryBuild()`

`EmbeddedJSON::Limits` exposes both compile-time constants and runtime accessors for parser depth and string length limits.

## Error handling and result API

The public API exposes both throwing and result-based paths:
- `JSON` methods throw exceptions on failure
- `JSON::...Result` methods return `Result<T>` objects for exception-free handling
- `EmbeddedJSON` adds `parseNoThrow`, `stringifyNoThrow`, `printNoThrow`, and `traverseNoThrow`

This layered design makes the library usable in both hosted and embedded environments.

## Build and integration

`CMakeLists.txt` defines the modular build structure:
- `JSON_Lib_Core` object library for core implementation
- `JSON_Lib_Parser` object library for parser code
- `JSON_Lib_Stringify` interface library for stringify backends
- `JSON_Lib_Embedded` interface library for compile-time policy flags

The final public target is `JSON_Lib`, which users link into their own projects.

## Summary

The architecture is designed to keep the public interface small and stable while allowing internal implementation details to evolve.

Public consumers should use `JSON_Lib.hpp` and `JSON_IO.hpp`, avoid internal `implementation/*` headers, and rely on `EmbeddedJSON` for constrained or exception-free builds.
