//
// Program: JSON_Create_At_Runtime
//
// Description: Create JSON programmatically at runtime using a combination
// of the JSON class, indexing/key operators plus the use of initializer lists.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;
namespace fs = std::filesystem;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    // Initialise logging.
    init(plog::debug, "JSON_Create_At_Runtime.log");
    PLOG_INFO << "JSON_Create_At_Runtime started ...";
    // Log version
    PLOG_INFO << js::JSON().version();
    // create an empty structure (null)
    js::JSON json;
    // add a number stored as double (note the implicit conversion of
    // JSON to an object)
    json["pi"] = 3.141;
    // add a Boolean stored as bool
    json["sad"] = true;
    // add a string stored as std::string
    json["first_name"] = "Niels";
    // add another null object by passing a nullptr
    json["nothing"] = nullptr;
    // add an object inside the object
    json["the_answer"]["everything"] = 42;
    // add an array stored as std::vector (using an initializer list)
    json["list"] = { 1, 0, 2 };
    // add another object (using an initializer list of pairs)
    json["object"] = { { "currency", "USD" }, { "value", 42.99 } };
    // add another object that has a nested array (Node{})
    json["object2"] = { { "currency", "USD" }, { "array", js::Node{ 23.22, 33, 55, 99.99 } } };
    js::BufferDestination destination;
    json.stringify(destination);
    PLOG_INFO << destination.toString();
    destination.clear();
    // create JSON using an initializer list and nesting array/objects using Node{}.
    const js::JSON json2 = { { "pi", 3.141 },
      { "sad", true },
      { "first_name", "Niels" },
      { "nothing", nullptr },
      { "the_answer", js::Node{ { "everything", 42 } } },
      { "list", js::Node{ 1, 0, 2 } },
      { "object", js::Node{ { "currency", "USD" }, { "value", js::Node{ 23.22, 33, 55, 99.99 } } } } };
    json2.stringify(destination);
    PLOG_INFO << destination.toString();
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "JSON_Create_At_Runtime exited.";
  exit(EXIT_SUCCESS);
}