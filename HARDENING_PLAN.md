# JSON_Lib Hardening Plan

## Goal

Harden `JSON_Lib` by reducing attack surface, enforcing parser/resource limits, clarifying embedded/no-exception policies, and improving test coverage for failure modes.

This plan targets the current codebase structure, focusing on the public API, parser implementation, error/result handling, build configuration, and documentation.

---

## Findings

1. `CMakeLists.txt` exposes many internal implementation headers during install and include propagation.
2. The parser limit comment and defaults are inconsistent: the documented default depth is 10 but the code sets 128.
3. `Result<T>` uses `std::unique_ptr<T>` and `std::string` for error text, which can still allocate memory in no-dynamic-memory or embedded builds.
4. `JSON_LIB_NO_STDIO`, `JSON_LIB_NO_EXCEPTIONS`, and other embedded build options are present, but build/test coverage for them is not clearly enforced.
5. Public header documentation exists, yet currently the public facade is mixed with internal implementation exposure.
6. There is no explicit hardening plan for invalid UTF encodings, invalid BOMs, deeply nested or oversized input, and malformed stream handling.

---

## Hardening Goals

- Limit public API and header install surface to stable interfaces only.
- Enforce strict, deterministic parser validation for malformed JSON and invalid encodings.
- Make compile-time and runtime resource limits explicit and testable.
- Ensure no-exception and no-dynamic-memory modes are verifiable.
- Improve error reporting without weakening safety assumptions.
- Document supported build modes, safety guarantees, and failure semantics clearly.

---

## Concrete Actions

### 1. Tighten Public API and Install Surface

- Reduce installed headers to only true public API headers.
- Avoid installing implementation headers under `classes/include/implementation/*` unless they are explicitly part of the public API.
- Ensure `CMakeLists.txt` clearly separates:
  - public headers (`JSON_Lib.hpp`, `JSON.hpp`, `JSON_IO.hpp`, `JSON_Config.hpp`, public interface headers)
  - internal headers required only for library development/tests.
- Confirm `JSON_LIB_EXPORT_INTERFACE` behaves correctly and that consumers cannot rely on private implementation internals.

Primary files:
- `CMakeLists.txt`
- `classes/include/JSON_Lib.hpp`
- `classes/include/JSON.hpp`
- `classes/include/JSON_IO.hpp`
- `docs/api.md`

### 2. Harden Parser Validation and Input Handling

- Audit `Default_Parser` and ensure all parse entry points enforce:
  - valid JSON grammar and token boundaries
  - valid UTF-8/UTF-16/UTF-32 input semantics
  - strict BOM processing for supported encodings
  - no parser recursion beyond `maxDepth`
  - no string values longer than `maxStringLength`
- Add explicit boundary checks for array/object depth and string length inside parser helper functions.
- Ensure error paths never leave partial state or invalid nodes in the result tree.

Primary files:
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/source/implementation/parser/Default_Parser.cpp`
- `classes/include/implementation/common/JSON_Error.hpp`

### 3. Strengthen Error Handling and Exception-Free API

- Standardize result creation so `Result<T>` and `Result<void>` always capture `Status`, message, and source position.
- Make `Result<T>` able to be used without heap allocation in no-dynamic-memory builds if possible.
- Validate that `NoThrow` APIs and `EmbeddedJSON` modes do not inadvertently allocate or throw when configured.
- Add tests for error result semantics and exception-free fallback behavior.

Primary files:
- `classes/include/implementation/common/JSON_Error.hpp`
- `classes/include/implementation/JSON_Impl.hpp`
- `classes/source/JSON.cpp`

### 4. Enforce Build-Time Policy and Resource Limits

- Correct the parser depth default comment and document library default as 128.
- Ensure `JSON_LIB_MAX_PARSER_DEPTH` and `JSON_LIB_MAX_STRING_LENGTH` propagate to both compile-time constants and runtime policy checks.
- Strengthen CMake logic so `JSON_LIB_NO_HEAP` and `JSON_LIB_NO_DYNAMIC_MEMORY` are mutually consistent and clearly documented.
- Ensure embedded build option behavior is explicit and not hidden by defaults.

Primary files:
- `CMakeLists.txt`
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/include/implementation/common/JSON_Attributes.hpp`
- `README.md`
- `docs/guide.md`

### 5. Expand Test Coverage for Failure and Embedded Modes

- Add regression tests for:
  - invalid JSON syntax and malformed escape sequences
  - invalid or unsupported encodings
  - parser depth exhaustion and oversized strings
  - file I/O disabled when `JSON_LIB_NO_STDIO` is active
  - `EmbeddedJSON` no-throw parser/stringify/traverse paths
  - no-exception/no-dynamic-memory build combinations
- Add dedicated test targets or qualifiers for embedded policy builds.
- Validate that `build/tests/JSON_Lib_Unit_Tests` can execute both normal and embedded-specific tests.

Primary files:
- `tests/CMakeLists.txt`
- `tests/source/`
- `CMakeLists.txt`

### 6. Improve Documentation and User Guidance

- Update `README.md` so build option descriptions match actual `CMakeLists.txt` semantics.
- Add a `docs/guide.md` section for safety policies, embedded limitations, and invalid input handling.
- Document the meaning of `JSON_LIB_NO_EXCEPTIONS`, `JSON_LIB_NO_STDIO`, `JSON_LIB_NO_DYNAMIC_MEMORY`, and `JSON_LIB_MAX_PARSER_DEPTH`.
- Record explicit supported compiler requirements and portability constraints.

Primary files:
- `README.md`
- `docs/api.md`
- `docs/guide.md`
- `docs/architecture.md`

---

## Implementation Roadmap

### Phase 1: Scope and Policy Hardening

1. Lock down header install and public API exposure.
2. Correct documentation mismatches and make compile-time options authoritative.
3. Add missing public API docs for embedded and no-exception usage.

### Phase 2: Parser and Error Hardening

1. Audit `Default_Parser` behavior and enforce strict validation.
2. Ensure `JSON_Error` and `Result<T>` are safe for embedded/no-allocation builds.
3. Add parser regression tests for invalid UTF and depth/size attacks.

### Phase 3: Policy Validation and Testing

1. Add test coverage for all library modes.
2. Add a build matrix for normal, embedded, no-exceptions, no-stdio, and no-dynamic-memory.
3. Ensure all regression tests run successfully on `JSON_Lib_Unit_Tests`.

### Phase 4: Review and Release Readiness

1. Run full test suite with `BUILD_TESTING=ON` and `JSON_LIB_EMBEDDED`/`NO_EXCEPTIONS` combinations.
2. Review final headers and install tree for API-only exposure.
3. Publish updated `README.md` and `docs/*` with the new safety and hardening guarantees.

---

## Immediate Priorities

1. Fix public install header exposure in `CMakeLists.txt`.
2. Audit `Default_Parser` for strict size/depth enforcement.
3. Add regression tests for malformed JSON, invalid encoding, and no-stdio/embedded builds.
4. Align documentation with actual behavior and policy defaults.

## Long-term Hardening Targets

- Keep internal implementation headers private unless explicitly part of the public API.
- Ensure parser rejects malicious or malformed payloads predictably.
- Maintain a low-dependency runtime footprint and clear embedded mode support.
- Preserve performance while avoiding unsafe default behavior.
