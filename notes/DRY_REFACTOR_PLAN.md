# JSON_Lib DRY Refactor Plan

## 1. Goal

Create a maintainable, DRY internal library architecture while preserving the existing public API. The plan focuses on removing repeated implementation patterns, centralizing shared behavior, and reducing per-node and per-operation overhead across parser, storage, stringify, and I/O subsystems.

## 2. Key findings

- `Node` currently stores object and array values through `std::unique_ptr`, creating repeated heap allocations and extra indirection.
- `ObjectEntry` stores a `std::string` key plus a nested `Node`, duplicating object data layout and slowing lookup/plain iteration.
- `Object` implements two separate index policies for embedded and desktop builds, which duplicates key lookup logic and error handling.
- Parser and stringify classes use repeated type-dispatch patterns rather than a centralized visitor/variant approach.
- `Default_Stringify` builds JSON output with temporary string operations and repeated indentation loops.
- Build and test targets duplicate include-path and target configuration patterns.

## 3. DRY refactor strategy

### 3.1 Centralize node storage semantics

- Replace `Node` heap-pointer storage for object/array values with an inline or policy-based discriminated storage model.
- Keep `Node` semantics but remove repeated `std::unique_ptr` overhead for the common container types.
- Introduce a shared `NodeStoragePolicy` or `VariantStorage` abstraction so desktop and embedded modes share the same API but use different allocations.

### 3.2 Unify object container behavior

- Replace duplicated `Object::contains` / `Object::find` / `Object::getIndex` logic with a single internal lookup function.
- Use a single `Index` alias with policy-driven implementation:
  - `std::unordered_map<std::string, std::size_t>` for desktop
  - ordered static index or linear search for embedded
- Avoid storing `Node` wrappers inside the key/value structure when only the value is required.

### 3.3 Deduplicate parser/stringify dispatch

- Centralize the node type dispatch logic into shared utilities or variant visitation helpers.
- Replace repeated `isA<T>(jNode)` chains with a common `visit`-style helper where practical.
- In the parser, replace any runtime `std::map`/`std::function` dispatch with direct character switch or table-based function pointers.

### 3.4 Consolidate string escaping and formatting

- Move quoted-string output and escape handling into a shared helper instead of duplicated methods.
- Reuse indentation output logic for objects and arrays rather than repeating loops in both `stringifyObject` and `stringifyArray`.
- Avoid temporary `std::string` concatenations for output by appending characters and string views directly to `IDestination`.

### 3.5 Centralize configuration and build-time flags

- Keep compile-time feature selection in the generated `JSON_Config.hpp`.
- Use the same build option names across `CMakeLists.txt` and the headers to avoid duplicate semantics.
- Avoid parallel manual include-path setup in multiple CMake targets by creating shared include-group variables.

## 4. Refactor roadmap

### Phase 1: Shared infrastructure

1. Create a common utility module for:
   - node type dispatch/visitor helpers
   - string escape and quoted output helpers
   - error/status construction
   - one shared `StoragePolicy` header with desktop/embedded specializations
2. Move repeated include-path definitions and target properties into reusable CMake variables/macros.

### Phase 2: Core node and object storage refactor

1. Refactor `classes/include/implementation/node/JSON_Node.hpp`
   - remove `std::unique_ptr<Object>` / `Array` from `Node::Storage`
   - unify the representation of container values with inline or policy-driven storage
2. Refactor `classes/include/implementation/variants/JSON_Object.hpp`
   - remove duplicated index policy branch logic
   - centralize `reserve`, `contains`, `find`, `operator[]`, and duplicate-key handling
   - use smaller `ObjectEntry` layout if possible
3. Refactor `classes/include/implementation/variants/JSON_Array.hpp`
   - centralize array storage helpers and capacity handling

### Phase 3: Parser and stringify simplification

1. Refactor `classes/include/implementation/parser/Default_Parser.hpp`
   - introduce a small parser dispatch helper for value type detection
   - remove or simplify repeated parse helpers if they follow the same shape
2. Refactor `classes/include/implementation/stringify/Default_Stringify.hpp`
   - centralize indentation and comma formatting into shared helpers
   - reduce repeated `destination.add` calls into a minimal output primitive layer
3. Extract common serialization helpers to a separate shared header used by both parse and stringify if applicable.

### Phase 4: Build/test cleanup

1. Refactor `CMakeLists.txt` to use reusable variables for common include directories.
2. Consolidate repeated test and benchmark target configuration into shared functions/macros.
3. Add optional build modes for `JSON_LIB_EMBEDDED`, `JSON_LIB_NO_EXCEPTIONS`, and `JSON_LIB_NO_STDIO` consistently.

### Phase 5: Validation and metrics

1. Add focused regression tests for the refactored core behaviors:
   - object lookup and `contains`/`find`
   - node creation and move semantics
   - parser/stringify correctness with pretty printing and compact mode
2. Add benchmark coverage for parse/stringify and object/array operations.
3. Verify the refactor with desktop and embedded-like build profiles.

## 5. High-value DRY targets

- `classes/include/implementation/node/JSON_Node.hpp`
- `classes/include/implementation/variants/JSON_Object.hpp`
- `classes/include/implementation/variants/JSON_Array.hpp`
- `classes/include/implementation/variants/JSON_String.hpp`
- `classes/include/implementation/parser/Default_Parser.hpp`
- `classes/include/implementation/stringify/Default_Stringify.hpp`
- `classes/include/implementation/io/JSON_BufferSource.hpp`
- `classes/include/implementation/io/JSON_BufferDestination.hpp`
- `CMakeLists.txt`
- `tests/CMakeLists.txt`

## 6. Immediate next step

Start by extracting shared node dispatch and string output helpers, then refactor `JSON_Node` storage and `JSON_Object` lookup into a single shared policy implementation.
