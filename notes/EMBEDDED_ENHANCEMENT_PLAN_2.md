# JSON_Lib Embedded Enhancement Plan — Round 6

## 1. Baseline & Scope

All Round 5 Embedded work is committed on `embedded-work`.
Baseline: **1892 assertions, 91 test cases** — must pass after every change.

**Scope**: All changes must be:
- **Additive** (`#if`-gated paths or new files) — the default (non-embedded) build is unaffected
- **`JSON_LIB_NO_EXCEPTIONS`** is the primary new target mode; core behaviour under `OFF` must not change
- **`JSON_LIB_EMBEDDED=ON`** embedded build (library only) must continue to compile cleanly after each phase

---

## 2. Findings

### Finding 2.1 — Latent compilation bug: integer `numberToString` uses `std::ostringstream` under `JSON_LIB_EMBEDDED`

**File**: `classes/include/implementation/variants/JSON_Number.hpp`  
**Lines**: 141–145

```cpp
// ALWAYS compiled, even when JSON_LIB_EMBEDDED=1 (sstream excluded):
  } else {
    std::ostringstream os;   // ← std::ostringstream, no <sstream> include
    os << number;
    return os.str();
  }
```

**Header guard at lines 9–14**:
```cpp
#if JSON_LIB_EMBEDDED
#include <array>
#else
#include <iomanip>
#include <sstream>
#endif
```

`<sstream>` is excluded under `JSON_LIB_EMBEDDED`, but the integer path references
`std::ostringstream` unconditionally. This compiles today **only because the template is
never instantiated** in the library-only embedded build (no tests, no examples). Any
consumer that calls `JSON::stringify()` on a JSON number node with an integer value will
get a compile error under `JSON_LIB_EMBEDDED`.

**Fix**: Gate the integer path. Under `JSON_LIB_EMBEDDED` use `std::to_chars`
(`<charconv>` is already included):

```cpp
  } else {
#if JSON_LIB_EMBEDDED
    std::array<char, 32> buf{};
    const auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), number);
    return std::string(buf.data(), ptr);
#else
    std::ostringstream os;
    os << number;
    return os.str();
#endif
  }
```

---

### Finding 2.2 — 46 unguarded `throw` statements make `JSON_LIB_NO_EXCEPTIONS` unusable

`JSON_Config.h.in` declares `JSON_LIB_NO_EXCEPTIONS` and `EmbeddedJSON::isExceptionFreeBuild()`
reports it, but **every throw in the codebase except the 2 in `FixedBufferDestination`** is
completely unguarded. On targets with `-fno-exceptions` the binary fails to link.

**Count by file**:

| File | Throws | Category |
|------|--------|----------|
| `Default_Parser.cpp` | 15 | Syntax errors (parse failures) |
| `Default_Translator.cpp` | 4 | UTF-16 / escape errors |
| `JSON_Impl.cpp` | 4 | Empty JSON, unknown node type |
| `JSON_FixedBufferSource.hpp` | 2 | Null input, read past end |
| `JSON_BufferSource.hpp` | 2 | Empty input, read past end |
| `JSON_FileSource.hpp` | 2 | File open, read past end |
| `JSON_FileDestination.hpp` | 1 | File open |
| `JSON_File.cpp` | 4 | Unsupported BOM format |
| `JSON_Object.hpp` | 2 | Duplicate key, key not found |
| `JSON_Array.hpp` | 1 | Index out of bounds |
| `JSON_Node_Reference.hpp` | 1 | Type mismatch |
| `JSON_Impl.hpp` | 1 | Unknown node type (traverse) |
| Stringify headers | 4 | Unknown node type |
| `JSON_Number.hpp` | 1 | Unknown numeric type |
| Linux/Windows converter | 4 | Null character conversion |
| **Total** | **48** | — |

**Fix**: Introduce a `JSON_THROW(e)` macro in a new header
`classes/include/implementation/common/JSON_Throw.hpp`:

```cpp
// classes/include/implementation/common/JSON_Throw.hpp
#pragma once
#include "JSON_Config.hpp"
#include <cstdlib>   // std::abort

#if JSON_LIB_NO_EXCEPTIONS
  #define JSON_THROW(exception) (std::abort(), static_cast<void>(0))
#else
  #define JSON_THROW(exception) throw exception
#endif
```

Include this header in every translation unit that currently has a `throw`, then do a
mechanical replacement:

```cpp
// Before:
throw SyntaxError(source.getPosition(), "Missing ':' in key value pair.");

// After:
JSON_THROW(SyntaxError(source.getPosition(), "Missing ':' in key value pair."));
```

This one macro covers all 48 sites. The default (exceptions enabled) build is **completely
unaffected** — macro expands to the same `throw` expression. The `std::abort()` + comma
expression suppresses any "statement has no effect" warnings and satisfies the compiler's
type-deduction rules in contexts where the original `throw` appeared inside a function
returning a value (the `[[noreturn]]` attribute of `std::abort` handles control-flow
analysis correctly).

**Files requiring the new include and macro substitution**:
- `classes/source/implementation/parser/Default_Parser.cpp` (15 throws)
- `classes/source/implementation/translator/Default_Translator.cpp` (4)
- `classes/source/implementation/JSON_Impl.cpp` (4)
- `classes/source/implementation/file/JSON_File.cpp` (4)
- `classes/source/implementation/converter/linux/JSON_Converter.cpp` (2)
- `classes/source/implementation/converter/windows/JSON_Converter.cpp` (2)
- `classes/include/implementation/io/JSON_FixedBufferSource.hpp` (2)
- `classes/include/implementation/io/JSON_BufferSource.hpp` (2)
- `classes/include/implementation/io/JSON_FileSource.hpp` (2)
- `classes/include/implementation/io/JSON_FileDestination.hpp` (1)
- `classes/include/implementation/variants/JSON_Object.hpp` (2)
- `classes/include/implementation/variants/JSON_Array.hpp` (1)
- `classes/include/implementation/variants/JSON_Number.hpp` (1)
- `classes/include/implementation/node/JSON_Node_Reference.hpp` (1)
- `classes/include/implementation/JSON_Impl.hpp` (1)
- Stringify headers: `Default_Stringify.hpp`, `Bencode_Stringify.hpp`, `XML_Stringify.hpp` (3)

---

### Finding 2.3 — `FixedBufferSource` constructor has no graceful failure path under `JSON_LIB_NO_EXCEPTIONS`

**File**: `classes/include/implementation/io/JSON_FixedBufferSource.hpp`  
**Lines**: 17–22

```cpp
FixedBufferSource(const char *data, std::size_t length)
  : data_(data), length_(length), position_(0)
{
  if (data == nullptr || length == 0) {
    throw Error("Empty source buffer passed to be parsed.");
  }
}
```

When `JSON_LIB_NO_EXCEPTIONS=1` and Finding 2.2's macro replaces this with `std::abort()`,
calling `EmbeddedJSON::parseNoThrow(FixedBufferSource{nullptr, 0})` terminates the program
instead of returning a `Result::InvalidInput` error. Embedded firmware often checks for
null/zero defensively and expects a Result value back, not a crash.

**Fix**: Under `JSON_LIB_NO_EXCEPTIONS`, set a `valid_` flag in the constructor instead
of aborting, and gate `next()` to be a no-op when `!valid_`:

```cpp
explicit FixedBufferSource(const char *data, std::size_t length)
  : data_(data), length_(length), position_(0)
{
#if JSON_LIB_NO_EXCEPTIONS
  if (data == nullptr || length == 0) { valid_ = false; return; }
#else
  if (data == nullptr || length == 0) {
    JSON_THROW(Error("Empty source buffer passed to be parsed."));
  }
#endif
}

bool more() const override { return valid_ && position_ < length_; }

// Accessor (gated to avoid layout changes in default builds):
#if JSON_LIB_NO_EXCEPTIONS
[[nodiscard]] bool valid() const noexcept { return valid_; }
#endif

private:
  const char *data_{nullptr};
  std::size_t length_{0};
  std::size_t position_{0};
  bool valid_{true};
```

The `more()` change is safe in both modes: when `valid_` is always `true` (default build),
the condition is identical to the current `position_ < length_`. When `parseNoThrow` is
called via `EmbeddedJSON`, parsing an invalid `FixedBufferSource` calls `more()` → `false`
immediately → parser returns empty root → `runStringify` returns `Result::InvalidInput`.

---

### Finding 2.4 — `maxParserDepth` and `maxStringLength` are runtime-settable but not compile-time-configurable

**Files**:
- `classes/include/implementation/parser/Default_Parser.hpp` line 12: `constexpr static unsigned long kDefaultMaxParserDepth = 10;`
- `classes/include/implementation/variants/JSON_String.hpp` line 10: `constexpr static int64_t kDefMaxStringLength = 16*1024;`

Embedded targets running in 4–64 KB RAM need to reduce these. Currently the only way is:
```cpp
Default_Parser::setMaxParserDepth(4);
String::setMaxStringLength(512);
```
This requires startup code and costs a `static` variable write; it also cannot be verified
at compile time.

**Fix**: Add two new CMake options and route them through `JSON_Config.h.in`:

```cmake
# CMakeLists.txt — after existing options
option(JSON_LIB_MAX_PARSER_DEPTH  "Override default maximum JSON parse depth" "")
option(JSON_LIB_MAX_STRING_LENGTH "Override default maximum JSON string length" "")
```

```c
// JSON_Config.h.in — new entries
#define JSON_LIB_MAX_PARSER_DEPTH  @JSON_LIB_MAX_PARSER_DEPTH_VALUE@
#define JSON_LIB_MAX_STRING_LENGTH @JSON_LIB_MAX_STRING_LENGTH_VALUE@
```

In `Default_Parser.hpp`:
```cpp
#if JSON_LIB_MAX_PARSER_DEPTH
  constexpr static unsigned long kDefaultMaxParserDepth = JSON_LIB_MAX_PARSER_DEPTH;
#else
  constexpr static unsigned long kDefaultMaxParserDepth = 10;
#endif
```

In `JSON_String.hpp`:
```cpp
#if JSON_LIB_MAX_STRING_LENGTH
  constexpr static int64_t kDefMaxStringLength = JSON_LIB_MAX_STRING_LENGTH;
#else
  constexpr static int64_t kDefMaxStringLength = 16 * 1024;
#endif
```

`CMakeLists.txt`: when the option is non-empty, pass `-DJSON_LIB_MAX_PARSER_DEPTH=N` etc
via `target_compile_definitions`. When empty, the generated `JSON_Config.hpp` emits `0`
and the `#if 0` branch falls through to the existing defaults — no behaviour change.

`EmbeddedJSON::Limits` can then also expose these as `constexpr`:
```cpp
struct Limits {
  static constexpr unsigned long kMaxParserDepth = Default_Parser::kDefaultMaxParserDepth;
  static constexpr uint64_t kMaxStringLength = String::kDefMaxStringLength;
  static uint64_t maxStringLength() noexcept;    // existing runtime accessor
  static unsigned long maxParserDepth() noexcept; // existing runtime accessor
};
```

---

### Finding 2.5 — `EmbeddedJSON` no-throw API is incomplete: `traverse` has no `Result`-returning overload

**File**: `classes/include/JSON.hpp` lines 77–79

```cpp
void traverse(IAction &action);
void traverse(IAction &action) const;
```

`traverse` throws `Error("No JSON to traverse.")` when called on an empty root (`JSON_Impl.cpp`
lines 102, 107). `EmbeddedJSON` exposes `parseNoThrow`/`stringifyNoThrow`/`printNoThrow`
but no equivalent for traverse, meaning the no-throw API surface is incomplete for any
embedded code that inspects the parsed tree via visitor actions.

**Fix**: Add `traverseNoThrow` overloads to `EmbeddedJSON` using the same `Result<void>`
wrapper pattern as `runStringify`:

In `classes/include/implementation/JSON_Impl.hpp`, add a private declaration:
```cpp
Result<void> runTraverse(IAction &action);
Result<void> runTraverse(IAction &action) const;
```

In `classes/source/implementation/JSON_Impl.cpp`, implement:
```cpp
Result<void> JSON_Impl::runTraverse(IAction &action)
{
  if (!root().valid()) { return Result<void>{Status::InvalidInput}; }
  try { traverse(action); return Result<void>{}; }
  catch (const ISource::Error &e) { return Result<void>{Status::ParseError, e.what()}; }
  catch (const std::exception &e) { return Result<void>{Status::ParseError, e.what()}; }
}
// const overload mirrors the same pattern
```

In `classes/include/JSON.hpp`, add to `JSON` class:
```cpp
Result<void> traverseResult(IAction &action);
Result<void> traverseResult(IAction &action) const;
```

In `EmbeddedJSON`:
```cpp
Result<void> traverseNoThrow(IAction &action)       { return traverseResult(action); }
Result<void> traverseNoThrow(IAction &action) const { return traverseResult(action); }
```

---

## 3. Refactor Roadmap

### Phase 1: Fix latent integer `numberToString` bug under `JSON_LIB_EMBEDDED`

**Files changed**: `classes/include/implementation/variants/JSON_Number.hpp` (1 block)

1. Add `#if JSON_LIB_EMBEDDED` gate around the integer branch of `numberToString<T>`
   (currently lines 141–145).
2. Embedded path: `std::array<char, 32>` + `std::to_chars`.
3. Non-embedded path: unchanged `std::ostringstream`.
4. Add a test in `tests/source/io/JSON_Lib_Tests_EmbeddedJSON.cpp` that calls
   `EmbeddedJSON::stringify()` on a tree containing integer nodes — this test would have
   been a compile error before the fix.

**Expected**: 1892 + 1–2 new assertions.

---

### Phase 2: `JSON_THROW` macro — systematic exception gating for all 48 throw sites

**Files changed**: 1 new header + 16 existing files

1. Create `classes/include/implementation/common/JSON_Throw.hpp`:
   - Define `JSON_THROW(e)` as `throw e` (default) or `(std::abort(), static_cast<void>(0))`.
2. Add `#include "JSON_Throw.hpp"` to each of the 16 files listed in Finding 2.2.
3. Replace every `throw X(...)` with `JSON_THROW(X(...))`.
   - In headers: use a `JSON_THROW(...)` that satisfies the return type
     (compilers accept `__builtin_unreachable()` after `std::abort()` if needed; the
     `(void)0` cast keeps syntax valid for void and non-void returns alike).
4. Add a CMake test configuration `JSON_LIB_NO_EXCEPTIONS=ON` build target (library-only,
   like the existing embedded check) to CI to confirm clean compilation.
5. Add tests in the embedded test file verifying that operations which previously required
   exceptions now return correct `Result` codes (the existing `parseNoThrow` tests already
   cover several; add cases for duplicate-key, index-out-of-bounds, and type-mismatch that
   verify `abort()` is NOT called — i.e., operations that return Result correctly).

**Note**: `JSON_LIB_NO_EXCEPTIONS` can be enabled independently of `JSON_LIB_EMBEDDED`.
The combination `-DJSON_LIB_NO_EXCEPTIONS=ON -DJSON_LIB_EMBEDDED=ON` is the full
bare-metal mode.

**Expected**: No change to assertion count; library-only embedded+no-exceptions build passes.

---

### Phase 3: `FixedBufferSource` graceful failure under `JSON_LIB_NO_EXCEPTIONS`

**Files changed**: `classes/include/implementation/io/JSON_FixedBufferSource.hpp`

1. Add `bool valid_{true}` private member.
2. Constructor: under `JSON_LIB_NO_EXCEPTIONS`, set `valid_ = false; return;` instead of
   aborting when given null/zero-length.
3. `more()`: return `valid_ && position_ < length_`.
4. Add `#if JSON_LIB_NO_EXCEPTIONS bool valid() const noexcept #endif` accessor.
5. Add test: `EmbeddedJSON::parseNoThrow(FixedBufferSource{nullptr, 0})` returns
   `Result::InvalidInput` (no crash).

**Expected**: 1 additional test case, 1–2 new assertions.

---

### Phase 4: Compile-time configurable `maxParserDepth` and `maxStringLength`

**Files changed**: `CMakeLists.txt`, `JSON_Config.h.in`, `Default_Parser.hpp`,
`JSON_String.hpp`, `classes/include/JSON.hpp`, `classes/source/JSON.cpp`

1. Add `JSON_LIB_MAX_PARSER_DEPTH` and `JSON_LIB_MAX_STRING_LENGTH` CMake options.
2. Pass as compile definitions when non-zero.
3. Gate `kDefaultMaxParserDepth` and `kDefMaxStringLength` via `#if` in their respective
   headers — fall through to existing values when the define is 0 (off).
4. Expose as `constexpr static` members in `EmbeddedJSON::Limits`.
5. Add tests verifying `EmbeddedJSON::Limits::kMaxParserDepth` and
   `EmbeddedJSON::Limits::kMaxStringLength` equal the expected compile-time values.

**Expected**: 2–4 additional assertions. No behaviour change in default build.

---

### Phase 5: `EmbeddedJSON::traverseNoThrow` — complete the no-throw API surface

**Files changed**: `classes/include/implementation/JSON_Impl.hpp`,
`classes/source/implementation/JSON_Impl.cpp`, `classes/include/JSON.hpp`,
`classes/source/JSON.cpp`, `tests/source/io/JSON_Lib_Tests_EmbeddedJSON.cpp`

1. Add `runTraverse(IAction&)` / `runTraverse(IAction&) const` to `JSON_Impl`
   (mirrors the existing `runStringify` pattern exactly).
2. Add `traverseResult(IAction&)` / `traverseResult(IAction&) const` to `JSON` class.
3. Add `traverseNoThrow(IAction&)` / `traverseNoThrow(IAction&) const` inline forwarders
   to `EmbeddedJSON`.
4. Add tests: `traverseNoThrow` with a populated tree (succeeds), `traverseNoThrow` on
   empty JSON (returns `Result::InvalidInput`).

**Expected**: 2 additional test cases, 4–6 new assertions.

---

### Phase 6: Validation

1. Run full test suite — must pass all assertions.
2. Run embedded build (`-DJSON_LIB_EMBEDDED=ON`) — must compile cleanly.
3. Run no-exceptions build (`-DJSON_LIB_NO_EXCEPTIONS=ON`) — must compile cleanly.
4. Run combined embedded+no-exceptions build — must compile cleanly.
5. Run benchmark suite — no regression.
6. Update `EMBEDDED_ENHANCEMENT_PLAN_2.md` with final assertion counts.

---

## 4. Non-Goals (Deferred)

The following findings were identified but deferred because they require non-additive
changes that would impact the core library:

| Deferred Finding | Reason |
|-----------------|--------|
| `JSON_String` → static string storage under `JSON_LIB_NO_DYNAMIC_MEMORY` | Requires changing `Node` variant type storage; non-additive, deep impact |
| `JSON_Array`/`JSON_Object` → static pool under `JSON_LIB_NO_HEAP` | Requires new container type; scope of a separate plan |
| `JSON_LIB_ALLOCATOR_INTERFACE` custom allocator hook | Requires `IAllocator` interface + integration at every `std::make_unique` site |
| Stack-allocated `Node` tree for tiny documents | Requires `std::variant` policy changes |

---
