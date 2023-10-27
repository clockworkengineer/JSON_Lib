//
// Program: JSON_Convert_File
//
// Description: For a each JSON file in a directory parse it, convert all
// numbers, booleans and nulls to strings and save the result.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include <filesystem>
#include <string>
#include <vector>
#include <stdexcept>

#include "JSON_Convert.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace js = JSON_Lib;
namespace fs = std::filesystem;

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
/// Parse JSON file and convert numbers into strings in its JSON tree.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  PLOG_INFO << "Converting ... " << fileName;
  JSON_Convert jsonConverter;
  js::JSON json;
  json.parse(js::FileSource{ fileName });
  json.traverse(jsonConverter);
  json.print(js::FileDestination{ fileName + ".new" });
  PLOG_INFO << "Finished " << fileName << ".";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  plog::init(plog::debug, "JSON_Convert_File.log");
  PLOG_INFO << "JSON_Convert_File started ...";
  PLOG_INFO << js::JSON().version();
  // Convert JSON files.
  for (auto &fileName : readJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  PLOG_INFO << "JSON_Convert_File exited.";
  exit(EXIT_SUCCESS);
}