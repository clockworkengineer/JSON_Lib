# JSON_Lib DRY Refactor Plan — Round 4

## 1. Baseline

All previous DRY work (Rounds 1–3) is committed on `embedded-work`.
Baseline: **1879 assertions, 80 test cases** — must pass after every change.

---

## 2. Findings

### 2.1 Stringify classes repeat translator member + boilerplate special-members

All four stringify classes independently declare:
- An explicit constructor `(std::unique_ptr<ITranslator> translator = make_unique<XTranslator>())` storing the result in a differently-named member (`jsonTranslator`, `bencodeTranslator`, `xmlTranslator`, `yamlTranslator`)
- The identical four deleted special-members + `~override = default`

```cpp
// Default_Stringify.hpp — same pattern in all 4 files, only names differ
explicit Default_Stringify(std::unique_ptr<ITranslator> translator = ...)
  : jsonTranslator(std::move(translator)) {}
Default_Stringify(const Default_Stringify &other) = delete;
Default_Stringify &operator=(const Default_Stringify &other) = delete;
Default_Stringify(Default_Stringify &&other) = delete;
Default_Stringify &operator=(Default_Stringify &&other) = delete;
~Default_Stringify() override = default;
```

**Fix**: introduce `StringifierBase` (non-virtual, inherits `IStringify`) that owns
`std::unique_ptr<ITranslator> translator_`, declares all four deleted special-members,
and exposes the uniform name `translator_`. Each concrete class inherits it and removes
the boilerplate.

---

### 2.2 `DestinationBase` does not declare the deleted copy/move operators

`DestinationBase` owns `lastChar_` and provides `last()`/`trackLast()`, but every
concrete destination (`BufferDestination`, `FileDestination`, `FixedBufferDestination`)
still individually redeclares the identical four-line delete block:

```cpp
// Identical in all three concrete destinations:
ClassName(const ClassName &other) = delete;
ClassName &operator=(const ClassName &other) = delete;
ClassName(ClassName &&other) = delete;
ClassName &operator=(ClassName &&other) = delete;
~ClassName() override = default;
```

**Fix**: add the four `= delete` declarations and `~DestinationBase() override = default`
directly to `DestinationBase`; remove the five identical lines from each concrete class.

---

### 2.3 `stringifyResult` / `printResult` in `JSON_Impl.cpp` duplicate try/catch body

```cpp
// stringifyResult — lines 54-63
Result<void> JSON_Impl::stringifyResult(IDestination &destination) const {
  if (jNodeRoot.isEmpty()) { return {Status::InvalidInput, {}, {0, 0}}; }
  try {
    jsonStringify->stringify(jNodeRoot, destination, 0);
    return {Status::Ok, {}, {0, 0}};
  } catch (const std::exception &ex) {
    return {Status::UnknownError, ex.what(), {0, 0}};
  }
}

// printResult — lines 72-81
Result<void> JSON_Impl::printResult(IDestination &destination) const {
  if (jNodeRoot.isEmpty()) { return {Status::InvalidInput, {}, {0, 0}}; }
  try {
    jsonStringify->stringify(jNodeRoot, destination, jsonStringify->getIndent());
    return {Status::Ok, {}, {0, 0}};
  } catch (const std::exception &ex) {
    return {Status::UnknownError, ex.what(), {0, 0}};
  }
}
```

The only difference is the `indent` argument (`0` vs `jsonStringify->getIndent()`).

**Fix**: extract a private helper
`Result<void> JSON_Impl::runStringify(IDestination &, unsigned long indent) const`
containing the isEmpty guard and try/catch; both methods delegate to it.

---

### 2.4 `BufferSource` empty-input check repeated in four constructors

```cpp
// Four BufferSource constructors all end with the same throw:
if (this->buffer.empty()) { throw Error("Empty source buffer passed to be parsed."); }
if (ownedBuffer.empty())  { throw Error("Empty source buffer passed to be parsed."); }
if (buffer == nullptr || buffer[0] == '\0') { throw Error("Empty source buffer passed to be parsed."); }
if (buffer.empty())       { throw Error("Empty source buffer passed to be parsed."); }
if (buffer == nullptr || length == 0) { throw Error("Empty source buffer passed to be parsed."); }
```

**Fix**: add a private `static void checkNotEmpty(bool empty)` helper that throws the
error; each constructor calls it with its own emptiness predicate.

---

## 3. Refactor Roadmap

### Phase 1: `StringifierBase`

1. Add `classes/include/implementation/stringify/JSON_StringifierBase.hpp`:
   - Class `StringifierBase : public IStringify`
   - Constructor taking `std::unique_ptr<ITranslator>`, stored as `translator_`
   - Four `= delete` copy/move declarations + `~StringifierBase() override = default`
2. Inherit `Default_Stringify`, `Bencode_Stringify`, `XML_Stringify`, `YAML_Stringify`
   from `StringifierBase<XTranslator>` (or plain `StringifierBase`).
3. Remove the five duplicated special-member declarations from each concrete class.
4. Rename each class's private translator member to `translator_` (or access via base).

### Phase 2: `DestinationBase` deleted operators

1. Add the four `= delete` declarations and `~DestinationBase() override = default` to
   `DestinationBase`.
2. Remove the now-redundant five lines from `BufferDestination`, `FileDestination`,
   and `FixedBufferDestination`.

### Phase 3: `runStringify` helper in `JSON_Impl.cpp`

1. Add `private: Result<void> runStringify(IDestination &, unsigned long indent) const`
   to `JSON_Impl` (declaration in `JSON_Impl.hpp`, body in `JSON_Impl.cpp`).
2. Implement: empty-guard + try/catch.
3. Replace `stringifyResult` and `printResult` bodies with a one-liner each.

### Phase 4: `BufferSource::checkNotEmpty`

1. Add `static void checkNotEmpty(bool empty)` to `BufferSource`.
2. Replace the five inline throw expressions with `checkNotEmpty(condition)` calls.

### Phase 5: Validation

1. Run full test suite — must pass 1879 assertions in 80 test cases.
2. Verify embedded build (`-DJSON_LIB_EMBEDDED=ON`) compiles cleanly.
3. Run benchmark suite to confirm no performance regression.

---

## 4. Priority Order

| Priority | Phase | Finding | Impact | Risk |
|----------|-------|---------|--------|------|
| High     | 1     | 2.1     | ~28 lines removed across 4 files | Low |
| High     | 2     | 2.2     | ~15 lines removed across 3 files | Low |
| Medium   | 3     | 2.3     | ~14 lines removed in 1 file | Low |
| Low      | 4     | 2.4     | ~5 lines removed in 1 file | Low |
| Low      | 5     | —       | Validation only | None |

---

## 5. Files Affected

| File | Phase(s) |
|------|----------|
| `classes/include/implementation/stringify/JSON_StringifierBase.hpp` (NEW) | 1 |
| `classes/include/implementation/stringify/Default_Stringify.hpp` | 1 |
| `classes/include/implementation/stringify/Bencode_Stringify.hpp` | 1 |
| `classes/include/implementation/stringify/XML_Stringify.hpp` | 1 |
| `classes/include/implementation/stringify/YAML_Stringify.hpp` | 1 |
| `classes/include/implementation/io/JSON_DestinationBase.hpp` | 2 |
| `classes/include/implementation/io/JSON_BufferDestination.hpp` | 2 |
| `classes/include/implementation/io/JSON_FileDestination.hpp` | 2 |
| `classes/include/implementation/io/JSON_FixedBufferDestination.hpp` | 2 |
| `classes/include/implementation/JSON_Impl.hpp` | 3 |
| `classes/source/implementation/JSON_Impl.cpp` | 3 |
| `classes/include/implementation/io/JSON_BufferSource.hpp` | 4 |
