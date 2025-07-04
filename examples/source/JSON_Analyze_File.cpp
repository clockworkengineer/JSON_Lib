//
// Program: JSON_Analyze_File
//
// Description: For each JSON file in a directory parse it, then analyze its
// JSON tree and produce an output report of the analysis.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include "JSON_Analyzer.hpp"

namespace js = JSON_Lib;

/// <summary>
/// Parse JSON file and analyze its JSON tree.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  PLOG_INFO << "Analyzing " << fileName;
  const js::JSON json;
  JSON_Analyzer jsonAnalyzer;
  json.parse(js::FileSource{ fileName });
  json.traverse(jsonAnalyzer);
  PLOG_INFO << jsonAnalyzer.dump();
  PLOG_INFO << "Finished " << fileName << ".";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  init(plog::debug, "JSON_Analyze_File.log");
  PLOG_INFO << "JSON_Analyze_File started ...";
  // Output JSON Lib version and data structure metrics
  PLOG_INFO << js::JSON().version();
  PLOG_INFO << JSON_Analyzer::dumpNumericSizes();
  PLOG_INFO << JSON_Analyzer::dumpNodeSizes();
  // Analyze JSON files.
  for (auto &fileName : Utility::createJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
      PLOG_ERROR << "Error: " << ex.what();
    }
  }
  PLOG_INFO << "JSON_Analyze_File exited.";
  exit(EXIT_SUCCESS);
}