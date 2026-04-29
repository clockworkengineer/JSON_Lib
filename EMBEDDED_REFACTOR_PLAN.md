# JSON_Lib Embedded Systems Refactor Plan

## 1. Current suitability assessment

### Strengths
- Modular CMake structure already splits core, parser, and stringify components.
- Public API is centralized in `classes/include/JSON.hpp` and can be narrowed further.
- Existing benchmark/regression harness provides a validation foundation.
- `JSON_Lib` already has options for example and test control in CMake.

### Weaknesses for embedded deployment
- Heavy dynamic memory usage:
  - `std::vector` for arrays and object entries
  - `std::unordered_map` for object key indexing
  - `std::string` for all input/output and string values
  - `std::unique_ptr<Object>` / `std::unique_ptr<Array>` inside `Node`
- Exception usage throughout parser/stringify/IO and error handling.
- Runtime polymorphism and virtual dispatch in `ISource`, `IDestination`, `IParser`, `IStringify`.
- C++ runtime dependencies:
  - `<iostream>` and `<filesystem>` in file I/O
  - `<sstream>` in version reporting
- No allocator abstraction or memory pool support.
- No explicit no-exceptions or no-heap build mode.
- Current string parser copies into `std::string` before translation.
- Public include paths expose implementation details that can increase binary size.

## 2. Embedded design goals

### Primary goals
- Enable a low-footprint, deterministic JSON library for constrained targets.
- Eliminate or make optional dynamic heap allocation.
- Remove exception-based control flow in embedded mode.
- Minimize dependency on C++ runtime facilities not guaranteed on bare-metal targets.
- Support buffer-backed input/output without `std::string` or file streams.
- Keep a compatibility layer for the existing desktop API where feasible.

### Secondary goals
- Provide compile-time selectable embedded vs desktop variants.
- Introduce a lightweight allocator/pool API for memory-bound environments.
- Keep the library header-only friendly for embedded cross-compilation.
- Preserve correctness of parse/stringify semantics.

## 3. Refactor phases

### Phase 1: Build system and configuration
1. Add `JSON_LIB_EMBEDDED` CMake option.
   - Default OFF.
   - When ON: disable tests/examples, disable LTO and desktop-only extensions.
2. Add `JSON_LIB_NO_EXCEPTIONS` and `JSON_LIB_NO_HEAP` options.
   - Use compile-time macros to change error handling and storage.
3. Add `JSON_LIB_ALLOCATOR_INTERFACE` option.
   - Allow custom user-provided allocator/pool for objects and containers.
4. Restrict public install headers to `classes/include` only.
   - Hide implementation headers under private build include paths.
5. Add a dedicated embedded target or compile flag set for smaller code generation.

### Phase 2: Core node and memory model
1. Replace heap-based `Node` storage with an inline tagged union.
   - Current `Node::Storage` is `std::variant<std::monostate, std::unique_ptr<Object>, std::unique_ptr<Array>, Number, String, Boolean, Null, Hole>`.
   - New design should store `Object` and `Array` inline when possible or use a user-provided allocator.
2. Remove `std::unique_ptr` ownership from `Object` / `Array`.
   - Use inline `Object`/`Array` fields or pool-backed handles.
3. Convert `ObjectEntry.key` to a fixed owned string type or string view into an external buffer.
   - Avoid `std::string` for keys in embedded mode.
4. Replace `std::unordered_map` object index with a deterministic lookup structure.
   - Options: sorted vector + binary search, open-addressed fixed hash table, or compile-time capacity.
   - Allow a simple `contains`/`find` path that does not require dynamic buckets.
5. Replace `std::vector` in `Array` with a fixed-capacity container or allocator-aware dynamic container.
   - Add `.reserve()` semantics for embedded mode.
6. Provide an explicit `Node`/`Container` memory footprint summary for embedded targets.

### Phase 3: Exception-free and error-code handling
1. Add a status/result type for embedded mode:
   - `enum class Status { Ok, SyntaxError, OutOfMemory, InvalidKey, UnsupportedEncoding, ... };`
   - Use `Result<T>` or `bool` return values with output parameters.
2. Replace `throw`/`catch` in parser, stringify, IO, and core operations in embedded mode.
   - `parse()` should return `Status` and optionally store an error position/message in a fixed buffer.
3. Keep desktop mode exceptions as an option behind `JSON_LIB_NO_EXCEPTIONS`.
4. Remove `std::runtime_error` dependency from core embedded code.

### Phase 4: Parser/stringify/runtime dependency reduction
1. Remove `std::string` temporary allocation in `extractString()`.
   - Parse strings directly into a buffer or attach string views to the source buffer.
   - Use a custom escape-handling state machine instead of translator-based temporary building.
2. Replace `std::string` in `Number` parsing with fixed-size character buffer or `std::array<char, N>`.
3. Reduce `Default_Stringify` temporaries:
   - Avoid `destination.add(std::string)` when possible.
   - Add `destination.add(char)` and `destination.add(const char*, size)` overloads for embedded writes.
   - Use direct output of punctuation and escaped fragments.
4. Replace translator dynamic allocation in `Default_Stringify` and `JSON_Impl`.
   - In embedded mode, use a stateless `NoopTranslator` or template translator.
   - Avoid `std::make_unique<Default_Translator>()`.
5. Remove `std::stringstream` and `<filesystem>` from embedded path.
   - Replace version formatting with a simple `snprintf`-style buffer or static string.
6. Add minimal binary-only parser path if needed for embedded (non-stringified formats) while preserving JSON semantics.

### Phase 5: IO and source/destination abstraction
1. Provide buffer-backed `InputSource` / `OutputDestination` with zero-copy support.
   - Avoid copying input into `std::string` when the source is already a buffer.
2. Replace `std::ifstream` and `std::ofstream` usage with portable, optional platform-specific adapters.
   - In embedded mode, file I/O should be disabled or provided by the user.
3. Simplify `ISource` / `IDestination` for embedded mode.
   - Consider making them non-virtual or template-based in embedded builds.
   - Keep `ISource`/`IDestination` virtual only in desktop mode.
4. Add a strict fixed-size destination mode for stringification.
   - Example: `FixedBufferDestination<char, N>` with overflow detection.
5. Add compile-time support for region-of-interest input decoding and output to flash/buffer.

### Phase 6: API and wrapper design
1. Add a dedicated embedded API entrypoint.
   - E.g. `EmbeddedJSON<AllocatorT>` or `EmbeddedJSONParser`.
2. Keep existing `JSON` class as desktop-compatible facade.
   - Under the hood, dispatch to embedded or desktop implementation based on build options.
3. Document the embedded-specific API and supported limitations.
4. Provide a lightweight runtime data structure preview:
   - fixed key/value counts, max string length, max depth.
5. Add macros or config header flags for embedded targets:
   - `JSON_LIB_EMBEDDED`, `JSON_LIB_NO_EXCEPTIONS`, `JSON_LIB_NO_STDIO`, `JSON_LIB_NO_DYNAMIC_MEMORY`.

### Phase 7: Validation and metrics
1. Add focused embedded regression tests.
   - parse/serialize/range checks for fixed-size buffers.
   - error handling with no-heap mode.
   - deterministic memory usage and no unexpected allocations.
2. Add embedded benchmark scenarios for memory and allocation counts.
3. Add a `PHASE_EMBEDDED_BASELINE.md` or extend `PHASE6_VALIDATION.md` with embedded metrics.
4. Add CI coverage for embedded build variants if possible.

## 4. Recommended target files for change
- `CMakeLists.txt`
- `classes/include/JSON.hpp`
- `classes/include/JSON_Core.hpp`
- `classes/include/implementation/node/JSON_Node.hpp`
- `classes/include/implementation/variants/JSON_Object.hpp`
- `classes/include/implementation/variants/JSON_Array.hpp`
- `classes/include/implementation/variants/JSON_String.hpp`
- `classes/include/implementation/variants/JSON_Number.hpp`
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/include/implementation/stringify/Default_Stringify.hpp`
- `classes/include/implementation/io/JSON_BufferSource.hpp`
- `classes/include/implementation/io/JSON_BufferDestination.hpp`
- `classes/include/implementation/io/JSON_FileSource.hpp`
- `classes/include/implementation/io/JSON_FileDestination.hpp`
- `classes/source/implementation/JSON_Impl.cpp`
- `tests/CMakeLists.txt`
- `tests/source/benchmark/JSON_Lib_Tests_Benchmark.cpp`

## 5. Embedded implementation roadmap
1. Add CMake flags and config header support.
2. Introduce compile-time `JSON_LIB_EMBEDDED`/`JSON_LIB_NO_EXCEPTIONS` modes.
3. Replace heap-based `Node` storage and object indexing.
4. Replace exceptions with status/error codes.
5. Replace file/stream I/O with buffer-only abstractions.
6. Validate with embedded-focused tests and metrics.

## 6. Notes
- The current library is functional and modular, but not yet lightweight enough for typical embedded targets.
- The refactor should preserve the desktop API while introducing a separate embedded path.
- The highest payoff for embedded systems is eliminating `std::string`/`std::vector`/`std::unordered_map` heap usage and exceptions.
- A phased approach will allow desktop compatibility to remain intact while gradually introducing embedded optimizations.
