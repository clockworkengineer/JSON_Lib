//
// Program: JSON_Fibonacci
//
// Description: On each activation add the next entry in the fibonacci
// sequence to the array stored in fibonacci.json; if the file does
// not exist then create the initial sequence of [0,1].
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "Utility.hpp"

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace js = JSON_Lib;

/// <summary>
/// Return Fibonaci json file name.
/// </summary>
/// <returns>JSON settings file name.</returns>
std::string jsonFibonacciFile() { return ((std::filesystem::current_path() / "files" / "fibonacci.json").string()); }
/// <summary>
/// Read in current fibonacci sequence from JSON file, calculate the
//  next in sequence and write back to JSON file.
/// </summary>
void nextFibonacci()
{
  js::JSON json;
  if (!std::filesystem::exists(jsonFibonacciFile())) {
    // If JSON file does not exist create intial sequence
    json.parse(js::BufferSource{ "[0, 1]" });
  } else {
    // Parse in current sequence
    json.parse(js::FileSource{ jsonFibonacciFile() });
    // Get index of last element
    auto last = js::JRef<js::Array>(json.root()).size() - 1;
    // Next is sum of last two entries
    auto next = js::JRef<js::Number>(json[last]).get<int>();
    next += js::JRef<js::Number>(json[last - 1]).get<int>();
    // Expand array by one and add next in sequence
    json[last + 1] = next;
  }
  // Write updated sequence back to file
  json.stringify(js::FileDestination{ jsonFibonacciFile() });
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    // Initialise logging.
    plog::init(plog::debug, "JSON_Fibonacci.log");
    PLOG_INFO << "JSON_Fibonacci started ...";
    // Log version
    PLOG_INFO << js::JSON().version();
    // Update current sequence
    nextFibonacci();
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "JSON_Fibonacci exited.";
  exit(EXIT_SUCCESS);
}