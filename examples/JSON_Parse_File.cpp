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
#include "JSON_Destinations.hpp"
#include "JSON_Sources.hpp"
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
/// Parse JSON file, stringify and parse JSON from buffer whilst timing each
/// step.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName) {
  auto elapsedTime = [](const auto &start) {
    return (std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start)
                .count());
  };
  PLOG_INFO << "Processing " << fileName;
  const JSON json;
  BufferDestination jsonDestination;
  // Parse from file
  auto start = std::chrono::high_resolution_clock::now();
  json.parse(FileSource{fileName});
  PLOG_INFO << elapsedTime(start) << " microseconds to parse from file.";
  // Stringify
  start = std::chrono::high_resolution_clock::now();
  json.stringify(jsonDestination);
  PLOG_INFO << elapsedTime(start) << " microseconds to stringify.";
  // Parse from buffer
  start = std::chrono::high_resolution_clock::now();
  json.parse(BufferSource{jsonDestination.getBuffer()});
  PLOG_INFO << elapsedTime(start) << " microseconds to parse from buffer.";
  // Display contents
  if (jsonDestination.getBuffer().size() < kMaxFileLengthToDisplay) {
    PLOG_INFO << "[" << jsonDestination.getBuffer() << "]";
  }
  PLOG_INFO << "--------------------FILE PROCESSED OK--------------------";
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
  // Initialise logging.
  plog::init(plog::debug, "JSON_Parse_Files.log");
  PLOG_INFO << "JSON_Parse_Files started ...";
  PLOG_INFO << JSON().version();
  // For each json parse it, stringify it and display unless its to large.
  for (auto &fileName : fileList) {
    try {
      if (const std::string fullFileName{prefixTestDataPath(fileName)};
          std::filesystem::exists(fullFileName)) {
        std::cout << "Processing " << fullFileName << "\n";
        processJSONFile(fullFileName);
        std::cout << "Finished " << fullFileName << ".\n";
      } else {
        PLOG_INFO << "File " << fullFileName << " not present.";
      }
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  exit(EXIT_SUCCESS);
}