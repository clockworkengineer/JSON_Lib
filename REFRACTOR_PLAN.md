# JSON_Lib Refactor Plan

## Goal
Create a concrete refactor plan that implements the library attributes defined in `notes/attributes.md` while preserving the current design strengths of `JSON_Lib`.

The plan maps the 10 quality attributes to specific improvements in the current library, using the existing public façade, CMake build structure, and embedded-friendly support as the baseline.

---

## Findings

The repository already demonstrates the intended library attributes in these ways:
- `classes/include/JSON_Lib.hpp` provides a root public facade
- `JSON` and `EmbeddedJSON` support both exception-based and exception-free APIs
- `CMakeLists.txt` offers embedded flags, compile-time limits, and public header installation controls
- Documentation exists for architecture, API, and usage, but it needs closer synchronization to current public and build behavior
- The codebase already separates public interfaces from internal implementation headers, but the install/include exposure is broader than the ideal public API

Key refactor opportunities:
- tighten public API surface and install layout
- align docs with actual CMake options and supported build modes
- harden parser validation, error result handling, and embedded constraints
- extend test coverage for library modes and edge cases
- ensure portability macros and compiler-specific behavior are explicit

---

## Attribute-driven refactor tasks

### 1. Intuitive API Design

Objectives:
- make the public API easy to discover, understand, and use
- reduce confusion between public and internal headers
- improve naming clarity for common operations

Concrete tasks:
- retain `classes/include/JSON_Lib.hpp` as the single public facade and avoid installing internal `implementation/*` headers
- keep `classes/include/JSON.hpp` as the stable public `JSON` interface and document its methods clearly
- preserve `JSON::contains()` and `JSON::at()` as explicit safe accessors for object and array access
- confirm `operator[]` semantics are documented as mutable access and `at()` is bounds-checked
- ensure all public API overloads favor `std::string_view` and treat `const char*` overloads as convenience wrappers
- keep I/O helpers centralized in `classes/include/JSON_IO.hpp`, not in internal implementation headers

Primary files:
- `classes/include/JSON.hpp`
- `classes/include/JSON_Lib.hpp`
- `classes/include/JSON_IO.hpp`
- `classes/include/implementation/io/JSON_Sources.hpp`
- `classes/include/implementation/io/JSON_Destinations.hpp`
- `CMakeLists.txt`
- `README.md`
- `docs/api.md`

### 2. Comprehensive Documentation

Objectives:
- document usage, build options, embedded constraints, and unsupported edge cases
- keep docs aligned with code and API surface

Concrete tasks:
- update `README.md` so the build option table and examples match the active `CMakeLists.txt` behavior
- refresh `docs/api.md` to describe the exact public API available through `JSON_Lib.hpp` and `JSON_IO.hpp`
- extend `docs/guide.md` with focused sections on embedded builds, no-exceptions mode, no-stdio mode, and resource limits
- strengthen `docs/architecture.md` to show public vs internal module boundaries and installation expectations
- add targeted examples for custom parser/stringifier injection, file I/O, `EmbeddedJSON` usage, and direct `Node` creation

Primary files:
- `README.md`
- `docs/api.md`
- `docs/guide.md`
- `docs/architecture.md`
- `examples/`

### 3. High Reliability

Objectives:
- ensure parser and serializer behave predictably across both valid and invalid inputs
- centralize and harden error reporting

Concrete tasks:
- audit `classes/include/implementation/common/JSON_Error.hpp` and related implementation to ensure stable error messages and result semantics
- strengthen `classes/source/implementation/parser/Default_Parser.cpp` to reject malformed JSON, invalid BOMs, and invalid UTF sequences
- verify `JSON_LIB_MAX_PARSER_DEPTH` and `JSON_LIB_MAX_STRING_LENGTH` are enforced in parser and string-handling code paths
- add regression tests for malformed JSON, invalid encodings, parse depth exhaustion, large strings, and invalid file inputs
- ensure `Result<T>` is used consistently on no-throw paths and that public `Result` helpers return clear diagnostics

Primary files:
- `classes/include/implementation/common/JSON_Error.hpp`
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/source/implementation/file/JSON_File.cpp`
- `tests/source/`

### 4. Performance and Efficiency

Objectives:
- preserve the library’s performance posture while avoiding unnecessary overhead
- optimize hot paths without sacrificing safety

Concrete tasks:
- verify public APIs accept `std::string_view` and avoid unnecessary string copies
- inspect parser and stringify implementations for efficient branch handling and minimal data movement
- add or refresh benchmark coverage to capture parse/stringify performance and real-world memory use
- validate that `JSON_LIB_ENABLE_LTO` is correctly applied when optimization is enabled and disabled safely for `O0`

Primary files:
- `CMakeLists.txt`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/source/implementation/stringify/*`
- benchmark entry points and build targets

### 5. Maintainability

Objectives:
- make the codebase easier to understand and extend
- reduce coupling between public API and implementation details

Concrete tasks:
- maintain a clear separation between stable public headers and internal implementation headers
- simplify the `CMakeLists.txt` include setup by exposing only the public interface to consumers and keeping implementation includes private
- document header and interface conventions in `docs/architecture.md` or `docs/guide.md`
- ensure `JSON_Config.hpp` is generated only for public macros and does not expose unnecessary internal build machinery
- keep the internal implementation modular and self-contained, using `JSON_Lib_Core`, `JSON_Lib_Parser`, and `JSON_Lib_Stringify` layers correctly

Primary files:
- `CMakeLists.txt`
- `classes/include/JSON.hpp`
- `classes/include/JSON_Lib.hpp`
- `classes/include/JSON_IO.hpp`
- `classes/include/implementation/*`

### 6. Flexibility and Customization

Objectives:
- preserve the library’s extensibility for custom translators, parsers, and I/O backends
- make customization hooks discoverable and documented

Concrete tasks:
- document `JSON::Options`, `IParser`, `IStringify`, `ISource`, and `IDestination` in `docs/api.md`
- add tests covering custom backend injection and optional fallback behavior
- ensure `EmbeddedJSON::Limits` is documented and available to embedded users as a reliable policy signal
- keep public interfaces stable for future extension while still allowing custom backend substitution

Primary files:
- `classes/include/JSON.hpp`
- `classes/include/interface/IParser.hpp`
- `classes/include/interface/IStringify.hpp`
- `docs/api.md`
- `docs/guide.md`

### 7. Strong Security

Objectives:
- protect host code from malformed input and resource abuse
- make compile-time and runtime policies explicit

Concrete tasks:
- enforce strict parse semantics for invalid UTF, BOMs, and structural violations
- verify limit enforcement for parser depth and string length across all workflows
- add tests for `JSON_LIB_NO_HEAP`, `JSON_LIB_NO_DYNAMIC_MEMORY`, and `JSON_LIB_NO_STDIO` build modes
- document the library’s safety boundaries and policy guarantees in `docs/guide.md`

Primary files:
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/source/implementation/file/JSON_File.cpp`
- `docs/guide.md`

### 8. High Testability

Objectives:
- make it easy to verify correctness and prevent regressions
- ensure all supported build modes are covered by tests

Concrete tasks:
- expand unit tests for `EmbeddedJSON::parseNoThrow`, `stringifyNoThrow`, and `traverseNoThrow`
- add coverage for no-exceptions, no-dynamic-memory, no-stdio, and embedded configuration builds
- document the test build workflow and presets in `README.md`
- make `tests/CMakeLists.txt` easy to extend and ensure it includes targeted reliability and embedded tests
- consider moving policy-specific tests into dedicated folders for clarity

Primary files:
- `tests/CMakeLists.txt`
- `tests/source/`
- `CMakeLists.txt`

### 9. Compatibility and Portability

Objectives:
- ensure the library builds cleanly across supported compilers and platforms
- make platform-specific behavior explicit and auditable

Concrete tasks:
- verify compiler detection and attribute macros in `classes/include/implementation/common/JSON_Attributes.hpp`
- ensure the public API does not expose platform-specific internals unintentionally
- update documentation for supported compiler versions and OS platforms
- validate `CMakeLists.txt` compiler flag behavior for MSVC, GCC, and Clang

Primary files:
- `classes/include/implementation/common/JSON_Attributes.hpp`
- `CMakeLists.txt`
- `README.md`

### 10. Low Dependency Footprint

Objectives:
- keep runtime dependencies minimal
- make build-time dependencies explicit

Concrete tasks:
- document that runtime dependencies are limited to the C++ standard library and that Catch2 is test-only
- ensure public headers do not require any external dependencies beyond the standard library
- confirm installed API includes only the intended public headers and not internal implementation headers
- keep build configuration self-contained and avoid unnecessary third-party runtime requirements

Primary files:
- `README.md`
- `CMakeLists.txt`
- `classes/include/JSON_Lib.hpp`
- `classes/include/JSON.hpp`

---

## Implementation roadmap

### Phase 1: Public API and documentation cleanup

1. verify and harden the `JSON_Lib.hpp` facade and public install interface
2. synchronize `README.md`, `docs/api.md`, `docs/guide.md`, and `docs/architecture.md` with the current code and CMake options
3. hide internal headers from installed API and ensure `implementation/*` is private to consumers
4. add practical examples for embedded usage, custom backends, and file I/O

### Phase 2: Reliability, security, and test coverage

1. audit parser/error handling code for malformed input, limit enforcement, and no-exception semantics
2. add regression tests for invalid JSON, invalid character encodings, parser depth limits, string-length limits, and embedded policy constraints
3. verify `Result<T>` and no-throw APIs behave consistently across public methods
4. add build-mode coverage for embedded-friendly presets and policy-driven tests

### Phase 3: Performance, portability, and release readiness

1. verify `CMakeLists.txt` option behavior and LTO/optimization interactions
2. add or refresh benchmark coverage for parse/stringify performance
3. confirm portability macros and platform compatibility notes are complete
4. finalize documentation, release notes, and API guidance for library consumers

---

## Validation checklist

- [ ] `JSON_Lib.hpp` remains the single public header façade and internal headers are hidden from installed API
- [ ] documentation reflects the exact public API and supported build flags
- [ ] tests cover parse/stringify, embedded/no-throw, no-stdio/no-dynamic-memory, invalid input, and custom backend cases
- [ ] `CMakeLists.txt` build options are consistent with `README.md` and documented presets
- [ ] error handling is centralized and consistent across throw and no-throw workflows
- [ ] compiler/platform macros in `JSON_Attributes.hpp` are portable and correct
- [ ] public headers do not expose non-standard runtime dependencies
- [ ] performance tradeoffs are documented and LTO behavior is validated
- [ ] security-related limits are enforced in both normal and embedded builds
- [ ] the library’s public API is simpler to use and its implementation remains maintainable

---

## Notes

This refactor plan is intentionally concrete and mapped to the library’s existing public façade, documentation structure, CMake build layout, and embedded-friendly capabilities. Start with API clarity and documentation, then move to reliability and test coverage, and finish with portability and performance validation.
