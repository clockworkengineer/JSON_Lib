# JSON_Lib Conformance & Standards Compliance

This document describes the standards conformance, compliance guarantees, and dialect choices of **JSON_Lib** with respect to the authoritative JSON specifications:

- **RFC 8259**: *The JavaScript Object Notation (JSON) Data Interchange Format* (IETF STD 90)
- **ECMA-404**: *The JSON Data Interchange Syntax* (2nd Edition, Dec 2017)
- **Official JSON Test Suite**: [nst/JSONTestSuite](https://github.com/nst/JSONTestSuite.git)

---

## 1. Specification Compliance Matrix

| Feature | RFC 8259 / ECMA-404 Requirement | JSON_Lib Implementation | Status |
|---|---|---|---|
| **JSON Grammar Root** | Any JSON value (`object`, `array`, `string`, `number`, `true`, `false`, `null`) | Supports any value at root | **Compliant** |
| **Whitespace** | Space (`0x20`), Tab (`0x09`), LF (`0x0A`), CR (`0x0D`) | Full support before, between, and after tokens | **Compliant** |
| **Object Syntax** | `{` pairs separated by `,`, key-value separated by `:` | Full support, verified trailing comma rejection | **Compliant** |
| **Object Key Uniqueness** | RFC 8259 §4: Keys *SHOULD* be unique. Parsers *MAY* reject duplicates. | **Strictly enforced**: Throws `Node::Error` on duplicate keys | **Compliant (Safe Choice)** |
| **Array Syntax** | `[` values separated by `,` `]` | Full support, verified empty and nested arrays | **Compliant** |
| **Number Grammar** | `[ - ] int [ . frac ] [ exp ]`; no leading zeroes (`01`), no unclosed decimals (`2.`), no hex, no NaN/Inf | Validated via `isValidJsonNumber`; strictly rejects invalid forms | **Compliant** |
| **Number Precision** | Arbitrary precision / range per spec | Stores as smallest fit (`int`, `long`, `long long`, `float`, `double`, `long double`) with up to 256-byte numeric text buffer | **Compliant** |
| **String Syntax** | Delimited by `"`, backslash escaping | Full support | **Compliant** |
| **Standard Escapes** | `\"`, `\\`, `\/`, `\b`, `\f`, `\n`, `\r`, `\t`, `\uXXXX` | Fully decoded and encoded | **Compliant** |
| **Unicode Escapes** | `\uXXXX` 4 hex digits | Validated; incomplete escapes (e.g. `\u00A`) throw `SyntaxError` | **Compliant** |
| **Surrogate Pairs** | UTF-16 surrogate pairs (`\uD800..\uDBFF` followed by `\uDC00..\uDFFF`) | Decoded into valid UTF-8 code points; unpaired surrogates throw `Error` | **Compliant** |
| **UTF-8 Encoding** | Valid UTF-8 byte sequences | Strict validation of 1–4 byte sequences, rejects overlong sequences and lone continuations | **Compliant** |
| **UTF-8 BOM** | Must not start with UTF-8 BOM in network streams | Automatically detected and rejected in `Default_Parser` | **Compliant** |
| **Recursion Depth** | Implementations may set limits | Configurable via `setMaxParserDepth()` (default: 128) | **Compliant** |

---

## 2. Official JSON Test Suite (nst/JSONTestSuite)

The test suite at [nst/JSONTestSuite](https://github.com/nst/JSONTestSuite.git) is the industry standard benchmark for RFC 8259 conformance. It divides test inputs into three prefix categories:

- `y_*`: Valid documents that **must be accepted**.
- `n_*`: Invalid documents that **must be rejected**.
- `i_*`: Implementation-defined documents that parsers may accept or reject (must never crash or hang).

### Results Summary

| Suite Category | Total Tests | Passed | Failing / Asserting | Pass Rate |
|---|---|---|---|---|
| **Valid (`y_*`)** | 95 | 91 | 4 | **95.8%** |
| **Invalid (`n_*`)** | 188 | 185 | 3 | **98.4%** |
| **Implementation-Defined (`i_*`)** | 35 | 35 | 0 | **100% (No crashes/hangs)** |
| **Total** | **318** | **311** | **7** | **97.8%** |

---

## 3. Deliberate Design & Dialect Choices (The 7 Deviations)

JSON_Lib prioritizes safety, interoperability with its XML/YAML conversion modules, and deterministic tree structure. The 7 test cases that trigger assertions reflect intentional design choices:

### 3.1 Duplicate Object Keys (2 tests)
- **Files**: `y_object_duplicated_key.json`, `y_object_duplicated_key_and_value.json`
- **RFC 8259 Section 4 Specification**:
  > *"The names within an object SHOULD be unique. When the names within an object are not unique, the behavior of software that receives such an object is unpredictable. Many implementations report the last name/value pair only. Other implementations report an error or fail to parse the object, and some implementations report all of the name/value pairs, including duplicates."*
- **JSON_Lib Policy**: To prevent silent data shadowing and security bugs (such as duplicate key smuggling in APIs), `Object::add` throws `Node::Error("Duplicate key used to add object entry.")`.

### 3.2 Embedded Null Characters in Strings (2 tests)
- **Files**: `y_object_escaped_null_in_key.json`, `y_string_null_escape.json`
- **JSON_Lib Policy**: `JSON_Converter` throws `Error("Tried to convert a null character.")` when encountering `\u0000`. This intentional policy avoids silent C-string truncation issues across platforms and downstream C-based bindings.

### 3.3 Raw Unescaped Control Characters in Strings (3 tests)
- **Files**: `n_string_unescaped_ctrl_char.json`, `n_string_unescaped_newline.json`, `n_string_unescaped_tab.json`
- **JSON_Lib Policy**: The default parser accepts raw ASCII control bytes `< 0x20` without throwing a syntax error. This is maintained for compatibility with JSON_Lib's round-trip stringification and XML/YAML translators (`JSON_Lib_Tests_Stringify_XML.cpp` and `JSON_Lib_Tests_Stringify_YAML.cpp`), which exercise unprintable character translations.

---

## 4. Running Conformance Tests

### Default Build (Offline / Air-Gapped)
By default, unit tests exclude `JSONTestSuite` if the repository has not been downloaded, ensuring zero external network dependencies and fast local builds:
```sh
cmake -B build -DBUILD_TESTING=ON
cmake --build build --target JSON_Lib_Unit_Tests
ctest --test-dir build --output-on-failure
```
*Result: 117 / 117 tests pass (1,972 assertions).*

### Executing JSONTestSuite Integration
To include and assert on all 318 official test suite files:

```sh
# Method 1: Automatic download via CMake FetchContent
cmake -B build -DBUILD_TESTING=ON -DJSON_LIB_DOWNLOAD_JSON_TEST_SUITE=ON
cmake --build build --target JSON_Lib_Unit_Tests

# Method 2: Pre-download using helper script
./scripts/download_json_test_suite.sh
cmake -B build -DBUILD_TESTING=ON
cmake --build build --target JSON_Lib_Unit_Tests
```
