# JSON_Lib

**JSON_Lib** is a C++20-based library designed to simplify interactions with JSON data. With a focus on providing a clean and intuitive API, the library enables developers to parse, manipulate, and serialize JSON data with ease and efficiency.

---
# JSON_Lib

**JSON_Lib** is a modern, high-performance C++23 library for parsing, manipulating, and serializing JSON data. It provides a clean, intuitive API and is designed for efficiency, extensibility, and standards compliance.

---

## Features

- Modern, lightweight, and header-oriented JSON handling
- Fully compatible with C++23 features (concepts, `std::string_view`, etc.)
- Highly customizable and efficient, designed for performance
- Support for parsing, creating, and serializing JSON objects and arrays
- Extensive error management and exception-based error handling
- Unicode and multi-encoding support (UTF-8, UTF-16LE/BE)
- Extensive examples and compliance documentation

---

## Getting Started

### Prerequisites

- **C++ compiler** with C++23 support (e.g., GCC 13+, Clang 16+, MSVC 19.36+)
- **CMake** (version 3.23 or above)

### Installation

1. Clone the repository:
   ```sh
   git clone <repository-url>
   cd JSON_Lib
   ```
2. Configure and build with CMake:
   ```sh
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```
3. Link the library in your project:
   ```cmake
   target_link_libraries(<your-target> PRIVATE JSON_Lib)
   ```

---

## Usage

### Basic Example

```cpp
#include "JSON.hpp"
    // Access JSON data
    std::string name = json["name"].as_string();
    int age = json["age"].as_int();
    bool isAdmin = json["is_admin"].as_bool();

    std::cout << "Name: " << name << ", Age: " << age << ", Admin: " << (isAdmin ? "Yes" : "No") << "\n";

    return 0;
}
```

### Creating JSON

```cpp
#include "JSON.hpp"
int main() {
   JsonLib::Json json;
   json["name"] = "Jane";
   json["age"] = 25;
   json["is_admin"] = false;
   std::string serialized_json = json.dump();
   std::cout << serialized_json << "\n";
   return 0;
}
```

---

## Documentation & Examples

- **API Reference:** See `docs/api.md` for class and function details.
- **User Guide:** See `docs/guide.md` for installation, usage, and advanced features.
- **Compliance Report:** See `docs/JSON_Lib_Compliance_Report.md` for standard compliance and deviations.
- **Examples:** Browse the `examples/source/` directory for real-world usage and advanced scenarios (file conversion, schema validation, custom serialization, etc.).

---

## Compliance & Deviations

JSON_Lib is largely compliant with RFC 8259/ECMA-404, with a few stricter behaviors:
- Strict BOM/file format handling (errors on unsupported BOMs)
- CRLF normalization to LF
- C++ type support for numbers (may differ from standard for large numbers)
- Strict Unicode error handling
See the compliance report for details.

---

## Contributing

Contributions are welcome! Please:
- Follow the code style and formatting guidelines
- Add relevant test cases for new features
- Open an issue for significant changes before submitting a pull request

---

## License

This project is licensed under the **MIT License**. See [LICENSE.txt](LICENSE.txt) for details.

---

## Contact

For bugs, questions, or suggestions, please file an issue in the repository.