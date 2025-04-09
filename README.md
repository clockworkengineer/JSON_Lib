# JSON_Lib

**JSON_Lib** is a lightweight, flexible, and efficient library for parsing, manipulating, and serializing JSON data in C++. It adheres to the JSON standard as specified in [RFC 8259](https://tools.ietf.org/html/rfc8259). The library allows easy creation and manipulation of JSON objects, offering robust features for developers working with JSON data.

---

## Features

- **JSON Parsing**: Convert JSON text into an internal JSON object while validating its syntax. Robust exceptions are thrown when invalid JSON is encountered.
- **JSON Serialization**: Convert JSON objects back into a text form with options for compact or well-formatted (indented) output.
- **Custom I/O Sources**: Support for parsing from or serializing to:
  - Byte buffers
  - Files
  - Custom sources or destinations through `ISource` and `IDestination` interfaces.
- **Tree Traversal**: Easily traverse the JSON object tree and perform actions through the `IAction` interface. Enables reading or modifying nodes during traversal.
- **Indentation Support**: Options for pretty-printing JSON with defined indentation levels using the `print` and `setIndent()` methods.

---

## Installation

1. Clone the repository:
   ```bash
   git clone <repository_link>
   ```
2. Install the library using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   sudo make install
   ```

---

## Basic Usage

### Parsing JSON
```cpp
#include "JSON_Lib/JSON.h"

// Parse JSON string into an internal JSON object
std::string jsonStr = R"({"key": "value"})";
JSON jsonObject = JSON::parse(jsonStr);
```

### Stringify JSON
```cpp
// Convert JSON object back to a compact string
std::string jsonString = jsonObject.stringify();
```

### Pretty Print
```cpp
jsonObject.setIndent(4); // Set indentation to 4 spaces
jsonObject.print(std::cout); // Print the formatted output
```

### Traversing a JSON Tree
Use the `IAction` interface to traverse the JSON tree and apply actions on each node.

---

## Examples
- The `examples` folder contains practical programs demonstrating:
    - Parsing JSON
    - Modifying JSON objects
    - Serializing JSON with various options

To explore these examples, navigate to the `examples` directory and build the sample programs.

---

## Testing
The library includes unit tests to ensure robustness. Use the following to build and run tests:
```bash
mkdir build && cd build
cmake ..
make test
```

---

## Documentation
Refer to the code comments and examples for in-depth details on how to use the library. For the full JSON specification, visit [RFC 8259](https://tools.ietf.org/html/rfc8259).

---

## Contributing
Contributions in the form of bug reports, feature requests, or pull requests are welcome! Please follow the standard GitHub workflow:

1. Fork the repository.
2. Create a new branch for your feature or fix.
3. Submit a pull request.

---

## License
This project is licensed under the **MIT License**. See the `LICENSE` file for more details.

---

## Contact
For support, feedback, or further information, feel free to reach out via the issue tracker or contribute to the discussions section.
```
