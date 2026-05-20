//
// Program: JSON_Strip_Whitespace
//
// Description: For a each JSON file in a directory strip its white space characters
// to a temporary file and then overwrite the existing file by renaming the temporary file.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include <iostream>

namespace js = JSON_Lib;

/// <summary>
/// Strip a JSON file of all its whitespace.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  std::cout << "Stripping " << fileName << '\n';

  const js::JSON json;
  json.strip(js::FileSource{ fileName }, js::FileDestination{ fileName + ".stripped" });
  std::filesystem::rename(fileName + ".stripped", fileName);
  std::cout << "Finished " << fileName << "." << '\n';

}

int main(int, char **)
{
    std::cout << "JSON_Strip_Whitespace started ..." << '\n';

  // Output JSON Lib version
  std::cout << js::JSON().version() << '\n';

  // Strip JSON files.
  for (auto &fileName : Utility::createJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';

    }
  }
  std::cout << "JSON_Strip_Whitespace exited." << '\n';

  exit(EXIT_SUCCESS);
}