//
// Program: JSON_Index_File
//
// Description: For a each JSON file in a directory parse it, then create a JSON
// array of all its keys, write them away to an index file and create a new JSON file
// using indexes instead of key values (note the index are just numeric string values
// starting at "0" and incementing by one for each new key).
//
// Dependencies: C20++, PLOG, JSON_Lib.
//
// =============
// INCLUDE FILES
// =============
// =======
// C++ STL
// =======
#include <filesystem>
#include <string>
#include <vector>
#include <stdexcept>
// =============
// JSON Analyzer
// =============
#include "JSON_Indexer.hpp"
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
/// Parse JSON file, travserse its JSON tree to produce an index of
/// object keys, modify the object keys to be these index values and
/// write away JSON array of key values to .idx file plus the modified
/// JSON with index values to a file with extenstion ".new".
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  PLOG_INFO << "Indexing " << fileName;
  json::JSON json;
  JSON_Indexer jsonIndexer{ fileName };
  json.parse(json::FileSource{ fileName });
  json.traverse(jsonIndexer);
  json.stringify(json::FileDestination{ fileName + ".new" });
  jsonIndexer.save();
  PLOG_INFO << "Finished " << fileName << ".";
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  plog::init(plog::debug, "JSON_Index_File.log");
  PLOG_INFO << "JSON_Index_File started ...";
  PLOG_INFO << json::JSON().version();
  // Analyze JSON files.
  for (auto &fileName : readJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  PLOG_INFO << "JSON_Index_File exited.";
  exit(EXIT_SUCCESS);
}