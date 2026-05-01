# JSON_Lib Embedded Enhancement Plan — Round 5

## 1. Baseline & Scope

All Round 4 DRY work is committed on `embedded-work`.
Baseline: **1879 assertions, 80 test cases** — must pass after every change.

**Scope**: All changes must be:
- **Additive** (new code or `#if`-gated paths) — the default (non-embedded) build is unaffected
- **`JSON_LIB_EMBEDDED=ON`** is the target mode; the core library's behaviour under `OFF` must not change

---

## 2. Current Embedded Support Inventory

| Feature | Status |
|---------|--------|
| `JSON_LIB_EMBEDDED` → sorted-vector `ObjectIndex` | ✅ Implemented |
| `JSON_LIB_NO_STDIO` → disables `FileSource`/`FileDestination` | ✅ Implemented |
| `FixedBufferDestination<N>` (stack-allocated output) | ✅ Implemented |
| `EmbeddedJSON` facade class with build-flag queries | ✅ Implemented |
| `JSON_LIB_NO_EXCEPTIONS` flag | ⚠️ Declared, **never guarded** in any implementation code |
| `JSON_LIB_NO_HEAP` / `JSON_LIB_NO_DYNAMIC_MEMORY` flags | ⚠️ Declared, **never guarded** |
| `JSON_LIB_ALLOCATOR_INTERFACE` flag | ⚠️ Declared, **never guarded** |
| `FixedBufferSource` (zero-heap input) | ❌ Missing |
| Float-to-string without `<sstream>`/`<iomanip>` | ❌ Missing |

---

## 3. Findings

### 3.1 `JSON_LIB_NO_EXCEPTIONS` flag exists but has zero implementation

The flag is defined in `JSON_Config.h.in`, exposed in `JSON_Config.hpp`, and reported via
`EmbeddedJSON::isExceptionFreeBuild()`, but **no implementation code inspects it**.
Every error path uses `throw`. On bare-metal targets with `-fno-exceptions`, the binary
simply fails to link.

The safest first step is to gate the overflow `throw` in `FixedBufferDestination` — the only
destination that has an internal overflow condition that is distinct from programmer error:

```cpp
// JSON_FixedBufferDestination.hpp — add(char) today:
if (size_ >= N) { throw Error("Fixed buffer destination overflow."); }

// Under JSON_LIB_NO_EXCEPTIONS:
#if JSON_LIB_NO_EXCEPTIONS
  if (size_ >= N) { onOverflow_(); return; }  // call registered handler, then return
#else
  if (size_ >= N) { throw Error("Fixed buffer destination overflow."); }
#endif
```

**Fix**: Add a `void setOverflowHandler(std::function<void()> h)` method (and a default
`std::terminate` handler) to `FixedBufferDestination`, gated behind
`#if JSON_LIB_NO_EXCEPTIONS`. The non-embedded path keeps the `throw` unchanged.

---

### 3.2 No `FixedBufferSource` — embedded input has no zero-heap option

`BufferSource` accepts `const char*` and `std::string_view` (no heap) but also offers
`std::string`-owning constructors and keeps a `std::string ownedBuffer` member, meaning the
class always has heap-capable constructors and storage even when unused.

Embedded firmware typically has its JSON in ROM (a `const char[]`) or in a fixed SRAM region.
There is no dedicated source type that:
- Is templated on a compile-time size
- Stores nothing on the heap
- Is explicit about ownership (borrows only)

**Fix**: Add `classes/include/implementation/io/JSON_FixedBufferSource.hpp`:

```cpp
// Zero-heap source: wraps a const char* pointer+length, no ownership.
class FixedBufferSource final : public ISource {
public:
  FixedBufferSource(const char *data, std::size_t length);
  // deleted: default, copy, move — same pattern as BufferSource
  ...
};
```

Include it in `JSON_Sources.hpp` unconditionally (it has no stdio dependency).

---

### 3.3 `Number` unconditionally includes `<sstream>` and `<iomanip>`

`JSON_Number.hpp` includes `<sstream>` and `<iomanip>` at the top level. These pull in
significant code (locale machinery, stream buffers) that is often unavailable or
undesirable on embedded targets.

The only consumer of these headers is `numberToString<T>` for floating-point types, which
builds a `std::ostringstream` to format the value. `std::to_chars` (C++17, `<charconv>`) is
already included and used for the integer path — it also supports floating-point.

```cpp
// Current float path in numberToString<T>:
std::ostringstream oss;
oss << std::setprecision(numberPrecision);
if (numberNotation == numberNotation::fixed) oss << std::fixed;
else if (numberNotation == numberNotation::scientific) oss << std::scientific;
oss << number;
return oss.str();

// Replacement using std::to_chars (no locale, no heap):
std::array<char, 64> buf{};
auto fmt = std::chars_format::general;
if (numberNotation == numberNotation::fixed)      fmt = std::chars_format::fixed;
else if (numberNotation == numberNotation::scientific) fmt = std::chars_format::scientific;
auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), number, fmt, numberPrecision);
return std::string(buf.data(), ptr);
```

**Fix**: Gate `<sstream>`/`<iomanip>` behind `#if !JSON_LIB_EMBEDDED`. Under
`JSON_LIB_EMBEDDED`, use `std::to_chars` with `std::chars_format` for the float path in
`numberToString<T>`. The integer path already uses `std::to_string` (or `to_chars`) and is
unaffected.

---

### 3.4 `EmbeddedJSON` does not surface the exception-free API

`EmbeddedJSON` inherits `JSON` via `using JSON::JSON`, which means all `parse`/`stringify`
throwing overloads are equally visible. There are no forwarding methods that direct embedded
users toward `parseResult`/`stringifyResult` — the safe `Result<T>`-returning API.

Under `JSON_LIB_NO_EXCEPTIONS`, users **must** use the `Result` variants but nothing in
`EmbeddedJSON` highlights this.

**Fix**: Add explicit non-throwing forwarding methods directly in `EmbeddedJSON`:

```cpp
class EmbeddedJSON final : public JSON {
public:
  using JSON::JSON;
  ...
  // Exception-free API (preferred for embedded targets)
  Result<Node>  parseNoThrow(ISource &source) const  { return parseResult(source); }
  Result<Node>  parseNoThrow(ISource &&source) const { return parseResult(source); }
  Result<void>  stringifyNoThrow(IDestination &dest) const  { return stringifyResult(dest); }
  Result<void>  stringifyNoThrow(IDestination &&dest) const { return stringifyResult(dest); }
  Result<void>  printNoThrow(IDestination &dest) const  { return printResult(dest); }
  Result<void>  printNoThrow(IDestination &&dest) const { return printResult(dest); }
  ...
};
```

This is purely additive — it does not remove any existing `JSON` method.

---

## 4. Refactor Roadmap

### Phase 1: `FixedBufferSource`

1. Create `classes/include/implementation/io/JSON_FixedBufferSource.hpp`:
   - `class FixedBufferSource final : public ISource`
   - Constructor: `FixedBufferSource(const char *data, std::size_t length)`
   - Deleted: default, copy, move (via `DestinationBase` pattern)
   - Implements `current()`, `next()`, `more()`, `reset()`, `position()`
   - No heap, no ownership — borrows only
2. Add `#include "JSON_FixedBufferSource.hpp"` to `JSON_Sources.hpp` unconditionally.
3. Add tests to the embedded test file.

### Phase 2: `Number` float-to-string via `std::to_chars` under `JSON_LIB_EMBEDDED`

1. In `JSON_Number.hpp`, wrap `#include <sstream>` and `#include <iomanip>` in
   `#if !JSON_LIB_EMBEDDED`.
2. In `numberToString<T>`, gate the float path:
   - `#if JSON_LIB_EMBEDDED`: use `std::to_chars` with `std::chars_format`
   - `#else`: keep existing `std::ostringstream` path unchanged
3. Verify the embedded build (`-DJSON_LIB_EMBEDDED=ON`) compiles and all tests pass.

### Phase 3: `JSON_LIB_NO_EXCEPTIONS` in `FixedBufferDestination`

1. Add `#include "JSON_Config.hpp"` to `JSON_FixedBufferDestination.hpp`.
2. Add `#if JSON_LIB_NO_EXCEPTIONS` guard around the overflow `throw`:
   - Under `NO_EXCEPTIONS`: set an internal `bool overflowed_` flag; provide
     `bool overflowed() const noexcept` and `void clearOverflow() noexcept` accessors.
   - Under normal build: keep `throw Error(...)` unchanged.
3. Add tests to the embedded test file covering the overflow flag path.

### Phase 4: `EmbeddedJSON` no-throw forwarding API

1. In `classes/include/JSON.hpp`, add six `parseNoThrow`/`stringifyNoThrow`/`printNoThrow`
   method declarations to `EmbeddedJSON` forwarding to the `Result<T>` base methods.
2. No `.cpp` changes required (all inline one-liners).
3. Add tests verifying the no-throw methods return the correct `Result` on success and
   on empty-JSON error.

### Phase 5: Validation ✅ COMPLETE

1. ✅ Full test suite: **1892 assertions, 91 test cases** — all passed.
2. ✅ Embedded build (`-DJSON_LIB_EMBEDDED=ON`): compiled cleanly, zero errors.
3. ✅ Benchmark suite: all 2 benchmark test cases passed, no performance regression.

---

## 5. Priority Order

| Priority | Phase | Finding | Impact | Risk |
|----------|-------|---------|--------|------|
| High | 1 | 3.2 | Completes zero-heap embedded I/O story; additive | Low |
| High | 2 | 3.3 | Removes `<sstream>`/`<iomanip>` from embedded build | Low |
| Medium | 3 | 3.1 | First real `NO_EXCEPTIONS` implementation | Medium |
| Low | 4 | 3.4 | API ergonomics for embedded consumers | Low |
| Low | 5 | — | Validation only | None |

---

## 6. Files Affected

| File | Phase(s) |
|------|----------|
| `classes/include/implementation/io/JSON_FixedBufferSource.hpp` (NEW) | 1 |
| `classes/include/implementation/io/JSON_Sources.hpp` | 1 |
| `tests/source/io/JSON_Lib_Tests_EmbeddedJSON.cpp` | 1, 3, 4 |
| `classes/include/implementation/variants/JSON_Number.hpp` | 2 |
| `classes/include/implementation/io/JSON_FixedBufferDestination.hpp` | 3 |
| `classes/include/JSON.hpp` | 4 |
