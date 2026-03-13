// Program: JSON_Custom_Struct_Serialization.cpp
//
// Description: Serialize and deserialize a custom C++ struct to/from JSON.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

struct Person {
  std::string name;
  int age;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    init(plog::debug, "JSON_Custom_Struct_Serialization.log");
    PLOG_INFO << "JSON_Custom_Struct_Serialization started ...";
    PLOG_INFO << js::JSON().version();
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
    PLOG_INFO << "Deserialized: " << p2.name << ", " << p2.age;
    return 0;
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
    return 1;
  }
}
