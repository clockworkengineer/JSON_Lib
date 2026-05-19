# JSON_Lib Refactor Plan

## Goal
Create a concrete refactor plan that implements the library attributes defined in `notes/attributes.md` while preserving the current design strengths of `JSON_Lib`.

This plan maps the 10 quality attributes to concrete tasks for `JSON_Lib` and identifies the key files and subsystems that should be improved.

---

## Summary of findings

The library already has strong foundations:
- Modern C++23 API with `JSON` and `EmbeddedJSON` façades
- Modular build via CMake and object libraries
- Zero runtime third-party dependencies
- Embedded/no-exceptions/no-stdio build modes
- Existing docs (`README.md`, `docs/api.md`) and tests under `tests/source`

Key opportunities for refactor:
- public API clarity and consistency
- stronger, synchronized documentation
- better reliability through focused validation and error handling
- more maintainable module separation and build configuration
- improved test coverage for policy and edge-case behavior
- explicit security and portability validation

---

## Attribute-driven refactor tasks

### 1. Intuitive API Design

Objectives:
- make the public API easy to discover, understand, and use
- reduce confusion between public and internal headers
- improve naming clarity for common operations

Tasks:
- create a single public header facade such as `include/JSON_Lib.hpp` that exposes only stable public types and hides internal implementation headers
- add `JSON::prettyPrint()` alias to complement `print()` and deprecate `print()` later if desired
- add explicit `JSON::contains(std::string_view key) const` and `JSON::at(std::string_view key) const` to avoid ambiguous `operator[]` semantics for object access
- document `JSON::operator[](size_t)` clearly as array-element access and add bounds-safe `at(size_t)` if missing
- unify overloads in `JSON.hpp` so `const std::string_view&` is the canonical input type and `const char*` overloads are wrappers
- expose public input/output helpers through a dedicated public header rather than direct `implementation/io` includes

Primary files:
- `classes/include/JSON.hpp`
- `classes/include/implementation/io/JSON_Sources.hpp`
- `classes/include/implementation/io/JSON_Destinations.hpp`
- `classes/include/implementation/common/JSON_Attributes.hpp`
- `README.md`
- `docs/api.md`

### 2. Comprehensive Documentation

Objectives:
- document usage, build options, embedded constraints, and unsupported edge cases
- keep docs aligned with code and API surface

Tasks:
- update `docs/api.md` to match the current public interface and remove internal implementation references from public docs
- create or extend `docs/guide.md` with a dedicated section for embedded builds, exception-free usage, and limit configuration
- add a `docs/architecture.md` or `docs/module-overview.md` describing the `JSON`, `EmbeddedJSON`, parser, stringify, source, destination, and node layers
- add example snippets for: custom parser/stringify injection, file I/O, embedded no-exception mode, and direct `Node` construction
- ensure `README.md` build instructions and CMake option table remain accurate and include example `cmake` invocations

Primary files:
- `README.md`
- `docs/api.md`
- `docs/guide.md`
- `examples/`

### 3. High Reliability

Objectives:
- ensure parser and serializer work predictably across valid/invalid inputs
- centralize and harden error reporting

Tasks:
- audit `JSON_Error.hpp` and `JSON_Error.cpp` to ensure error codes/messages are consistent, descriptive, and stable
- add regression tests for malformed JSON, invalid BOMs, invalid Unicode, and parser depth/length limit exceedance
- tighten validation in `Default_Parser` and `JSON_FileSource` for invalid inputs rather than allow undefined behavior
- expose any policy or limit failures clearly through `Result<T>` in the exception-free API

Primary files:
- `classes/include/implementation/common/JSON_Error.hpp`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/include/implementation/parser/Default_Parser.hpp`
- `tests/source/parse/` and `tests/source/stringify/`

### 4. Performance and Efficiency

Objectives:
- preserve and document the library's performance posture
- optimize hot paths without compromising safety

Tasks:
- verify `JSON_LIB_LIKELY` / `JSON_LIB_UNLIKELY` are used effectively in parser and stringify hot loops
- remove unnecessary copies in public APIs and prefer `std::string_view` / references when safe
- add or refresh benchmark targets in `examples/` or `tests/benchmarks` to capture parse/stringify times and memory behavior
- validate `JSON_LIB_ENABLE_LTO` / `JSON_LIB_OPTIMIZATION_LEVEL` interaction in `CMakeLists.txt`
- add a performance audit checklist for future changes

Primary files:
- `CMakeLists.txt`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/source/implementation/stringify/*`
- benchmark sources if present

### 5. Maintainability

Objectives:
- make the codebase easier to understand and extend
- reduce coupling between public API and implementation details

Tasks:
- reorganize `classes/include` into stable public headers and internal implementation headers, with clear folder separation
- keep `classes/include/implementation` private to library internals and not installed as public API
- add or update a coding style / contribution guide section for header usage, naming, and target boundaries
- simplify `CMakeLists.txt` by using modern target properties and fewer manual include directory lists
- ensure `JSON_Config.hpp` is generated only for public config macros and does not expose internal build machinery

Primary files:
- `CMakeLists.txt`
- `classes/include/JSON.hpp`
- `classes/include/implementation/*`

### 6. Flexibility and Customization

Objectives:
- preserve the library's extensibility for custom translators and parsers
- document and broaden customization hooks

Tasks:
- document the custom parser/stringify injection APIs in `docs/api.md` and `README.md`
- consider exposing a `JSON::Options` or `JSON::Settings` struct for parse/stringify policies instead of only constructor arguments
- add tests for custom backend injection and fallback behavior when custom I/O components are replaced
- extend `EmbeddedJSON::Limits` documentation with practical usage examples

Primary files:
- `classes/include/JSON.hpp`
- `classes/include/interface/IParser.hpp`
- `classes/include/interface/IStringify.hpp`
- `docs/api.md`

### 7. Strong Security

Objectives:
- protect the host application from malformed input and resource abuse
- make build-time policies explicit and auditable

Tasks:
- verify strict UTF/BOM validation in the parser and disallow invalid encodings
- ensure `JSON_LIB_MAX_PARSER_DEPTH` and `JSON_LIB_MAX_STRING_LENGTH` are enforced in all code paths, including exception-free mode
- improve `no heap` / `no dynamic memory` build documentation and add tests to confirm behavior under those flags
- add a security section to docs describing the library’s strict parse semantics and any safe subset guarantees

Primary files:
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/source/implementation/io/JSON_File.cpp`
- `docs/guide.md`

### 8. High Testability

Objectives:
- make it easy to verify correctness and prevent regressions
- ensure all build modes are covered by automated tests

Tasks:
- extend test coverage for embedded/no-exceptions/no-stdio/no-dynamic-memory builds
- add component tests for `JSON::parseResult`, `JSON::stringifyResult`, and `EmbeddedJSON::parseNoThrow`
- add tests for public API aliases and deprecated names if introduced
- document test-running commands and build matrix for `BUILD_TESTING`, `BUILD_EXAMPLES`, and embedded presets
- make `tests/CMakeLists.txt` and `tests/source` layout easier to find and extend

Primary files:
- `tests/CMakeLists.txt`
- `tests/source/*`
- `CMakeLists.txt`

### 9. Compatibility and Portability

Objectives:
- ensure the library builds cleanly across supported compilers and platforms
- make platform-specific behavior explicit

Tasks:
- ensure `JSON_Attributes.hpp` properly detects attributes on MSVC, GCC, Clang, and fallback compilers
- add or update compiler-specific guard macros for Windows vs Linux path/I/O handling
- add cross-compiler test targets or CI configuration to validate MSVC, GCC, and Clang builds
- validate `std::string_view`, `std::variant`, and C++23 features are used in portable ways

Primary files:
- `classes/include/implementation/common/JSON_Attributes.hpp`
- `CMakeLists.txt`
- `README.md`

### 10. Low Dependency Footprint

Objectives:
- keep runtime dependency count minimal
- make build-time dependencies explicit and easy to obtain

Tasks:
- document that runtime dependencies are limited to the C++ standard library and that Catch2 is a test-only dependency
- simplify build-time dependency setup by using `FetchContent` for Catch2 or documenting how to provide it
- confirm no internal public header includes external dependencies beyond the standard library
- keep installation targets restricted to the API headers and library artifact only

Primary files:
- `README.md`
- `CMakeLists.txt`
- `classes/include/JSON.hpp`

---

## Implementation roadmap

### Phase 1: API and documentation cleanup

1. add `include/JSON_Lib.hpp` or equivalent public header facade
2. refine `JSON.hpp` public methods and add optional clarity helpers (`contains`, `at`, `prettyPrint` alias)
3. segregate headers into public vs internal layout
4. update `README.md`, `docs/api.md`, and `docs/guide.md` to reflect the public API and build options
5. add a `docs/architecture.md` or architecture section

### Phase 2: reliability, security, and test coverage

1. audit and harden error handling in `JSON_Error` and parser logic
2. add targeted regression tests for invalid JSON and embedded build policies
3. implement missing exception-free or limit-bound checks in parser/sources/destinations
4. add test coverage for embedded/no-exceptions/no-stdio/no-dynamic-memory builds

### Phase 3: performance, portability, and release readiness

1. review hot-path annotations and benchmark support
2. verify CMake build options and LTO settings across compilers
3. add compatibility notes and compile-time attribute detection improvements
4. document the final refactor in a release-oriented changelog or upgrade note

---

## Validation checklist

- [ ] public header facade exists and internal headers are hidden from installed API
- [ ] docs reflect supported public methods, macros, and build presets
- [ ] `notes/attributes.md` attributes are mapped to deliverables
- [ ] tests cover parse/stringify, embedded mode, and error handling
- [ ] `CMakeLists.txt` build options are consistent with README and docs
- [ ] `JSON_Attributes.hpp` attribute macros provide portable annotations
- [ ] security-related limits are enforced in both throw and no-throw code paths
- [ ] runtime dependency footprint remains zero beyond the standard library

---

## Notes

This plan is intentionally concrete and implementation-focused. The next step is to convert each task into one or more pull requests or branch-level refactors, starting with API clarity and docs updates, then moving into reliability and test coverage.
