# JSON_Lib Refactor Plan: Reduce Size and Increase Performance

## Summary
This plan targets the current `JSON_Lib` implementation, which is functionally complete but has structural inefficiencies that inflate binary size and reduce runtime performance.

Primary issues:
- `Node` values are heap-allocated via `std::unique_ptr<Variant>`, causing high allocation overhead and non-contiguous memory.
- JSON objects use `std::vector<ObjectEntry>` with linear key lookup, which is expensive for large objects.
- The parser uses `std::map<char, std::function<Node(...)>>` for dispatch, adding indirect call overhead and dynamic allocations.
- Static global state in `JSON_Impl` and `Default_Stringify` increases coupling, complicates optimization, and can increase code size.
- Stringification creates temporary `std::string` objects for small constant outputs and indentation, increasing allocations.
- Build configuration currently includes all examples and tests by default and does not expose optimization/size-reduction build modes explicitly.

## Goals
1. Reduce runtime heap allocations and heap-dependent memory usage.
2. Improve parser/stringify dispatch performance.
3. Make node storage more compact and cache-friendly.
4. Shrink final library size by removing unnecessary indirection and controlling build composition.
5. Preserve API compatibility where possible while making targeted internal improvements.

## Phase 1: Benchmark and Baseline
1. Add a small benchmark suite if not already present. Focus on parse, stringify, object lookup, and traversal.
2. Measure current library size and runtime for representative JSON workloads:
   - Large arrays of primitive values
   - Deeply nested objects
   - Object key lookup
   - Pretty-print/stringify output
3. Record baseline numbers before refactoring.

## Phase 2: Core Data Model Refactor
### 2.1 Replace heap-allocated node variant storage
- Refactor `classes/include/JSON_Node.hpp` so `Node` no longer stores `std::unique_ptr<Variant>`.
- Use a tagged union pattern instead of pointer indirection.
- Suggested design:
  - `std::variant<std::monostate, Object, Array, Number, String, Boolean, Null, Hole>`
  - or a custom compact discriminated union with explicit storage.
- Benefits:
  - Eliminates one allocation per `Node`
  - Improves locality and cache behavior
  - Reduces code size from destructors and allocator calls

### 2.2 Simplify variant type hierarchy
- Convert `JSON_Variant.hpp` from a polymorphic base into a simple tag/enum type if possible.
- Remove runtime `getNodeType()` indirection and use direct variant visitation or type dispatch.

### 2.3 Optimize object and array storage
- In `classes/include/implementation/variants/JSON_Object.hpp`:
  - Replace linear `std::vector<ObjectEntry>` lookup with a faster structure.
  - Options:
    - `std::unordered_map<std::string, Node>` for direct key lookup
    - Sorted vector + binary search if insertion order must be preserved
  - Avoid throwing exceptions for normal not-found cases; return optional-like semantics or use `find` APIs.
- In `classes/include/implementation/variants/JSON_Array.hpp`:
  - Reserve capacity before array growth where possible.
  - Keep `operator[]` bounds checks in debug only if release performance is critical.

### 2.4 Reduce `ObjectEntry` overhead
- Avoid storing the object key as a separate `Node`; use a dedicated `std::string` or `std::string_view` key field.
- This removes two nested `Node` objects per entry and simplifies string handling.

## Phase 3: Parser and Stringifier Optimization
### 3.1 Replace parser dispatch table
- In `classes/include/implementation/parser/Default_Parser.hpp`:
  - Remove `std::map<char, ParseFunc>` and `std::function` overhead.
  - Use a direct `switch` on the next non-whitespace character, or a small static array of function pointers keyed by ASCII value.
- Benefits:
  - Lower dynamic dispatch overhead
  - More inlining opportunities
  - Smaller code size than `std::map` + `std::function`

### 3.2 Streamline number/string/boolean parsing
- Ensure parsing routines avoid unnecessary string copies.
- Prefer parsing directly from the source buffer when possible.
- Cache translator output or avoid translator allocations in the hot path.

### 3.3 Reduce stringify temporaries
- In `classes/include/implementation/stringify/Default_Stringify.hpp`:
  - Avoid constructs like `destination.add('"' + ... + '"')` that create temporary strings.
  - Add output one character at a time or use a dedicated `appendQuotedString(destination, str)` helper.
  - Replace `std::string(indent, ' ')` with a reusable indentation buffer or output repeated spaces directly.
- For pretty printing, avoid repeated temporary string construction on each object/array element.

### 3.4 Remove static `unique_ptr` globals
- Convert `jsonTranslator`, `jsonParser`, and `jsonStringify` to instance members or function-local singletons only when needed.
- This reduces global initialization overhead and helps linkers eliminate unused code.

## Phase 4: Build System and API Modularization
### 4.1 Add size/performance build options
- In `CMakeLists.txt`:
  - Add `option(BUILD_EXAMPLES "Build examples" OFF)` and `option(BUILD_TESTS "Build tests" ON)` or similar.
  - Add `option(JSON_LIB_ENABLE_LTO "Enable link-time optimization" ON)`.
  - Add a `JSON_LIB_BUILD_TYPE` or `JSON_LIB_OPTIMIZATION_LEVEL` option to expose `-Os`, `-O2`, `-O3`.
  - Add `option(JSON_LIB_EXPORT_INTERFACE "Install only public headers" ON)`.

### 4.2 Modularize the library
- Split library components into smaller targets if possible:
  - `JSON_Lib_Core`
  - `JSON_Lib_Parser`
  - `JSON_Lib_Stringify`
- This makes it easier to link only the needed pieces in downstream consumers.
- Keep the public API header set minimal and avoid installing internal implementation headers unnecessarily.

### 4.3 Review install and public headers
- The current `target_include_directories` exposes many internal include paths.
- Restrict public headers to the actual user-facing API (`classes/include/`) and hide implementation headers under `PRIVATE` or private include paths.
- This reduces the exposed surface area and helps downstream compilers perform devirtualization and inlining.

## Phase 5: Code and API Clean-up
### 5.1 Remove unnecessary `[[maybe_unused]]`
- Only use `[[maybe_unused]]` where the parameter is genuinely unused; avoid it in hot path functions if possible.
- This improves readability and avoids masking design issues.

### 5.2 Improve const/constexpr usage
- Use `constexpr` for constant character values, default sizes, and format tokens.
- Ensure trivial functions are inlineable and not exposed unnecessarily.

### 5.3 Limit `std::string` copies
- In `String::toString()`, consider returning `const std::string &` or providing an output overload to avoid copy construction.
- In translator and file I/O helpers, accept `std::string_view` and write directly to the destination when possible.

## Phase 6: Validation and Metrics
1. Re-run benchmarks after each major refactor.
2. Compare library size and runtime before/after.
3. Add regression tests for:
   - object lookup speed and correctness
   - allocation counts for Node/array/object operations
   - parse/stringify behavior with large payloads
4. Verify ABI/public API compatibility for existing clients.

## Recommended Refactor Corridors
### Highest-impact changes
- Replace `Node` heap storage with inline variant storage.
- Replace parser dispatch map and `std::function` with direct dispatch.
- Replace object key storage as nested `Node` with plain key strings.
- Avoid temporary string concatenation during stringify.

### Medium-impact changes
- Add library build options for size and LTO.
- Reduce public include exposure and private interface leaks.
- Convert static state to instance members.

### Low-impact / cleanup changes
- Improve error handling to avoid exceptions in normal lookup paths.
- Add benchmarking and performance regression tests.
- Clean up naming and documentation around `JSON::Format` and translator usage.

## Suggested file targets
- `classes/include/JSON_Node.hpp`
- `classes/include/implementation/variants/JSON_Variant.hpp`
- `classes/include/implementation/variants/JSON_Object.hpp`
- `classes/include/implementation/variants/JSON_Array.hpp`
- `classes/include/implementation/variants/JSON_String.hpp`
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/include/implementation/stringify/Default_Stringify.hpp`
- `classes/include/implementation/JSON_Impl.hpp`
- `CMakeLists.txt`
- `tests/` (add benchmarks and regression coverage)

## Notes
- The current code is already clean and modern, but the core node model is the main source of both size and performance costs.
- Refactoring the node representation and parser dispatch first should yield the best payoff.
- After structural refactors, revisit `JSON_File.cpp` and I/O buffer handling for any remaining performance hotspots.
