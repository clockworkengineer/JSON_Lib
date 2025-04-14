# JSON_Lib

**JSON_Lib** is a C++20-based library designed to simplify interactions with JSON data. With a focus on providing a clean and intuitive API, the library enables developers to parse, manipulate, and serialize JSON data with ease and efficiency.

---

## Features

- Modern and lightweight JSON handling.
- Fully compatible with C++20 features (e.g., concepts, `std::string_view`, etc.).
- Highly customizable and efficient, designed for performance.
- Support for parsing, creating, and serializing JSON objects and arrays.
- Extensive error management for JSON parsing and handling.

---

## Getting Started

### Prerequisites

Before using `JSON_Lib`, ensure you have the following installed:

- **C++ compiler** with C++20 support (e.g., GCC 10+, Clang 10+, MSVC 19.26+).
- **CMake** (version 3.15 or above) for build configuration.

### Installation

1. Clone the repository to your local machine:

   ```bash
   git clone <repository-url>
   cd JSON_Lib
   ```

2. Configure and build the project using CMake:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Include the library in your project by linking it during compilation:

   ```cmake
   target_link_libraries(<your-project-name> PRIVATE JSON_Lib)
   ```

---

## Usage

### Basic Example

Here's a simple example of how to use the library to parse a JSON string:

```cpp
#include "json_lib.h"

int main() {
    std::string json_string = R"({"name": "John", "age": 30, "is_admin": true})";
    JsonLib::Json json = JsonLib::Json::parse(json_string);

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
#include "json_lib.h"

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

## Documentation

For detailed API documentation, refer to the **`docs/`** folder in this repository.

You can also explore the `json_lib.h` header file for insights into the API design.

---

## Contributing

We welcome contributions to enhance the library! Feel free to fork the repository and submit a pull request. Before submitting, ensure that:

- The code follows consistent formatting and style guides.
- You have included relevant test cases for any new functionality added.

For significant changes, please open an issue first to discuss your ideas.

---

## License

This project is available under the **MIT License**. See the [LICENSE](LICENSE) file for more details.

---

## Acknowledgments

Special thanks to all contributors and the open-source community for inspiration and feedback!

---

## Contact

If you encounter any bugs or have questions, please feel free to file an issue in the repository.