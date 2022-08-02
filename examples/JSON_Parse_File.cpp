//
// Program: JSON_Parse_File
//
// Description: Read a number of JSON files and parse from the file, stringify
// it back into text form and then parse the buffer created; timing each step in
// turn for each file.
//
// Dependencies: C20++, PLOG, JSONLib.
//
// =============
// INCLUDE FILES
// =============
// =======
// C++ STL
// =======
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
// =======
// Logging
// =======
#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"
// ====================
// JSON class namespace
// ====================
using namespace JSONLib;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
static constexpr size_t kMaxFileLengthToDisplay = 16 * 1024;
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Prefix current path to test data file name.
/// </summary>
/// <param name="name">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &file) {
  return ((std::filesystem::current_path() / "files" / file).string());
}
/// <summary>
/// Parse JSON file and display it reconstituted via stringify.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName) {
  std::cout << "Processing " << fileName << "\n";
  PLOG_INFO << "Processing " << fileName;
  const JSON json;
  BufferDestination jsonDestination;
  //
  // Parse
  //
  auto start = std::chrono::high_resolution_clock::now();
  json.parse(FileSource{fileName});
  auto stop = std::chrono::high_resolution_clock::now();
  auto parsedTime =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  PLOG_INFO << "Took " << parsedTime.count()
            << " microseconds to parse from file.";
  //
  // Stringify
  //
  start = std::chrono::high_resolution_clock::now();
  json.stringify(jsonDestination);
  stop = std::chrono::high_resolution_clock::now();
  auto stringifyTime =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  PLOG_INFO << "Took " << stringifyTime.count()
            << " microseconds to stringify.";
  //
  // Parse from buffer
  //
  BufferSource sourceBuffer{jsonDestination.getBuffer()};
  start = std::chrono::high_resolution_clock::now();
  json.parse(sourceBuffer);
  stop = std::chrono::high_resolution_clock::now();
  parsedTime =
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
  PLOG_INFO << "Took " << parsedTime.count()
            << " microseconds to parse from buffer.";
  //
  // Display contents
  //
  if (jsonDestination.getBuffer().size() < kMaxFileLengthToDisplay) {
    PLOG_INFO << "[" << jsonDestination.getBuffer();
  }
  PLOG_INFO << "----------------------------OK";
  std::cout << "Finished " << fileName << ".\n";
  PLOG_INFO << "Finished " << fileName << ".";
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  const std::vector<std::string> fileList{
      "testfile001.json", "testfile002.json", "testfile003.json",
      "testfile004.json", "testfile005.json",
      "large-file.json" // Not kept in GitHub as 24Meg in size.
  };
  //
  // Initialise logging.
  //
  plog::init(plog::debug, "JSON_Parse_Files.log");
  PLOG_INFO << "JSON_Parse_Files started ...";
  PLOG_INFO << JSON().version();
  //
  // For each json parse it, stringify it and display unless its to large.
  //
  for (auto &fileName : fileList) {
    try {
      if (const std::string fullFileName{prefixTestDataPath(fileName)};
          std::filesystem::exists(fullFileName)) {
        processJSONFile(fullFileName);
      } else {
        PLOG_INFO << "File " << fullFileName << " not present.";
      }
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  exit(EXIT_SUCCESS);
}