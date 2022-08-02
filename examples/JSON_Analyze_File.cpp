//
// Program: JSON_Analyze_File
//
// Description: For a JSON file parse it then analyze its JSON Tree
// and produce a output report of the analysis.
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
#include <set>
// =============
// JSON Analyzer
// =============
#include "JSON_Analyzer.hpp"
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
/// Parse JSON file and analyze its JSON Tree.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName) {
  PLOG_INFO << "Analyzing " << fileName;
  const JSON json;
  JSON_Analyzer jsonAnalyzer;
  json.parse(FileSource{fileName});
  json.traverse(jsonAnalyzer);
  PLOG_INFO << jsonAnalyzer.dump();
  PLOG_INFO << "Finished " << fileName << ".";
}
// // ============================
// // ===== MAIN ENTRY POINT =====
// // ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  const std::vector<std::string> fileList{
      "testfile001.json", "testfile002.json", "testfile003.json",
      "testfile004.json", "testfile005.json",
      "large-file.json" // Not kept in GitHub as 24Meg in size.
  };
  //
  // Initialise logging.
  //
  plog::init(plog::debug, "JSON_Analyze_File.log");
  PLOG_INFO << "JSON_Analyze_File started ...";
  PLOG_INFO << JSON().version();
  //
  // Analyze JSON file and display its details.
  //
  for (auto &fileName : fileList) {
    try {
      if (const auto fullFileName{prefixTestDataPath(fileName)};
          std::filesystem::exists(fullFileName)) {
        std::cout << "Analyzing " << fullFileName << "\n";
        processJSONFile(fullFileName);
        std::cout << "Finished " << fullFileName << ".\n";
      } else {
        std::cout << "File " << fullFileName << " not present.\n";
      }
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  PLOG_INFO << "JSON_Analyze_File exited.";
  exit(EXIT_SUCCESS);
}