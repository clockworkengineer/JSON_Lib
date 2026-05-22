//
// Program: JSON_Parse_File
//
// Description: For each JSON file in a directory parse it, stringify
// it back into text form and then parse the buffer created; timing each
// step in turn for each file.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;
namespace chrono = std::chrono;

static constexpr size_t kMaxFileLengthToDisplay = 16 * 1024;

/// <summary>
/// Parse JSON file, stringify and parse JSON from buffer whilst timing each
/// step.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  auto elapsedTime = [](const auto &start, const auto &stop) {
    return chrono::duration_cast<chrono::microseconds>(stop - start).count();
  };
  std::cout << "Processing " << fileName << '\n';

  js::JSON json;
  js::BufferDestination jsonDestination;
  // Parse from file
  auto start = chrono::high_resolution_clock::now();
  json.parse(js::FileSource{ fileName });
  auto stop = chrono::high_resolution_clock::now();
  std::cout << elapsedTime(start, stop) << " microseconds to parse from file." << '\n';

  // Stringify to file
  start = chrono::high_resolution_clock::now();
  json.stringify(js::FileDestination{ fileName + ".new" });
  stop = chrono::high_resolution_clock::now();
  std::cout << elapsedTime(start, stop) << " microseconds to stringify to file." << '\n';

  // Stringify to buffer
  start = chrono::high_resolution_clock::now();
  json.stringify(jsonDestination);
  stop = chrono::high_resolution_clock::now();
  std::cout << elapsedTime(start, stop) << " microseconds to stringify to buffer." << '\n';

  // Parse from buffer
  start = chrono::high_resolution_clock::now();
  json.parse(js::BufferSource{ jsonDestination.view() });
  stop = chrono::high_resolution_clock::now();
  std::cout << elapsedTime(start, stop) << " microseconds to parse from buffer." << '\n';

  // Display contents
  if (jsonDestination.size() < kMaxFileLengthToDisplay) { std::cout << "[" << jsonDestination.view() << "]"; }
  std::cout << "--------------------FILE PROCESSED OK--------------------" << '\n';

  std::cout << "Finished " << fileName << "." << '\n';

}

int main(int, char **)
{
    std::cout << "JSON_Parse_Files started ..." << '\n';

  std::cout << js::JSON().version() << '\n';

  // For each json parse it, stringify it and display unless its to large.
  for (auto &fileName : Utility::createJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
      std::cerr << "Error: " << ex.what() << '\n';

    }
  }
  exit(EXIT_SUCCESS);
}