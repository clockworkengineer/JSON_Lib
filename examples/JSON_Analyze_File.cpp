//
// Program: JSON_Analyze_File
//
// Description: For a each JSON file in a directory parse it, then analyze its
// JSON tree and produce a output report of the analysis.
//
// Dependencies: C20++, PLOG, JSONLib.
//
// =============
// INCLUDE FILES
// =============
// =======
// C++ STL
// =======
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
// =============
// JSON Analyzer
// =============
#include "JSON_Analyzer.hpp"
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
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
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
/// Parse JSON file and analyze its JSON tree.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  PLOG_INFO << "Analyzing " << fileName;
  const json::JSON json;
  json::JSON_Analyzer jsonAnalyzer;
  json.parse(json::FileSource{ fileName });
  json.traverse(jsonAnalyzer);
  PLOG_INFO << jsonAnalyzer.dump();
  PLOG_INFO << "Finished " << fileName << ".";
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  plog::init(plog::debug, "JSON_Analyze_File.log");
  PLOG_INFO << "JSON_Analyze_File started ...";
  PLOG_INFO << json::JSON().version();
  // Analyze JSON files.
  for (auto &fileName : readJSONFileList()) {
    try {
      std::cout << "Analyzing " << fileName << "\n";
      processJSONFile(fileName);
      std::cout << "Finished " << fileName << ".\n";
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  PLOG_INFO << "JSON_Analyze_File exited.";
  exit(EXIT_SUCCESS);
}