//
// Program: JSON_Parse_File
//
// Description: For each JSON file in a directory parse it, stringify
// it back into text form and then parse the buffer created; timing each
// step in turn for each file.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include <filesystem>
#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>

#include "JSON.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace js = JSON_Lib;
namespace fs = std::filesystem;
namespace chrono = std::chrono;

static constexpr size_t kMaxFileLengthToDisplay = 16 * 1024;

/// <summary>
/// Return a vector of JSON files to analyze.
/// </summary>
/// <returns>Vector of JSON file names</returns>
std::vector<std::string> readJSONFileList()
{
  std::vector<std::string> fileList;
  for (auto &file : fs::directory_iterator(fs::current_path() / "files")) {
    if (const auto fileName = file.path().string(); fileName.ends_with(".json")) { fileList.push_back(fileName); }
  }
  return (fileList);
}
/// <summary>
/// Parse JSON file, stringify and parse JSON from buffer whilst timing each
/// step.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  auto elapsedTime = [](const auto &start, const auto &stop) {
    return (chrono::duration_cast<chrono::microseconds>(stop - start).count());
  };
  PLOG_INFO << "Processing " << fileName;
  const js::JSON json;
  js::BufferDestination jsonDestination;
  // Parse from file
  auto start = chrono::high_resolution_clock::now();
  json.parse(js::FileSource{ fileName });
  auto stop = chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop) << " microseconds to parse from file.";
  // Stringify to file
  start = chrono::high_resolution_clock::now();
  json.stringify(js::FileDestination{ fileName + ".new" });
  stop = chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop) << " microseconds to stringify to file.";
  // Stringify to buffer
  start = chrono::high_resolution_clock::now();
  json.stringify(jsonDestination);
  stop = chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop) << " microseconds to stringify to buffer.";
  // Parse from buffer
  start = chrono::high_resolution_clock::now();
  json.parse(js::BufferSource{ jsonDestination.getBuffer() });
  stop = chrono::high_resolution_clock::now();
  PLOG_INFO << elapsedTime(start, stop) << " microseconds to parse from buffer.";
  // Display contents
  if (jsonDestination.getBuffer().size() < kMaxFileLengthToDisplay) {
    PLOG_INFO << "[" << jsonDestination.getBuffer() << "]";
  }
  PLOG_INFO << "--------------------FILE PROCESSED OK--------------------";
  PLOG_INFO << "Finished " << fileName << ".";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  plog::init(plog::debug, "JSON_Parse_Files.log");
  PLOG_INFO << "JSON_Parse_Files started ...";
  PLOG_INFO << js::JSON().version();
  // For each json parse it, stringify it and display unless its to large.
  for (auto &fileName : readJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
      PLOG_ERROR << "Error: " << ex.what();
    }
  }
  exit(EXIT_SUCCESS);
}