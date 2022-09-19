//
// Program: JSON_Fibonacci
//
// Description: On each activation add the next entry in the fibonacci
// sequence to the array stored in fibonacci.json; if the file does
// not exist then create the initial sequence of [0,1].
//
// Dependencies: C20++, PLOG, JSON_Lib.
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
namespace json = JSON_Lib;
namespace fs = std::filesystem;
// =======================
// LOCAL TYPES/DEFINITIONS
// =======================
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Return Fibonaci json file name.
/// </summary>
/// <returns>JSON settings file name.</returns>
std::string jsonFibonacciFile() { return ((fs::current_path() / "files" / "fibonacci.json").string()); }
/// <summary>
/// Read in current fibonacci sequence from JSON file, calculate the
//  next in sequence and write back to JSON file.
/// </summary>
void nextFibonacci()
{
  json::JSON json;
  if (!fs::exists(jsonFibonacciFile())) {
    // If JSON file does not exist create intial sequence
    json.parse(json::BufferSource{ "[0, 1]" });
  } else {
    // Parse in current sequence
    json.parse(json::FileSource{ jsonFibonacciFile() });
    // Get index of last element
    auto last = json::JRef<json::Array>(json.root()).size() - 1;
    // Next is sum of last two entries
    auto next = json::JRef<json::Number>(json[last]).get<int>();
    next += json::JRef<json::Number>(json[last - 1]).get<int>();
    // Expand array by one and add next in sequence
    json[last + 1] = next;
  }
  // Write updated sequence back to file
  json.stringify(json::FileDestination{ jsonFibonacciFile() });
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    // Initialise logging.
    plog::init(plog::debug, "JSON_Fibonacci.log");
    PLOG_INFO << "JSON_Fibonacci started ...";
    // Log version
    PLOG_INFO << json::JSON().version();
    // Update current sequence
    nextFibonacci();
  } catch (std::exception &e) {
    PLOG_ERROR << "Error: " << e.what();
  }
  PLOG_INFO << "JSON_Fibonacci exited.";
  exit(EXIT_SUCCESS);
}