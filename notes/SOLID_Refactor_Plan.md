# Comprehensive SOLID Refactoring Plan for JSON_Lib

## 1. Executive Summary & Objectives

The `JSON_Lib` codebase is a C++20 JSON processing library providing parsing, tree construction, stringification (JSON, XML, YAML, Bencode), traversal, and file I/O. While functional and backed by comprehensive unit tests, an architectural audit reveals notable violations of the **SOLID** design principles.

This refactoring plan provides a concrete, step-by-step roadmap to eliminate these violations, enhance modularity, improve testability, decouple component dependencies, and enforce clear separation of concerns across the library without introducing breaking changes to the public `JSON` API where possible.

---

## 2. Analysis of SOLID Violations

### 2.1 Single Responsibility Principle (SRP)
*A class should have one, and only one, reason to change.*

* **God Class in `JSON` & `JSON_Impl`** (`classes/include/JSON.hpp`, `classes/source/implementation/JSON_Impl.cpp`):
  - `JSON_Impl` currently handles:
    1. Node tree root management and array/object mutation (`operator[]`, `resize`).
    2. Lifecycle management of parsers, stringifiers, and translators.
    3. Traversal execution (`traverse`, `traverseNodes`).
    4. Text processing (`strip`).
    5. Disk/File I/O (`fromFile`, `toFile`, `getFileFormat`).
  - **Impact**: Any change to file formats, whitespace stripping rules, or tree indexing logic requires altering `JSON_Impl`.
* **Stream & State Overload in `ISource`** (`classes/include/interface/ISource.hpp`):
  - Combines character stream navigation (`current()`, `next()`, `more()`), state tracking (`lineNo`, `column`), pattern matching (`match()`), and whitespace filtering (`isWS()`, `ignoreWS()`).
* **Configuration State in `IParser` and `IStringify`** (`IParser.hpp`, `IStringify.hpp`):
  - `IParser` mixes parsing strategy with recursion depth state (`setMaxParserDepth`, `getMaxParserDepth`).
  - `IStringify` mixes serialization with indentation state (`setIndent`, `getIndent`), which is irrelevant for non-indented formats like Bencode.

### 2.2 Open/Closed Principle (OCP)
*Software entities should be open for extension, but closed for modification.*

* **File Format Encoding Handler** (`classes/source/implementation/file/JSON_File.cpp`):
  - File reading and writing uses rigid `switch` statements over `JSON::Format` enum values (`utf8`, `utf8BOM`, `utf16BE`, `utf16LE`).
  - **Impact**: Supporting new encodings (e.g. UTF-32BE, UTF-32LE) requires modifying `JSON_File.cpp` source code and adding cases to existing methods.
* **Monolithic `Default_Parser`** (`classes/include/implementation/parser/Default_Parser.hpp`):
  - Parsing rules for Objects, Arrays, Strings, Numbers, Booleans, and Nulls are hardcoded within `Default_Parser`. Customizing individual grammar token parsing requires modifying or replacing the entire parser.

### 2.3 Liskov Substitution Principle (LSP)
*Subtypes must be substitutable for their base types.*

* **Formatting Inconsistency in `IStringify` Derived Classes** (`XML_Stringify.hpp`, `YAML_Stringify.hpp`, `Bencode_Stringify.hpp`):
  - Calling `JSON::setIndent()` delegates to `IStringify::setIndent()`. Non-JSON stringifiers inherit a default no-op from `IStringify`, creating silent divergence in API behavior.
* **Data Loss Bug in `XML_Stringify` Array Handling** (`XML_Stringify.hpp` Line 66):
  - `XML_Stringify::stringifyArray` checks `if (NRef<Array>(jNode).value().size() > 1)`. An array with 0 or 1 element is completely skipped, producing invalid/incomplete output and breaking behavioral substitutability.
* **Subclassing `EmbeddedJSON` from `JSON`** (`JSON.hpp` Line 173):
  - `EmbeddedJSON` inherits from `JSON` but alters build-flag assumptions (`JSON_LIB_NO_STDIO`). If stdio is disabled, base class methods like `fromFile` fail or are omitted conditionally, violating LSP contract expectations.

### 2.4 Interface Segregation Principle (ISP)
*Clients should not be forced to depend on methods they do not use.*

* **Fat `IAction` Interface** (`classes/include/interface/IAction.hpp`):
  - `IAction` defines 14 virtual callback methods for visiting nodes (`onNode`, `onString`, `onNumber`, `onBoolean`, `onNull`, `onArray`, `onObject` in mutable and const overloads).
  - **Impact**: Observers or transformers that only care about a single node type must inherit from a bloated interface.
* **`IDestination` Interface Overload** (`classes/include/interface/IDestination.hpp`):
  - Demands that all output sinks support `add(const std::string&)`, `add(const char*)`, `add(std::string_view)`, `add(char)`, `clear()`, and `last()`.
  - **Impact**: `last()` forces streaming sinks (e.g. network sockets or unbuffered file writers) to maintain state unnecessarily.

### 2.5 Dependency Inversion Principle (DIP)
*High-level modules should not depend on low-level modules. Both should depend on abstractions.*

* **Hardcoded Dependencies in `JSON_Impl` Constructor** (`JSON_Impl.cpp` Line 17):
  - `JSON_Impl` directly instantiates `std::make_unique<Default_Translator>()` and `Default_Parser` / `Default_Stringify` rather than depending on injected abstractions or factories.
* **Deprecated Conversion Utilities in `JSON_Converter.cpp`**:
  - Direct reliance on `std::wstring_convert` and `<codecvt>` (deprecated in C++17/C++20) inside platform folders without an abstract `ICharConverter` interface.

---

## 3. Proposed Refactoring Architecture & Target State

```
                      +-------------------+
                      |   JSON (Facade)   |
                      +---------+---------+
                                |
             +------------------+------------------+
             |                                     |
+------------v------------+              +---------v---------+
|      JSON_Document      |              |   JSON_FileIO     |
| (Tree Holder & Mutator) |              |  (File & Encodings|
+------------+------------+              +---------+---------+
             |                                     |
             +------------------+                  |
                                |                  v
                      +---------v---------+ +------+------------+
                      |   JSON_Services   | | IEncodingHandler  |
                      |  (Parser/String)  | +-------------------+
                      +---------+---------+
                                |
        +-----------------------+-----------------------+
        |                       |                       |
+-------v-------+       +-------v-------+       +-------v-------+
|    IParser    |       |  IStringify   |       |  ITranslator  |
+---------------+       +---------------+       +---------------+
```

---

## 4. Concrete Implementation Steps

### Phase 1: Decouple File I/O & Encodings (SRP & OCP Fixes)
1. **Create `IEncodingHandler` Interface**:
   - Define `classes/include/interface/IEncodingHandler.hpp` with methods `read()` and `write()`.
   - Implement concrete handlers in `classes/include/implementation/io/encoding/`:
     - `Utf8EncodingHandler`
     - `Utf8BOMEncodingHandler`
     - `Utf16BEEncodingHandler`
     - `Utf16LEEncodingHandler`
2. **Extract `JSON_FileIO` Service**:
   - Create `classes/include/implementation/file/JSON_FileIO.hpp` and `.cpp`.
   - Move static methods `fromFile`, `toFile`, `getFileFormat` out of `JSON_Impl` and `JSON_File.cpp` into `JSON_FileIO`.
   - Update `JSON::fromFile` and `JSON::toFile` to delegate to `JSON_FileIO`.

### Phase 2: Interface Segregation & LSP Fixes (ISP & LSP Fixes)
1. **Segregate `IAction` Traversal Callbacks**:
   - Retain `IAction` as a composite interface for backwards compatibility.
   - Introduce fine-grained visitor interfaces:
     - `INodeVisitor` (`onNode`)
     - `IValueVisitor` (`onString`, `onNumber`, `onBoolean`, `onNull`)
     - `IContainerVisitor` (`onObject`, `onArray`)
   - Create `ActionAdapter` base class providing default no-op implementations.
2. **Refactor `IDestination`**:
   - Split into `IByteWriter` (pure output stream: `add(string_view)`) and `IStatefulDestination` (adds `clear()` and `last()`).
3. **Fix `XML_Stringify` Bug**:
   - Remove the `size() > 1` condition in `XML_Stringify::stringifyArray` so single-element and empty arrays are correctly converted to XML tags (`<Row>...</Row>`).

### Phase 3: Dependency Inversion & Parser/Stringifier Modularization (DIP & SRP Fixes)
1. **Inject Translator Dependency**:
   - Modify `JSON_Impl` to accept `std::shared_ptr<ITranslator>` or `std::unique_ptr<ITranslator>` explicitly rather than hardcoding `Default_Translator`.
2. **Decouple Parser Configuration**:
   - Extract `ParserOptions` struct containing `maxDepth` and pass it into `IParser` constructors instead of polluting `IParser` with stateful setters.
3. **Modernize Character Converter**:
   - Replace `<codecvt>` usage in `JSON_Converter.cpp` with a clean UTF conversion utility class (`ICharConverter` abstraction).

### Phase 4: Composition over Inheritance for `EmbeddedJSON`
1. **Refactor `EmbeddedJSON`**:
   - Use composition or static policy traits for `EmbeddedJSON` instead of inheriting from `JSON`.

---

## 5. Verification Plan

### Automated Testing
- **Unit Tests**: Execute Catch2 test suite `/home/robt/projects/JSON_Lib/build/tests/JSON_Lib_Unit_Tests` to ensure 100% regression pass (all 1966 assertions across 112 test cases).
- **New Unit Tests**:
  - Add test cases for `XML_Stringify` with single-element arrays.
  - Add test cases for custom `IEncodingHandler` extensions.
  - Add test cases for segregated `INodeVisitor` implementations.

### Manual Verification
- Build library across all CMake build options (`JSON_LIB_NO_STDIO`, `JSON_LIB_NO_EXCEPTIONS`, `JSON_LIB_EMBEDDED`).
