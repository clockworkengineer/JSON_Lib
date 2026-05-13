# JSON_Lib Concrete Refactor Plan — Library Quality Attributes

**Date:** 2026-05-13  
**Scope:** All ten library-quality attributes from `attributes.md`  
**Working assumption:** changes must not break the existing public API unless explicitly called out as a breaking change.

---

## Attribute 1 — Intuitive API Design

### Findings
| Location | Issue |
|---|---|
| `JSON.hpp` / `IStringify.hpp` | `JSON(IStringify*, IParser*)` takes raw **owning** pointers. Ownership transfer is silent; `makeStringify<T>()` calls `.release()` to signal this, but callers who forget leak memory. |
| `JSON.hpp` | `parse()` is marked `const` but internally mutates the tree through the pImpl. This violates the principle of least surprise. |
| `JSON.hpp` | `setIndent()` and `strip()` are `static` methods that affect global/shared state, yet `parse()`/`stringify()` are instance methods — inconsistent design. |
| `JSON_Node_Reference.hpp` | `NRef<T>(jNode)` is a cryptic name. `isA<T>(jNode)` is a free function that duplicates the member `jNode.is<T>()`. |
| `JSON.hpp` | `JSON::root()` returns a non-const reference from a const method — the const overload is correct but the non-const one has a misleading signature. |

### Concrete Tasks

**1a. Change constructor parameter ownership from raw to `unique_ptr`** *(breaking)* ✅ DONE  
- Changed `JSON(IStringify*, IParser*)` → `JSON(std::unique_ptr<IStringify>, std::unique_ptr<IParser>)`.
- Updated `JSON_Impl` constructor to match; replaced `reset(ptr)` with `std::move(uptr)`.
- Updated `makeStringify<T>()` in `IStringify.hpp` to return `std::unique_ptr<IStringify>`.
- All call sites (examples + tests) use `makeStringify<T>()` and required no changes.
- All 1904 assertions pass.

**1b. Remove `const` from `parse()` overloads**  
- **Files:** `classes/include/JSON.hpp`, `classes/source/JSON.cpp`
- `parse(ISource&)` and `parse(ISource&&)` are logically mutating — drop `const`.
- Same for `parseResult` overloads.

**1c. Provide a readable type alias for `NRef<T>()`**  
- **Files:** `classes/include/implementation/node/JSON_Node_Reference.hpp`
- Add `template<typename T> T& as(Node& n) { return NRef<T>(n); }` and the const overload.  
- Keep `NRef` for backwards compatibility but document it as a legacy alias.

**1d. Make `setIndent()` an instance method on `JSON`**  
- **Files:** `classes/include/JSON.hpp`, `classes/source/JSON.cpp`, `classes/include/implementation/JSON_Impl.hpp`
- Replace `static void setIndent(long)` with `void setIndent(long)` that forwards to the owned `IStringify`.
- Remove the static global `Default_Stringify::defaultPrintIndent` (see also Attribute 3).

---

## Attribute 2 — Comprehensive Documentation

### Findings
| Location | Issue |
|---|---|
| All `interface/` headers | Only `ISource.hpp` and `IStringify.hpp` have partial doc comments. `IAction.hpp`, `IDestination.hpp`, `IParser.hpp`, `ITranslator.hpp` have section banners but no parameter/return docs. |
| Whole library | No Doxygen configuration file (`Doxyfile`) exists. |
| `README.md` / `docs/` | Thread safety is never mentioned anywhere. |
| `classes/include/implementation/` | `JSON_Node_Reference.hpp`, `JSON_Node_Index.hpp`, `JSON_Node_Creation.hpp` have no doc comments at all. |

### Concrete Tasks

**2a. Add `Doxyfile` to the repository root**  
- **Files:** create `Doxyfile`
- Configure `INPUT` to `classes/include`, `classes/source`; `RECURSIVE = YES`; `EXTRACT_ALL = YES`.
- Add `docs` as output directory; exclude `build/` and `_deps/`.
- Add a `docs` CMake target: `find_package(Doxygen OPTIONAL_COMPONENTS dot)` → `doxygen_add_docs(...)`.

**2b. Add Doxygen `/// @brief / @param / @return` comments to all public interfaces**  
- **Files:** `interface/IAction.hpp`, `interface/IDestination.hpp`, `interface/IParser.hpp`, `interface/ISource.hpp`, `interface/IStringify.hpp`, `interface/ITranslator.hpp`
- Each virtual method must have at minimum `@brief` and `@return` where applicable.

**2c. Add a thread-safety notice to `README.md` and `docs/api.md`**  
- Note that `Default_Parser::maxParserDepth`, `String::maxStringLength`, and `Default_Stringify::defaultPrintIndent` are global mutable state — **not** thread-safe.  
- Recommend creating one `JSON` instance per thread, and not modifying static state concurrently.

**2d. Document `Result<T>` usage patterns in `docs/guide.md`**  
- Add a dedicated section showing the full exception-free workflow with `parseResult` → `ok()` check → `unwrap()`.

---

## Attribute 3 — High Reliability

### Findings
| Location | Issue |
|---|---|
| `Default_Parser.hpp` | `inline static unsigned long maxParserDepth` — shared across all `JSON` instances and all threads. Tests that change it bleed state into subsequent tests. |
| `JSON_String.hpp` | `inline static uint64_t maxStringLength` — same global state problem. |
| `Default_Stringify.hpp` | `static long defaultPrintIndent` — same. |
| `JSON_Node_Index.hpp` | `Node::operator[](size_t)` catches `Node::Error` from array bounds and silently auto-grows the array. This is surprising and hides bugs in user code. |
| `JSON_FixedBufferSource.hpp` | The constructor sets `valid_ = false` in no-exceptions mode on invalid input, but `next()` still calls `JSON_THROW` (→ `abort()`). The graceful flag is never observed. |
| `JSON_BufferSource.hpp` | `BufferSource(const char*)` uses `strlen()` — silent truncation on buffers containing embedded null bytes. |

### Concrete Tasks

**3a. Move parser depth from static to instance state**  
- **Files:** `classes/include/implementation/parser/Default_Parser.hpp`, `classes/source/implementation/parser/Default_Parser.cpp`
- Change `inline static unsigned long maxParserDepth` to a non-static member with the same default value.
- Update `setMaxParserDepth()`/`getMaxParserDepth()` to be non-static instance methods.
- Update `JSON_Impl` to pass depth configuration through to the owned parser.

**3b. Move string max-length from static to instance state**  
- **Files:** `classes/include/implementation/variants/JSON_String.hpp`
- Change `inline static uint64_t maxStringLength` to a regular constructor parameter or thread-local. 
- Provide a free function `setDefaultStringLength(uint64_t)` that sets a process-wide default, plus an instance `String(value, maxLen)` constructor.

**3c. Move stringify indent from static to instance state**  
- **Files:** `classes/include/implementation/stringify/Default_Stringify.hpp`
- Change `static long defaultPrintIndent` to a non-static member.
- This is already partially done through the `setIndent()`/`getIndent()` virtual pair — remove the static entirely.

**3d. Remove silent auto-grow in `Node::operator[](size_t)`**  
- **Files:** `classes/include/implementation/node/JSON_Node_Index.hpp`
- Remove the `try/catch(Error)` that silently expands the array.
- If auto-grow behaviour is desired, expose it as a separate method (e.g., `resize(n)`), not via `operator[]`.

**3e. Fix `FixedBufferSource` no-exceptions inconsistency**  
- **Files:** `classes/include/implementation/io/JSON_FixedBufferSource.hpp`
- The constructor already sets `valid_ = false`. Add a public `bool valid() const noexcept` accessor.
- Change `next()` to guard on `valid_` and call the abort path only when genuinely out of bounds — not on construction-time invalidity.
- Ensure `more()` returns `false` when `valid_` is `false` so the parser loop terminates cleanly.

**3f. Add explicit length parameter overload for `BufferSource(const char*, size_t)` and deprecate null-terminated ctor**  
- **Files:** `classes/include/implementation/io/JSON_BufferSource.hpp`
- The `(const char*, size_t)` overload already exists. Mark `explicit BufferSource(const char*)` as `[[deprecated("Use the (const char*, size_t) overload to avoid silent truncation on embedded nulls")]]`.

---

## Attribute 4 — Performance and Efficiency

### Findings
| Location | Issue |
|---|---|
| `JSON_BufferDestination.hpp` | `toString()` returns `std::string` by value — a copy of the full output buffer. Large outputs double memory. |
| `Default_Parser.hpp` | All nine private static parse methods take `const ITranslator&` as a parameter on every recursive call — passing through the stack on deeply nested documents. |
| `JSON_Object.hpp` | `Object::add()` calls `contains()` first (linear scan of the key string in the vector) before the O(1) hash insert. For large objects this is an unnecessary O(n) scan. |
| `JSON_Impl.hpp` | `runStringify()` is a private helper called by `stringify`, `stringifyResult`, `print`, `printResult` — fine, but the Result wrappers allocate a `std::string` message even on the happy path. |

### Concrete Tasks

**4a. Add `string_view` accessor to `BufferDestination`**  
- **Files:** `classes/include/implementation/io/JSON_BufferDestination.hpp`
- Add `JSON_LIB_NODISCARD std::string_view view() const noexcept { return buffer; }`.
- Update examples and benchmarks to use `view()` where the string is only read, not owned.

**4b. Cache translator reference as a member in `Default_Parser`**  
- **Files:** `classes/include/implementation/parser/Default_Parser.hpp`, `classes/source/implementation/parser/Default_Parser.cpp`
- The `jsonTranslator` member already exists on the class. Remove the `const ITranslator&` parameter from all nine private static methods and change them to non-static — they will use `jsonTranslator` from `*this`.
- Depth can remain a parameter (it changes per call frame).

**4c. Remove redundant `contains()` scan in `Object::add()`**  
- **Files:** `classes/include/implementation/variants/JSON_Object.hpp`
- In the non-embedded path, the `unordered_map` insert already enforces uniqueness. Change to attempt the insert first and throw on duplicate detection from the map's return value, removing the preliminary linear `contains()` check.

**4d. Pre-reserve `BufferDestination` on construction**  
- **Files:** `classes/include/implementation/io/JSON_BufferDestination.hpp`
- Add an optional `explicit BufferDestination(size_t reserveBytes = 0)` constructor that calls `buffer.reserve(reserveBytes)` — allows callers to avoid reallocation for known output sizes.

---

## Attribute 5 — Maintainability

### Findings
| Location | Issue |
|---|---|
| `JSON_Core.hpp` | Includes 20+ headers unconditionally. Any TU that includes this pulls in the parser, all stringifiers, all IO types, translator, converter — even if only `Node` is needed. |
| `Default_Parser.hpp` | Includes both `JSON.hpp` and `JSON_Core.hpp` — these are the two heaviest headers in the library. |
| `JSON_Node_Creation.hpp` | Contains a `static Node typeToNode(...)` free function — `static` at namespace/file scope in a header is technically legal but gives each TU its own copy, inflating binary size. |
| `JSON_Impl.hpp` + `JSON.hpp` | Every public method on `JSON` is a thin delegation to `JSON_Impl`. These ~25 method pairs are pure boilerplate. |
| `JSON_StringifierBase.hpp` | Declares the base class but the `stringifyNumber`, `stringifyString`, etc. protected helpers used by all subclasses are defined elsewhere (presumably `JSON_StringifierBase.cpp` or inlined into the base header body not shown). This separation makes it hard to find where a helper lives. |

### Concrete Tasks

**5a. Split `JSON_Core.hpp` into focused sub-headers**  
- **Files:** `classes/include/JSON_Core.hpp` (refactor)
- Create `JSON_Node_Core.hpp` (Node + variants only), `JSON_IO_Core.hpp` (sources + destinations), `JSON_Parser_Core.hpp` (parser + translator).
- Keep `JSON_Core.hpp` as an include-all convenience header with a deprecation note.
- Update internal includes to use the minimal header needed.

**5b. Remove heavy includes from `Default_Parser.hpp`**  
- **Files:** `classes/include/implementation/parser/Default_Parser.hpp`
- Replace `#include "JSON.hpp"` and `#include "JSON_Core.hpp"` with only the specific headers needed: `JSON_Node_Fwd.hpp`, `IParser.hpp`, `ITranslator.hpp`, `ISource.hpp`.
- Move full includes to `Default_Parser.cpp`.

**5c. Change `typeToNode` from `static` to `inline` (or move to `.cpp`)**  
- **Files:** `classes/include/implementation/node/JSON_Node_Creation.hpp`
- Replace `static Node typeToNode(...)` with `inline Node typeToNode(...)`. This keeps one definition and avoids per-TU copies.

**5d. Document the `JSON` → `JSON_Impl` delegation pattern**  
- **Files:** `classes/include/JSON.hpp`
- Add a file-level comment explaining the pImpl idiom and noting that all instance methods delegate to `impl_`.
- This is low-effort and eliminates confusion for contributors.

**5e. Consolidate protected helpers in `JSON_StringifierBase`**  
- **Files:** `classes/include/implementation/stringify/JSON_StringifierBase.hpp`
- All `stringify*` helper methods (`stringifyNumber`, `stringifyString`, `stringifyObject`, etc.) that subclasses (`Default_Stringify`, `XML_Stringify`, `YAML_Stringify`, `Bencode_Stringify`) share should be declared (and inlined if small) in `JSON_StringifierBase.hpp`.
- Currently finding where a stringify helper is defined requires hunting across multiple files.

---

## Attribute 6 — Flexibility and Customization

### Findings
| Location | Issue |
|---|---|
| `CMakeLists.txt` | `JSON_LIB_ALLOCATOR_INTERFACE` is a build option but no `IAllocator.hpp` interface exists — it is an unfulfilled stub. |
| `IAction.hpp` | All 14 virtual callbacks are no-op defaults. It is not immediately clear which events fire in which order during traversal without reading the traversal implementation. |
| `Default_Stringify.hpp` | A custom `ITranslator` can be passed but only via wrapping it in a custom `IStringify` subclass — there is no easier path. |
| `JSON.hpp` | No mechanism to substitute a custom `ISource` factory or add middleware between source and parser. |

### Concrete Tasks

**6a. Implement or remove `JSON_LIB_ALLOCATOR_INTERFACE`**  
- **Files:** `CMakeLists.txt`, `JSON_Config.h.in`, new `classes/include/interface/IAllocator.hpp`
- Option A (implement stub): Create `IAllocator.hpp` with `allocate(size_t)` / `deallocate(void*, size_t)` pure virtuals. Guard `Object`, `Array`, `Node` allocation behind the interface when `JSON_LIB_ALLOCATOR_INTERFACE == 1`.
- Option B (remove): Remove the `JSON_LIB_ALLOCATOR_INTERFACE` CMake option and the config macro until there is a real design. A misleading option is worse than no option.

**6b. Add a traversal order reference to `IAction.hpp`**  
- **Files:** `classes/include/interface/IAction.hpp`
- Add a doc comment on the class describing the traversal contract: pre-order, which callbacks fire for each node type, and the expected call sequence for arrays/objects.

**6c. Add a convenience `Default_Stringify` constructor that accepts `ITranslator*`**  
- **Files:** `classes/include/implementation/stringify/Default_Stringify.hpp`
- Add `explicit Default_Stringify(ITranslator* translator)` that wraps the raw pointer in a `unique_ptr` — mirrors the existing `JSON(IStringify*, IParser*)` pattern and allows passing a custom translator without subclassing `IStringify`.

---

## Attribute 7 — Strong Security

### Findings
| Location | Issue |
|---|---|
| Entire library | All static/inline-static mutable state means concurrent calls to `JSON::parse()` on different instances from different threads are **not safe** without external synchronisation. This is undocumented. |
| `Default_Parser.hpp` | `kDefaultMaxParserDepth = 10` — very conservative. Legitimate JSON (e.g. MongoDB BSON-derived, Kubernetes manifests) can exceed this. The default is silent; violating it throws with an opaque error. |
| `JSON_BufferSource.hpp` | `BufferSource(const char*)` uses `strlen` — a null byte in the middle of a buffer silently truncates the parse without error. |
| `IParser.hpp` | `parseResult` default implementation wraps `parse()` in a try/catch but re-throws anything that isn't `std::exception` — unknown exception types can escape. |

### Concrete Tasks

**7a. Add a thread-safety guarantee section to `docs/api.md`**  
- Document: (a) concurrent reads of an already-parsed tree are safe; (b) concurrent writes or concurrent parse of different `JSON` objects sharing static state is not safe.
- Document recommended pattern: create `JSON` instance per thread or use a mutex around static-state-modifying calls.

**7b. Raise `kDefaultMaxParserDepth` to 128 and document it**  
- **Files:** `classes/include/implementation/parser/Default_Parser.hpp`
- Change `kDefaultMaxParserDepth` from `10` to `128`. The current default rejects many valid real-world documents.
- Add a `@note` doc comment explaining the depth limit and how to raise it.

**7c. Catch all exceptions in `IParser::parseResult` default implementation**  
- **Files:** `classes/source/interface/IParser.cpp` (or wherever `parseResult` is defined)
- Change the catch block to `catch (...)` and set `Status::UnknownError` rather than re-throwing.
- Same fix for any `traverseResult` / `stringifyResult` wrappers that do not catch `...`.

**7d. Deprecate `BufferSource(const char*)` (covered by 3f)**  
- Already listed under Attribute 3 task 3f.

---

## Attribute 8 — High Testability

### Findings
| Location | Issue |
|---|---|
| `Default_Parser.hpp`, `JSON_String.hpp`, `Default_Stringify.hpp` | Static mutable state makes tests order-dependent. A test that calls `setMaxParserDepth(5)` leaks that state into every subsequent test. |
| `tests/include/JSON_Lib_Tests.hpp` | No RAII helper to save and restore static configuration between tests. |
| `tests/source/` | No mock implementations of `IAction`, `IParser`, `IStringify` are provided. Each test file must implement its own. |
| `tests/source/parse/` | Parser depth limit is tested but the test changes the static and does not restore it on failure. |

### Concrete Tasks

**8a. Add RAII scope-guard helpers for static configuration**  
- **Files:** `tests/include/JSON_Lib_Tests.hpp`
- Add:
  ```cpp
  struct ScopedMaxDepth {
    unsigned long saved;
    explicit ScopedMaxDepth(unsigned long d)
      : saved(Default_Parser::getMaxParserDepth())
    { Default_Parser::setMaxParserDepth(d); }
    ~ScopedMaxDepth() { Default_Parser::setMaxParserDepth(saved); }
  };
  struct ScopedMaxStringLength { /* same pattern for String::maxStringLength */ };
  struct ScopedIndent { /* same pattern for defaultPrintIndent */ };
  ```
- Use these guards in every test that changes static state.

**8b. Add a `tests/mocks/` directory with reusable mock implementations**  
- **Files:** create `tests/include/mocks/Mock_Action.hpp`, `tests/include/mocks/Mock_Parser.hpp`, `tests/include/mocks/Mock_Stringify.hpp`
- `Mock_Action`: records which callbacks were called and in which order — useful for traversal tests.
- `Mock_Parser`: returns a pre-built Node regardless of source — useful for testing `JSON_Impl` in isolation.

**8c. Address static-state issues once tasks 3a–3c are done**  
- After moving `maxParserDepth`, `maxStringLength`, and `defaultPrintIndent` to instance state, the scope-guard helpers become unnecessary for those fields. The task order is: **3a–3c first, then 8a as a short-term bridge until 3a–3c land**.

**8d. Add a compile-time check that `Result<T>` is not ignored**  
- **Files:** `classes/include/implementation/common/JSON_Error.hpp`
- Add `[[nodiscard]]` to `Result<T>` and `Result<void>` structs themselves (not just `ok()`).

---

## Attribute 9 — Compatibility and Portability

### Findings
| Location | Issue |
|---|---|
| `CMakeLists.txt` | Requires C++23 and hard-errors (`set(CMAKE_CXX_STANDARD_REQUIRED ON)`). No C++20 fallback path. |
| `XML_Stringify.hpp` | Uses `std::ranges::replace` (C++20 ranges). If C++23 is a hard requirement this is fine; if a C++20 path is ever needed, this call must be guarded. |
| `JSON_FileSource.hpp` | `#include <filesystem>` — on older Linux with GCC < 9, this requires `-lstdc++fs`. CMake `target_link_libraries` does not explicitly link `stdc++fs`. |
| `stdafx.h` | A Visual Studio precompiled-header file at the workspace root. It is not referenced by any CMake target — dead artifact. |
| `CMakeLists.txt` | Catch2 is fetched via `FetchContent` — good, but there is no `find_package(Catch2)` fallback for environments where downloading is prohibited. |
| `JSON_Number.hpp` | `long double` is 80-bit on x87, 64-bit on ARM and MSVC — arithmetic results vary by platform. No documentation of this. |

### Concrete Tasks

**9a. Add `stdc++fs` link for GCC < 9**  
- **Files:** `CMakeLists.txt`
- After the `add_library` block, add:
  ```cmake
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(${JSON_LIBRARY_NAME} PRIVATE stdc++fs)
  endif()
  ```

**9b. Add `find_package(Catch2)` fallback before `FetchContent`**  
- **Files:** `tests/CMakeLists.txt`
- Wrap the `FetchContent_Declare` block in `if(NOT Catch2_FOUND)` with a prior `find_package(Catch2 3 QUIET)`. This allows CI environments with a pre-installed Catch2 to skip the download.

**9c. Delete `stdafx.h`**  
- **Files:** `stdafx.h` at workspace root
- Confirm it is not included by any source or CMake target, then remove it.

**9d. Document `long double` platform variance in `JSON_Number.hpp`**  
- **Files:** `classes/include/implementation/variants/JSON_Number.hpp`
- Add a `@note` comment on the `Values` variant: *"On x86/x87, `long double` is 80-bit extended precision. On ARM, MSVC, and most non-x86 targets it is the same width as `double`. JSON serialization of `long double` values may differ across platforms."*

---

## Attribute 10 — Low Dependency Footprint

### Findings
The library already excels here:
- Zero runtime dependencies beyond the C++ standard library.
- Catch2 only for tests, fetched via `FetchContent`.
- Examples reference an `external/` directory but this is isolated to the examples target.

### Concrete Tasks

**10a. Document zero-runtime-dependency guarantee in `README.md`**  
- Add a "Dependencies" section making the guarantee explicit.

**10b. Audit `examples/include/external/`**  
- **Files:** `examples/include/external/`
- Confirm the contents are header-only and isolated to the `BUILD_EXAMPLES` target. Ensure they are not transitively exposed via `target_include_directories(PUBLIC ...)` on the main library target.

---

## Implementation Priority Order

| Priority | Tasks | Rationale |
|---|---|---|
| **P0 — Critical** | 3a, 3b, 3c, 3e, 7c, 8d | Global mutable state and silent error suppression are correctness bugs, not style issues. |
| **P1 — High** | 1a, 1b, 3d, 4b, 4c, 5b, 7b | API clarity and performance — these affect every user every day. |
| **P2 — Medium** | 2a, 2b, 2c, 2d, 4a, 4d, 5a, 5c, 8a, 8b | Documentation, testability tooling, and maintainability. |
| **P3 — Low** | 1c, 1d, 5d, 5e, 6a, 6b, 6c, 9a, 9b, 9c, 9d, 10a, 10b | Ergonomics, portability edge-cases, and nice-to-haves. |

---

## Breaking-Change Summary

Only the following tasks are breaking to the public API:

| Task | Breaking change |
|---|---|
| 1a | `JSON` constructor parameter type changes from `IStringify*` to `unique_ptr<IStringify>`. Call sites must update. |
| 1b | `parse()` / `parseResult()` drop `const`. Const `JSON` objects can no longer be parsed (which was logically wrong anyway). |
| 1d | `setIndent()` becomes a non-static instance method. Code calling `JSON::setIndent(n)` must change to `json.setIndent(n)`. |
| 3a | `setMaxParserDepth` / `getMaxParserDepth` become non-static. |
| 3d | `operator[](size_t)` on a Node no longer auto-grows. User code relying on this behaviour must call `resize()` explicitly. |

All other tasks are purely additive or internal.
