# Changelog

All notable changes to **JSON_Lib** are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.3.0] - 2026-09-25

### Fixed
- **Clang 18 Compatibility**: Deferred `Node` constructors, destructor, and move operations out of inline class definition to resolve `std::default_delete` instantiation with incomplete types under Clang and libstdc++ `<variant>`.

### Changed
- **Documentation**: Added official CI, C++23, license, and donation badges to `README.md` and streamlined build configuration sections.

---

## [1.2.0] - 2026-09-25

### Added
- **CMake Packaging & Export Config**: Added `JSON_LibConfig.cmake` and `JSON_LibConfigVersion.cmake` generation, enabling seamless integration via `find_package(JSON_Lib CONFIG REQUIRED)` and target `JSON_Lib::JSON_Lib`.
- **CPack Support**: Added archive package generation (`.tar.gz`, `.zip`) containing full headers, compiled libraries, and CMake export targets.
- **Portable UTF-8 / UTF-16 Converter**: Implemented a standalone, thread-safe UTF conversion routine in pure C++20/C++23, eliminating deprecated `<codecvt>` usage and Windows SDK `<windows.h>` header leakage.
- **Official JSONTestSuite Integration**: Integrated official `nst/JSONTestSuite` into Catch2 unit tests (`JSON_Lib_Tests_JSONTestSuite.cpp`), conditionally enabled when downloaded via `FetchContent` or script.
- **Standards Conformance Documentation**: Added [docs/conformance.md](docs/conformance.md) providing a full specification matrix (RFC 8259, ECMA-404) and documenting dialect choices.
- **Packaging Documentation**: Added [docs/packaging.md](docs/packaging.md) covering CMake integration, `FetchContent`, installation, build profiles, and CPack.
- **GitHub Actions CI/CD**: Added multi-platform workflow (`.github/workflows/ci.yml`) covering Linux (GCC, Clang), Windows (MSVC), macOS (Apple Clang), AddressSanitizer, UndefinedBehaviorSanitizer, and constrained build profiles (`JSON_LIB_EMBEDDED`, `NO_EXCEPTIONS`, `NO_STDIO`, `NO_DYNAMIC_MEMORY`).
- **Automated GitHub Releases**: Added `.github/workflows/release.yml` to package and attach binary release archives on tag push.
- **LLVM libFuzzer Harness**: Added continuous fuzzing target under `tests/fuzz/JSON_Lib_Fuzz_Parser.cpp` with CMake option `JSON_LIB_BUILD_FUZZERS`.
- **Community Templates**: Added `.github/PULL_REQUEST_TEMPLATE.md` and GitHub issue templates for bug reports and feature requests.
- **Security Policy**: Added [SECURITY.md](SECURITY.md) describing supported versions and reporting guidelines.

### Changed
- Refactored public headers with `__has_include(...)` guards to allow headers to be consumed seamlessly both from source tree and from installed directories (`<JSON_Lib.hpp>` and `<JSON_Lib/JSON_Lib.hpp>`).
- Decoupled `JSON_Lib` archive target from private link dependency on `JSON_Lib_Stringify` to guarantee clean CMake target export.
- Made `IEncodingHandler` and `EncodingHandlers.hpp` self-contained with explicit header includes and warning-clean return paths.

---

## [1.1.0] - 2026-09-24

### Added
- **SOLID Architecture Refactoring**:
  - Fine-grained visitor interfaces (`INodeVisitor`, `IValueVisitor`, `IContainerVisitor`) adhering to Interface Segregation Principle (ISP).
  - Open/Closed encoding strategy pattern (`IEncodingHandler`, `EncodingHandlerFactory`).
  - Dependency inversion for parser string translation (`ITranslator`).
  - Decoupled file I/O operations (`JSON_FileIO`).
- Architectural documentation in [docs/solid_architecture.md](docs/solid_architecture.md).

---

## [1.0.0] - 2026-09-20

### Added
- Initial release of `JSON_Lib`.
- High-performance RFC 8259 JSON parser and serializer.
- Polyglot stringification to XML, YAML, and Bencode.
- Configurable resource limits (maximum depth, maximum string length).
- Embedded and constrained profiles (`NO_EXCEPTIONS`, `NO_STDIO`, `NO_DYNAMIC_MEMORY`).
