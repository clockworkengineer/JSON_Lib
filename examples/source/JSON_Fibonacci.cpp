//
// Program: JSON_Fibonacci
//
// Description: On each activation add the next entry in the fibonacci
// sequence to the array stored in fibonacci.json; if the file does
// not exist, then create the initial sequence of [0,1].
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

/// <summary>
/// Return Fibonacci json file name.
/// </summary>
/// <returns>JSON settings file name.</returns>
std::string jsonFibonacciFile() { return (std::filesystem::current_path() / "files" / "fibonacci.json").string(); }
/// <summary>
/// Read in the current fibonacci sequence from JSON file, calculate the
/// next in sequence and write back to JSON file.
/// </summary>
void nextFibonacci()
{
  js::JSON json;
  if (!std::filesystem::exists(jsonFibonacciFile())) {
    // If JSON file does not exist, create the initial sequence
    json.parse(js::BufferSource{ "[0, 1]" });
  } else {
    // Parse in current sequence
    json.parse(js::FileSource{ jsonFibonacciFile() });
    // Get index of last element
    const auto last = js::NRef<js::Array>(json.root()).size() - 1;
    // Next is the sum of the last two entries
    auto next = js::NRef<js::Number>(json[last]).value<int>();
    next += js::NRef<js::Number>(json[last - 1]).value<int>();
    // Expand the array by one and add next in sequence
    json[last + 1] = next;
  }
  // Write the updated sequence back to file
  json.stringify(js::FileDestination{ jsonFibonacciFile() });
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    // Initialise logging.
    init(plog::debug, "JSON_Fibonacci.log");
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