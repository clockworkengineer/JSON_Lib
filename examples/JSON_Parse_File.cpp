//
// Program: JSON_Parse_File
//
// Description: For each JSON file in a directory parse it, stringify
// it back into text form and then parse the buffer created; timing each
// step in turn for each file.
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
// ==========
// NAMESPACES
// ==========
namespace json = JSONLib;
namespace fs = std::filesystem;
namespace chrono = std::chrono;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
static constexpr size_t kMaxFileLengthToDisplay = 16 * 1024;
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Return directory name containing JSON files.
/// </summary>
/// <returns>JSON file directory</returns>
std::string jsonFileDirectory() {
  return ((fs::current_path() / "files").string());
}
/// <summary>
/// Return a vector of JSON files to analyze.
/// </summary>
/// <returns>Vector of JSON file names</returns>
std::vector<std::string> readJSONFileList() {
  std::vector<std::string> fileList;
  for (auto &file : fs::directory_iterator(fs::path(jsonFileDirectory()))) {
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
  return ((fs::current_path() / "files" / file).string());
}
/// <summary>
/// Parse JSON file, stringify and parse JSON from buffer whilst timing each
/// step.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName) {
  auto elapsedTime = [](const auto &start, const auto &stop) {
    return (chrono::duration_cast<chrono::microseconds>(stop - start).count());
  };
  PLOG_INFO << "Processing " << fileName;
  const json::JSON json;
  json::BufferDestination jsonDestination;
  // Parse from file
  auto start = chrono::high_resolution_clock::now();
  json.parse(json::FileSource{fileName});
  auto stop = chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop) << " microseconds to parse from file.";
  // Stringify
  start = chrono::high_resolution_clock::now();
  json.stringify(jsonDestination);
  stop = chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop) << " microseconds to stringify.";
  // Parse from buffer
  start = chrono::high_resolution_clock::now();
  json.parse(json::BufferSource{jsonDestination.getBuffer()});
  stop = chrono::high_resolution_clock::now();
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
  PLOG_INFO << json::JSON().version();
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