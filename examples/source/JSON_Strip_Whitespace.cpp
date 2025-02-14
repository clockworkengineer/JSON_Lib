//
// Program: JSON_Strip_Whitespace
//
// Description: For a each JSON file in a directory strip its white space characters
// to a temporary file and then overwrite the existing file by renaming the temporary file.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

/// <summary>
/// Strip a JSON file of all its whitespace.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  PLOG_INFO << "Stripping " << fileName;
  const js::JSON json;
  json.strip(js::FileSource{ fileName }, js::FileDestination{ fileName + ".stripped" });
  std::filesystem::rename(fileName + ".stripped", fileName);
  PLOG_INFO << "Finished " << fileName << ".";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  // Initialise logging.
  init(plog::debug, "JSON_Strip_Whitespace.log");
  PLOG_INFO << "JSON_Strip_Whitespace started ...";
  // Output JSON Lib version
  PLOG_INFO << js::JSON().version();
  // Strip JSON files.
  for (auto &fileName : Utility::createJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
    }
  }
  PLOG_INFO << "JSON_Strip_Whitespace exited.";
  exit(EXIT_SUCCESS);
}