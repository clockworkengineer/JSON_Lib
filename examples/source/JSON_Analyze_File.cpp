//
// Program: JSON_Analyze_File
//
// Description: For each JSON file in a directory parse it, then analyze its
// JSON tree and produce an output report of the analysis.
//
// Dependencies: C++20, JSON_Lib.
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
  std::cout << "Analyzing " << fileName << '\n';

  js::JSON json;
  JSON_Analyzer jsonAnalyzer;
  json.parse(js::FileSource{ fileName });
  json.traverse(jsonAnalyzer);
  std::cout << jsonAnalyzer.dump() << '\n';

  std::cout << "Finished " << fileName << "." << '\n';

}

int main(int, char **)
{
    std::cout << "JSON_Analyze_File started ..." << '\n';

  // Output JSON Lib version and data structure metrics
  std::cout << js::JSON().version() << '\n';

  std::cout << JSON_Analyzer::dumpNumericSizes() << '\n';

  std::cout << JSON_Analyzer::dumpNodeSizes() << '\n';

  // Analyze JSON files.
  for (auto &fileName : Utility::createJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
      std::cerr << "Error: " << ex.what() << '\n';

    }
  }
  std::cout << "JSON_Analyze_File exited." << '\n';

  exit(EXIT_SUCCESS);
}