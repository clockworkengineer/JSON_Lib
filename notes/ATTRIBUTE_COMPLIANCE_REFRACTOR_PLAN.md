# JSON_Lib Attribute Compliance Refactor Plan

## Purpose
This document captures a concrete refactor plan for applying C++ attribute-based quality improvements to `JSON_Lib` and aligning the library with the ten high-level attributes described in `notes/attributes.md`.

The goal is to make the library more compliant by standardizing attribute usage, improving ABI and API safety, and making the code easier to maintain and optimize.

## Current Findings

1. `[[nodiscard]]` is already used widely in many public getter functions and value-returning APIs.
2. There is no consistent application of `noexcept`, `constexpr`, or visibility attributes across the public API and hot paths.
3. There is only limited use of compiler-specific attributes (`__attribute__`) in example/external code, not in the core library.
4. The public API and internal interfaces are mixed; some interfaces should be hidden or explicitly marked for export.
5. No explicit deprecation or upgrade path is visible for older or obsolete APIs.

## Compliance Gaps

- `[[nodiscard]]` should extend to all result-bearing functions where ignoring the return value is likely incorrect, especially parsing, error checking, and conversion operations.
- `noexcept` should be applied to all trivial and non-throwing accessors, `const` methods, and low-level helpers to enable better optimization and clearer semantics.
- `constexpr` should be used for constant expressions and sized constants in hot-path classes.
- Public library API should expose symbol visibility through portable attributes or macros to reduce export surface and improve link-time optimization.
- Unused parameter warnings should be formally resolved with `[[maybe_unused]]` only where necessary, rather than suppressed implicitly.
- Intentional switch fall-through and branch prediction hints should be documented with `[[fallthrough]]`, `[[likely]]`, and `[[unlikely]]` only when they improve clarity and performance.
- Deprecated APIs should be marked with `[[deprecated("reason")]]` when an older interface is retained for compatibility but a replacement exists.

## Refactor Roadmap

### Phase 1: Define portable attribute macros
- Add a small header such as `classes/include/implementation/common/JSON_Attributes.hpp`.
- Define macros for `JSON_LIB_NODISCARD`, `JSON_LIB_NOEXCEPT`, `JSON_LIB_CONSTEXPR`, `JSON_LIB_DEPRECATED`, `JSON_LIB_EXPORT`, and `JSON_LIB_HIDDEN`.
- Use compiler detection for MSVC, GCC, and Clang while preserving C++ standard attribute support.

### Phase 2: Apply attribute discipline in core API and hot paths
- Review and update public API interfaces in `classes/include/JSON.hpp`:
  - Add `JSON_LIB_NODISCARD` to parse, conversion, and status-checking methods.
  - Add `JSON_LIB_NOEXCEPT` to trivial getters, `const` accessors, and non-throwing low-level methods.
- Review implementation headers in `classes/include/implementation/` and add `constexpr` to compile-time constants, small helpers, and tag conversions.
- Add `JSON_LIB_NODISCARD` to error result wrappers in `classes/include/implementation/common/JSON_Error.hpp`.

### Phase 3: Tighten implementation visibility
- Mark core library symbols with `JSON_LIB_EXPORT` for the public compile target.
- Mark internal implementation classes and helper functions with `JSON_LIB_HIDDEN` or leave them anonymous inline where possible.
- Refactor CMake to install only public headers and to set `-fvisibility=hidden` when supported.

### Phase 4: Remove attribute misuse and inconsistency
- Find and remove redundant or unnecessary `[[maybe_unused]]` markers.
- Avoid `[[likely]]` / `[[unlikely]]` in the parser or stringifier unless profiling indicates a real branch bias.
- Annotate intentionally falling switch cases with `[[fallthrough]]`.
- Avoid `[[nodiscard]]` on functions where the returned value is intentionally optional to ignore.

### Phase 5: Add deprecation hygiene
- Identify any older APIs in `classes/include/` or public examples that should be phased out.
- Apply `JSON_LIB_DEPRECATED("Use X instead")` selectively to maintain compatibility while signaling future removal.
- Add a migration note in `README.md` or `docs/api.md` if deprecation is introduced.

## Target Files
- `classes/include/JSON.hpp`
- `classes/include/implementation/common/JSON_Error.hpp`
- `classes/include/implementation/common/JSON_StoragePolicy.hpp`
- `classes/include/implementation/node/JSON_Node.hpp`
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/include/implementation/stringify/Default_Stringify.hpp`
- `classes/include/implementation/io/JSON_FixedBufferDestination.hpp`
- `classes/include/implementation/io/JSON_BufferDestination.hpp`
- `CMakeLists.txt`

## Validation
- Add compiler warnings to treat unused return values from key APIs as errors during CI.
- Add or update tests to verify `constexpr` constants, `noexcept` behavior, and public header exposure.
- Validate that all exported symbols match the intended public API after applying visibility macros.
- Confirm that the library still builds with MSVC, GCC, and Clang as supported by the build matrix.

## Expected Outcome
- More consistent and correct attribute usage across the library.
- Improved optimizer friendliness and smaller public symbol surface.
- Clearer semantic contracts for callers, especially when return values should not be ignored.
- A concrete refactor path that can be executed incrementally with low compatibility risk.
