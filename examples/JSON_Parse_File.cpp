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
// ==========
// Namespaces
// ==========
namespace js = JSONLib;
namespace fs = std::filesystem;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
static constexpr size_t kMaxFileLengthToDisplay = 16 * 1024;
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Return a vector of JSON files to analyze.
/// </summary>
/// <returns>Vector of JSON file names</returns>
std::vector<std::string> readJSONFileList() {
  std::vector<std::string> fileList;
  for (auto &file : fs::directory_iterator(
           fs::path(fs::current_path() / "files").string())) {
    if (const auto fileName = file.path().string();
        fileName.ends_with(".json")) {
      fileList.push_back(fileName);
    }
  }
  return (fileList);
}
/// <summary>
/// Prefix current path to test data file name.
/// </summary>
/// <param name="name">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixPath(const std::string &file) {
  return ((std::filesystem::current_path() / "files" / file).string());
}
/// <summary>
/// Parse JSON file, stringify and parse JSON from buffer whilst timing each
/// step.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName) {
  auto elapsedTime = [](const auto &start, const auto &stop) {
    return (std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
                .count());
  };
  PLOG_INFO << "Processing " << fileName;
  const js::JSON json;
  js::BufferDestination jsonDestination;
  // Parse from file
  auto start = std::chrono::high_resolution_clock::now();
  json.parse(js::FileSource{fileName});
  auto stop = std::chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop) << " microseconds to parse from file.";
  // Stringify
  start = std::chrono::high_resolution_clock::now();
  json.stringify(jsonDestination);
  stop = std::chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop) << " microseconds to stringify.";
  // Parse from buffer
  start = std::chrono::high_resolution_clock::now();
  json.parse(js::BufferSource{jsonDestination.getBuffer()});
  stop = std::chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop)
            << " microseconds to parse from buffer.";
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
  // Initialise logging.
  plog::init(plog::debug, "JSON_Parse_Files.log");
  PLOG_INFO << "JSON_Parse_Files started ...";
  PLOG_INFO << js::JSON().version();
  // For each json parse it, stringify it and display unless its to large.
  for (auto &fileName : readJSONFileList()) {
    try {
      std::cout << "Processing " << fileName << "\n";
      processJSONFile(fileName);
      std::cout << "Finished " << fileName << ".\n";
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  exit(EXIT_SUCCESS);
}