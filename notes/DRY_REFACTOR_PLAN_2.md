# JSON_Lib DRY Refactor Plan — Round 2

## 1. Baseline

All previous DRY work (Phases 1–5) is committed on `embedded-work`.
Baseline: **1879 assertions, 80 test cases** — must pass after every change.

---

## 2. Findings

### 2.1 Destination `lastChar` tracking duplicated in all three concrete destinations

`BufferDestination`, `FileDestination`, and `FixedBufferDestination` each:
- maintain a private `lastChar` / `lastChar_` field
- update it identically in every `add()` overload
- expose it via `char last() override`

The `IDestination` interface declares `last()` but cannot hold state. There is no
shared base between the three classes that carries this behaviour.

**Fix**: introduce a `DestinationBase` CRTP or non-virtual mixin that owns `lastChar`
and provides the `last()` implementation; each concrete class inherits it and calls
`trackLast(ch)` at the end of each `add()` overload.

### 2.2 `IDestination::add()` has four virtual overloads — only `add(char)` is fundamental

All `add(string)`, `add(string_view)`, and `add(const char*)` overloads in every
concrete destination delegate to `add(char)` one character at a time (FileDestination)
or replicate the same `append + lastChar` logic (BufferDestination, FixedBufferDestination).

The interface forces every implementor to provide all four overloads even though
the multi-byte variants are mechanically derivable from `add(char)` with no semantic
difference.

**Fix**: make `add(const std::string_view&)` the single pure virtual in `IDestination`;
provide default implementations of `add(const std::string&)`, `add(const char*)`,
and `add(char)` in the interface that forward to it — or adopt CRTP so the bulk
overloads are generated once.

### 2.3 `traverseNodes` dispatch is still an `isA<T>` chain

`JSON_Impl::traverseNodes` (in `JSON_Impl.hpp`) has the same `isA<T>` if/else chain
that was eliminated from all stringify classes in Phase 3.3. It is the last remaining
occurrence.

**Fix**: replace with `jNode.visit(overloaded{...})`.

### 2.4 `JSON_Node_Creation.hpp` — `typeToNode` is another `get_if` dispatch chain

`typeToNode` uses 10 sequential `std::get_if` checks on an `InitializerListTypes`
variant. This can be replaced with `std::visit`.

**Fix**: replace `typeToNode` with `std::visit(overloaded{...}, type)`.

### 2.5 `Number::getAs` is a 6-arm `get_if` chain

`Number::getAs<T>()` tests six types with `std::get_if`. The `Values` variant already
supports `std::visit`.

**Fix**: replace with `std::visit(overloaded{...}, jNodeNumber)`.

### 2.6 `JSON_Error.hpp` — position-string concatenation duplicated in two error types

`Error` and `SyntaxError` both build `"[Line: N Column: M]: "` with identical
`to_string` concatenation. The only difference is the prefix string.

**Fix**: extract a `static std::string formatPosition(pair<long,long>, string_view prefix)`
free function or a shared base `PositionError`.

### 2.7 `FileDestination::add(string/string_view/const char*)` iterates char-by-char

`FileDestination` translates `\n` to `\r\n` for every character. The three bulk
overloads loop over bytes calling `add(char)`. This is the correct approach for
correct CRLF handling, but the three overloads are identical boilerplate (`for ch: add(ch); flush(); lastChar = back`).

**Fix**: provide a single `add(string_view)` implementation that loops and calls
`add(char)`; the other two overloads delegate to it.

### 2.8 `Default_Translator` uses two separate `unordered_map` lookup tables

`fromEscape` and `toEscape` are populated in the constructor and looked up in `from()`
and `to()`. The maps are `mutable` despite being constant after construction.

**Fix**: change to `const` maps initialized with a `constexpr` or inline-initializer
pattern; or use two `static const` tables to avoid per-instance allocation.

### 2.9 `JSON_Impl.cpp` version string uses repeated `to_string` concatenation

`"JSON_Lib Version  " + to_string(MAJOR) + "." + to_string(MINOR) + "." + to_string(PATCH)`
builds four temporary strings. Minor but consistent with the rest of the refactor.

**Fix**: use `std::format` (C++23) or sequential `add`-style construction with a
`std::string` that is `reserve`d.

---

## 3. Refactor Roadmap

### Phase 1: Destination `lastChar` mixin

1. Add `DestinationBase` (non-virtual, non-template) to a new header
   `classes/include/implementation/io/JSON_DestinationBase.hpp`:
   - owns `char lastChar_{}` field
   - `void trackLast(char ch) noexcept { lastChar_ = ch; }`
   - `char last() noexcept { return lastChar_; }`
2. Inherit `BufferDestination`, `FileDestination`, `FixedBufferDestination` from
   `DestinationBase` publicly; remove the three separate `lastChar` fields; call
   `trackLast()` at end of each `add()`.
3. Override `last()` in each class to return `DestinationBase::last()` (or make
   `DestinationBase::last()` the `override` directly by having it implement the
   virtual from `IDestination`).

### Phase 2: Simplify `FileDestination` bulk `add()` overloads

1. Implement `add(const std::string_view &bytes)` as the primary bulk method
   (iterates, calls `add(char)`).
2. Replace `add(const std::string &)` and `add(const char *)` with one-line
   delegating overloads.

### Phase 3: Replace remaining `isA<T>` chains with `visit`

1. `JSON_Impl.hpp` — `traverseNodes`: replace `isA<T>` if/else with
   `jNode.visit(overloaded{...})`.
2. `JSON_Node_Creation.hpp` — `typeToNode`: replace `get_if` chain with
   `std::visit(overloaded{...}, type)`.
3. `JSON_Number.hpp` — `Number::getAs<T>()`: replace `get_if` chain with
   `std::visit(overloaded{...}, jNodeNumber)`.

### Phase 4: DRY error position formatting

1. Extract `formatPosition(std::pair<long,long>, std::string_view prefix)` free
   function in `JSON_Error.hpp`.
2. Both `Error` and `SyntaxError` position constructors call it.

### Phase 5: Translator and version string cleanup

1. `Default_Translator`: make `fromEscape`/`toEscape` non-mutable; initialize as
   `const` maps (inline static or constructor-initialized const references).
2. `JSON_Impl.cpp` `version()`: replace `to_string` concatenation chain with a
   single `std::format` call or a pre-reserved `std::string` with `append`.

### Phase 6: Validation

1. Run full test suite — must pass 1879 assertions in 80 test cases unchanged.
2. Verify embedded build (`-DJSON_LIB_EMBEDDED=ON`) compiles cleanly.
3. Run benchmark suite to confirm no performance regression.

---

## 4. Priority Order

| Priority | Phase | Impact | Risk |
|----------|-------|--------|------|
| High     | 3     | Eliminates last `isA` chains; consistent with Phase 3.3 | Low |
| High     | 1     | Removes ~30 lines of duplicated `lastChar` tracking | Low |
| Medium   | 4     | Removes duplicated position-string builds | Low |
| Medium   | 2     | Simplifies FileDestination bulk overloads | Low |
| Low      | 5     | Minor cleanup; mutable → const, format string | Low |
| Low      | 6     | Validation only | None |

---

## 5. Files Affected

| File | Phase(s) |
|------|----------|
| `classes/include/implementation/io/JSON_DestinationBase.hpp` (NEW) | 1 |
| `classes/include/implementation/io/JSON_BufferDestination.hpp` | 1 |
| `classes/include/implementation/io/JSON_FileDestination.hpp` | 1, 2 |
| `classes/include/implementation/io/JSON_FixedBufferDestination.hpp` | 1 |
| `classes/include/implementation/JSON_Impl.hpp` | 3 |
| `classes/include/implementation/node/JSON_Node_Creation.hpp` | 3 |
| `classes/include/implementation/variants/JSON_Number.hpp` | 3 |
| `classes/include/implementation/common/JSON_Error.hpp` | 4 |
| `classes/include/implementation/translator/Default_Translator.hpp` | 5 |
| `classes/source/implementation/JSON_Impl.cpp` | 5 |
