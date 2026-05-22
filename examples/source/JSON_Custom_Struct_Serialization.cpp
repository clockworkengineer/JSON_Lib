// Program: JSON_Custom_Struct_Serialization.cpp
//
// Description: Serialize and deserialize a custom C++ struct to/from JSON.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

struct Person {
  std::string name;
  int age;
};

int main(int, char **)
{
  try {
        std::cout << "JSON_Custom_Struct_Serialization started ..." << '\n';

    std::cout << js::JSON().version() << '\n';

    // Serialize
    Person p{"Alice", 30};
    js::JSON json;
    json["name"] = p.name;
    json["age"] = p.age;
    json.stringify(js::FileDestination{"files/person.json"});
    // Deserialize
    js::JSON jsonIn;
    jsonIn.parse(js::FileSource{"files/person.json"});
    Person p2;
    p2.name = js::NRef<js::String>(jsonIn["name"]).value();
    p2.age = js::NRef<js::Number>(jsonIn["age"]).value<int>();
    std::cout << "Deserialized: " << p2.name << ", " << p2.age << '\n';

    return 0;
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';

    return 1;
  }
}
