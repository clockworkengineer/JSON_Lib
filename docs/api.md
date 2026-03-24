# JSON_Lib API Reference

This document provides an overview of the public API for the JSON_Lib library, including class descriptions, function signatures, and usage notes.

## Table of Contents
- [Overview](#overview)
- [Core Classes](#core-classes)
- [Key Functions](#key-functions)
- [Error Handling](#error-handling)
- [Examples](#examples)

---

## Overview
JSON_Lib is a C++ library for parsing, manipulating, and serializing JSON data. It is designed for performance, compliance, and ease of use.

## Core Classes

- **JSON**: Main class for parsing, manipulating, and serializing JSON data. Provides methods for parsing from strings, files, and buffers, as well as serializing to strings and files.
- **Node**: Represents a node in the JSON tree (object, array, string, number, boolean, or null). Used for type-safe access and manipulation.
- **Object**: Represents a JSON object (key-value pairs). Provides methods for iterating and accessing members.
- **Array**: Represents a JSON array (ordered list of values). Provides methods for iterating and modifying elements.
- **IAction**: Interface for custom actions during traversal (see examples for analyzers and converters).

## Key Functions

- `JSON::parse(const std::string& jsonStr)`: Parses a JSON string and returns a JSON object.
- `JSON::parse(FileSource)`: Parses JSON from a file.
- `JSON::stringify(IDestination) const`: Serializes the JSON object to a destination (file, buffer, etc.).
- `JSON::version() const`: Returns the library version string.
- `Node::as_string()`, `Node::as_int()`, `Node::as_bool()`: Type-safe accessors for node values.
- `Object::operator[]`: Access object members by key.
- `Array::push_back(const Node&)`: Append a value to an array.
- `IAction::onNode`, `onString`, `onNumber`, etc.: Custom traversal hooks for advanced processing.

## Error Handling
JSON_Lib uses exception-based error handling. Most parsing and type errors throw `std::exception` or custom error types. Always use try-catch blocks:

```cpp
try {
	js::JSON json;
	json.parse(js::FileSource{"input.json"});
} catch (const std::exception& ex) {
	std::cerr << "Error: " << ex.what() << std::endl;
}
```

See also the `examples/` directory for error handling patterns.


## Examples

### Basic Parsing
```cpp
#include "json_lib.h"

int main() {
		std::string json_string = R"({\"name\": \"John\", \"age\": 30, \"is_admin\": true})";
		JsonLib::Json json = JsonLib::Json::parse(json_string);
		std::string name = json["name"].as_string();
		int age = json["age"].as_int();
		bool isAdmin = json["is_admin"].as_bool();
		std::cout << "Name: " << name << ", Age: " << age << ", Admin: " << (isAdmin ? "Yes" : "No") << "\n";
}
```

### Creating and Serializing JSON
```cpp
#include "json_lib.h"

JsonLib::Json json;
json["name"] = "Alice";
json["age"] = 30;
std::string out = json.stringify();
```

### File Parsing and Stringifying
```cpp
#include "JSON_Utility.hpp"
namespace js = JSON_Lib;
js::JSON json;
json.parse(js::FileSource{"input.json"});
json.stringify(js::FileDestination{"output.json"});
```

### Custom Struct Serialization
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

### HTTP Integration (Pseudo-code)
```cpp
// Fetch JSON from API (requires external HTTP library)
std::string jsonResponse = R"({\"message\":\"Hello from API!\"})";
js::JSON json;
json.parse(js::BufferSource{jsonResponse});
std::cout << js::NRef<js::String>(json["message"]).value();
```

---

For more details, refer to the [Guide](guide.md) or the source code documentation.

---

For more details, refer to the [Guide](guide.md) or the source code documentation.
