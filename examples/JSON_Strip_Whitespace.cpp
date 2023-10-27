//
// Program: JSON_Strip_Whitespace
//
// Description: For a each JSON file in a directory strip its white space characters
// temporary file and then overwrite the existing file by renaming the temporary file.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include <filesystem>
#include <string>
#include <vector>
#include <stdexcept>

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"

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
/// Strip a JSON file of all its whitespace.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  PLOG_INFO << "Stripping " << fileName;
  const js::JSON json;
  json.strip(js::FileSource{ fileName }, js::FileDestination{ fileName + ".stripped" });
  fs::rename(fileName + ".stripped", fileName);
  PLOG_INFO << "Finished " << fileName << ".";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  plog::init(plog::debug, "JSON_Strip_Whitespace.log");
  PLOG_INFO << "JSON_Strip_Whitespace started ...";
  // Output JSON Lib version
  PLOG_INFO << js::JSON().version();
  // Strip JSON files.
  for (auto &fileName : readJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  PLOG_INFO << "JSON_Strip_Whitespace exited.";
  exit(EXIT_SUCCESS);
}