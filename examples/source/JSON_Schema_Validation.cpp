// Program: JSON_Schema_Validation.cpp
//
// Description: Validate JSON files against a schema.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include <iostream>
// #include "JSON_Schema.hpp" // Uncomment if schema support exists

namespace js = JSON_Lib;

int main(int, char **)
{
  try {
        std::cout << "JSON_Schema_Validation started ..." << '\n';

    std::cout << js::JSON().version() << '\n';

    // Example: Load schema and JSON file
    std::string schemaFile = "files/schema.json";
    std::string jsonFile = "files/data.json";
    js::JSON schema, json;
    schema.parse(js::FileSource{ schemaFile });
    json.parse(js::FileSource{ jsonFile });
    // TODO: Add schema validation logic here
    std::cout << "Schema and JSON loaded. (Validation logic not implemented)" << '\n';

    return 0;
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';

    return 1;
  }
}
