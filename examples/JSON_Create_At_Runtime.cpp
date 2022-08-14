//
// Program: JSON_Create_At_Runtime
//
// Description
// Dependencies: C20++, PLOG, JSONLib.
//
// =============
// INCLUDE FILES
// =============
// =======
// C++ STL
// =======
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Types.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
// =======
// Logging
// =======
#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"
// ==========
// NAMESPACES
// ==========
namespace json = JSONLib;
namespace fs = std::filesystem;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
// ===============
// LOCAL FUNCTIONS
// ===============
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "JSON_Create_At_Runtime.log");
    PLOG_INFO << "JSON_Create_At_Runtime started ...";
    // Log version
    PLOG_INFO << json::JSON().version();
    // create an empty structure (null)
    json::JSON json;
    // add a number that is stored as double (note the implicit conversion of j
    // to an object)
    json["pi"] = 3.141;
    // add a Boolean that is stored as bool
    json["sad"] = true;
    // add a string that is stored as std::string
    json["first_name"] = "Niels";
    // add another null object by passing nullptr
    json["nothing"] = nullptr;
    // add an object inside the object
    json["the_answer"]["everything"] = 42;
    // add an array that is stored as std::vector (using an initializer list)
    json["list"] = {1, 0, 2, 4, 6};
    // add another object (using an initializer list of pairs)
    json["object"] = {{"currency", "USD"}, {"value", 42.99}};
    json::BufferDestination destination;
    json.stringify(destination);
    PLOG_INFO << destination.getBuffer();
    // instead, you could also write (which looks very similar to the JSON
    // above) 
    // NOT SUPPORTED YET AS I CANNOT GET TO COMPILE.
    // json json2 = {{"pi", 3.141},
    //               {"sad", true},
    //               {"first_name", "Niels"},
    //               {"nothing", nullptr},
    //               {"the_answer", {{"everything", 42}}},
    //               {"list", {1, 0, 2, 4, 6}},
    //               {"object", {{"currency", "USD"}, {"value", 42.99}}}};
  } catch (std::exception &e) {
    PLOG_ERROR << "Error: " << e.what();
  }
  PLOG_INFO << "JSON_Create_At_Runtime exited.";
  exit(EXIT_SUCCESS);
}