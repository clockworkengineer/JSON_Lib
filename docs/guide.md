# JSON_Lib User Guide

This guide provides step-by-step instructions for installing, using, and extending the JSON_Lib library.

## Table of Contents
- [Getting Started](#getting-started)
- [Basic Usage](#basic-usage)
- [Advanced Features](#advanced-features)
- [Examples](#examples)
- [Troubleshooting](#troubleshooting)

---

## Getting Started
1. **Installation**: Build the library using CMake:
  ```sh
  mkdir build && cd build
  cmake ..
  cmake --build .
  ```
  This will generate the static or shared library in the `build/` directory.

2. **Include in Your Project**:
  - Add the `include/` directory to your compiler's include path.
  - Link against the built library (e.g., `JSON_Lib.lib` or `libJSON_Lib.a`).
  - Ensure your project is set to use C++17 or later.

3. **Dependencies**:
  - Requires a C++17 (or later) compatible compiler.
  - Uses [PLOG](https://github.com/SergiusTheBest/plog) for logging (included in examples).

## Basic Usage

### Parsing a JSON String
```cpp
#include "json_lib.h"
std::string json_string = R"({\"name\": \"John\", \"age\": 30})";
JsonLib::Json json = JsonLib::Json::parse(json_string);
```

### Accessing Data
```cpp
std::string name = json["name"].as_string();
int age = json["age"].as_int();
```

### Serializing JSON
```cpp
std::string jsonStr = json.stringify();
```

### Parsing from and Writing to Files
```cpp
#include "JSON_Utility.hpp"
namespace js = JSON_Lib;
js::JSON json;
json.parse(js::FileSource{"input.json"});
json.stringify(js::FileDestination{"output.json"});
```

## Advanced Features

- **Custom Struct Serialization**:
  Serialize and deserialize C++ structs to/from JSON:
  ```cpp
  struct Person {
    std::string name;
    int age;
  };
  Person p{"Alice", 30};
  js::JSON json;
  json["name"] = p.name;
  json["age"] = p.age;
  json.stringify(js::FileDestination{"person.json"});
  // Deserialize
  js::JSON jsonIn;
  jsonIn.parse(js::FileSource{"person.json"});
  Person p2;
  p2.name = js::NRef<js::String>(jsonIn["name"]).value();
  p2.age = js::NRef<js::Number>(jsonIn["age"]).value<int>();
  ```

- **Error Handling**:
  Use try-catch blocks to handle parsing and type errors:
  ```cpp
  try {
    js::JSON json;
    json.parse(js::FileSource{"input.json"});
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
  }
  ```

- **Integration**:
  See `examples/JSON_HTTP_Integration.cpp` for fetching and processing JSON from web APIs (requires an external HTTP library).

- **Schema Validation**:
  See `examples/JSON_Schema_Validation.cpp` for loading and preparing to validate JSON against a schema (validation logic to be implemented).

## Examples

The `examples/` directory contains:
- **JSON_Parse_File.cpp**: Parse and stringify JSON files, measure performance.
- **JSON_Custom_Struct_Serialization.cpp**: Serialize/deserialize C++ structs.
- **JSON_HTTP_Integration.cpp**: Fetch and process JSON from web APIs.
- **JSON_Schema_Validation.cpp**: Load JSON schema and data files.
- **JSON_Analyzer.hpp**: Analyze JSON structure using custom actions.
- **JSON_Convert.hpp**: Convert JSON types using custom actions.

## Troubleshooting

- **Compiler Errors**: Ensure your compiler supports C++17 or later.
- **Linker Errors**: Verify the library is built and linked correctly.
- **Parsing Errors**: Use try-catch blocks and check input file validity.
- **API Questions**: Check the [API Reference](api.md) for class and function details.
- **Testing**: Review the `tests/` directory for usage patterns and additional examples.
- **Logging**: Example programs use PLOG for logging; check log files for diagnostics.

---

For API details, see the [API Reference](api.md).
