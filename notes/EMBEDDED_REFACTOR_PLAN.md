# JSON_Lib Embedded Systems Refactor Plan

## 1. Current suitability assessment

### Strengths
- Cleanly separated subsystems in `classes/source`, `classes/include`, parser, stringify, IO, and converter code.
- Public API surface is focused in `classes/include/JSON.hpp` and `classes/include/JSON_Core.hpp`.
- Existing CMake build and tests provide a strong baseline for validating compatibility.
- Buffer source/destination abstractions already exist, which is a good starting point for embedded mode.

### Embedded readiness gaps
- Heavy heap usage in core JSON data structures:
  - `std::vector` for arrays
  - `std::unordered_map` for object members
  - `std::string` for keys, values, parse buffers, and serialized output
- Desktop-only runtime dependencies:
  - `std::ifstream` / `std::ofstream`, `<filesystem>`, and file I/O helpers
  - `std::stringstream` in version/reporting and example utilities
- Exception-based error handling throughout parser, converter, and file operations.
- Multiple duplicated implementation paths for I/O, conversion, and string handling.
- Public headers expose broad implementation details and desktop-only APIs.

## 2. Refactor goals

### Primary goals
- Create a reusable embedded-friendly JSON core with optional heapless operation.
- Make desktop vs embedded behavior selectable through compile-time configuration.
- Eliminate duplicate I/O and conversion code paths.
- Centralize error handling, allocator policy, and storage semantics.
- Preserve existing desktop compatibility while adding a lean embedded API.

### Target constraints
- Optional no-heap/non-dynamic memory operation
- Optional no-exceptions operation
- Optional no-stdio/file I/O support
- Minimal runtime dependency for embedded builds
- Configurable fixed-capacity limits for embedded usage

## 3. DRY refactor analysis

### Duplication hotspots
- Build flags and macros are replicated between `CMakeLists.txt` and public headers.
- File I/O and buffer I/O classes duplicate behavior across `JSON_File*` and `JSON_Buffer*` implementations.
- String escaping, translator conversions, and numeric formatting appear in multiple modules.
- Error reporting is duplicated in parser, stringify, file I/O, and converter code.
- Object and array storage concerns are repeated across node variants and public wrapper APIs.

### DRY strategy
- Centralize compile-time feature selection in generated `JSON_Config.hpp`.
- Introduce shared low-level abstractions:
  - `Result<T>` / `Status`
  - `BufferSource` / `BufferDestination`
  - `StoragePolicy` for node/array/object/string storage
  - `PlatformSupport` for optional desktop-only features
- Separate algorithms from allocation and I/O logic.
- Convert desktop-only behavior into optional adapters rather than core semantics.
- Use policy-based templates where the embedded build can replace heap, error, and I/O behavior.

## 4. Refactor roadmap

### Phase 1: Configuration and build isolation
1. Centralize embedded/CD build flags in CMake and generate `JSON_Config.hpp`.
2. Add `JSON_LIB_EMBEDDED`, `JSON_LIB_NO_EXCEPTIONS`, `JSON_LIB_NO_HEAP`, `JSON_LIB_NO_DYNAMIC_MEMORY`, `JSON_LIB_NO_STDIO`, and `JSON_LIB_ALLOCATOR_INTERFACE`.
3. Create an embedded-specific target (`JSON_Lib_Embedded`) to propagate compile definitions.
4. Disable tests/examples/file I/O by default for embedded mode.
5. Restrict public installation to `classes/include` and hide implementation headers.

### Phase 2: Core storage policy refactor
1. Separate `Node` semantics from storage implementation.
2. Introduce a `StoragePolicy` abstraction for arrays, objects, and strings.
3. Replace `std::unordered_map` object storage with a deterministic embedded alternative.
4. Replace `std::vector` array storage with policy-backed containers.
5. Support inline keys/value buffers or string views for embedded keys.
6. Provide desktop storage policy as the compatibility default.

### Phase 3: Unified error handling
1. Define `Status` and `Result<T>` across the library.
2. Replace exceptions with status-based APIs when `JSON_LIB_NO_EXCEPTIONS` or embedded mode is enabled.
3. Add compact error metadata support for position and code.
4. Keep exception-based wrappers for desktop compatibility.

### Phase 4: Parse/stringify and translator consolidation
1. Move common parsing/stringifying algorithms into shared implementation modules.
2. Eliminate duplicated temporary `std::string` allocations in string parsing and escaping.
3. Move translation and escaping helpers into a shared translator interface.
4. Add low-level `IDestination` append primitives for buffer-only output.
5. Isolate desktop-only UTF-16/UTF-32 conversion helpers behind optional adapters.

### Phase 5: I/O abstraction consolidation
1. Make `BufferSource` / `BufferDestination` the core I/O path.
2. Make file sources/destinations optional and disable them under `JSON_LIB_NO_STDIO`.
3. Simplify embedded I/O to non-polymorphic adapters where possible.
4. Add fixed-capacity destination types with overflow detection.
5. Provide hooks for custom platform I/O adapters.

### Phase 6: Embedded-facing API and compatibility layer
1. Add explicit embedded entry points or policies, e.g. `EmbeddedJsonDocument<StoragePolicy>`.
2. Preserve the existing `JSON` facade for desktop and compatibility use.
3. Document embedded limits, build modes, and required macros.
4. Provide embedded builders and fixed-size memory helpers.

### Phase 7: Validation and regression coverage
1. Add embedded-specific unit tests covering:
   - heap-free parse/stringify
   - fixed-size buffer overflow
   - status code failures
   - deterministic container limits
2. Add embedded benchmark scenarios for allocation counts and buffer throughput.
3. Add CI coverage for an embedded build profile.
4. Update docs/guide for embedded usage and compatibility notes.

## 5. Key refactor targets

Primary files and subsystems:
- `CMakeLists.txt`
- `classes/include/JSON.hpp`
- `classes/include/JSON_Core.hpp`
- `classes/include/implementation/node/JSON_Node.hpp`
- `classes/include/implementation/node/JSON_Node_Index.hpp`
- `classes/include/implementation/node/JSON_Node_Reference.hpp`
- `classes/include/implementation/node/JSON_Node_Creation.hpp`
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
- `classes/source/implementation/converter/linux/JSON_Converter.cpp`
- `classes/source/implementation/converter/windows/JSON_Converter.cpp`
- `tests/CMakeLists.txt`
- `tests/source/benchmark/JSON_Lib_Tests_Benchmark.cpp`

## 6. Risks and mitigation

- Risk: Core node/storage refactor may break desktop compatibility.
  - Mitigation: preserve desktop storage policy and add a separate embedded policy.
- Risk: No-exception semantics could change API behavior.
  - Mitigation: keep exception-based wrappers for desktop builds.
- Risk: Fixed-capacity embedded mode may not suit all document shapes.
  - Mitigation: document limits clearly and fail gracefully with `Status::OutOfMemory`.

## 7. Immediate next steps

1. Implement shared compile-time configuration in `CMakeLists.txt` and `JSON_Config.hpp`.
2. Create a centralized `JSON_Lib_Embedded` compile target for embedded mode.
3. Refactor I/O abstractions so `BufferSource`/`BufferDestination` are the core path.
4. Introduce `Status`/`Result` and move shared error handling into a common header.
5. Begin the storage policy refactor in `JSON_Object.hpp`, `JSON_Array.hpp`, and node headers.
