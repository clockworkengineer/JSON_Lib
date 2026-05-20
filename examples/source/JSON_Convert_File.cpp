//
// Program: JSON_Convert_File
//
// Description: For a each JSON file in a directory parse it, convert all
// numbers, booleans and nulls to strings and save the result.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include "JSON_Convert.hpp"
#include <iostream>

namespace js = JSON_Lib;

/// <summary>
/// Parse JSON file and convert numbers into strings in its JSON tree.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  std::cout << "Converting ... " << fileName << '\n';

  JSON_Convert jsonConverter;
  js::JSON json;
  json.parse(js::FileSource{ fileName });
  json.traverse(jsonConverter);
  json.print(js::FileDestination{ fileName + ".new" });
  std::cout << "Finished " << fileName << "." << '\n';

}

int main(int, char **)
{
    std::cout << "JSON_Convert_File started ..." << '\n';

  std::cout << js::JSON().version() << '\n';

  // Convert JSON files.
  for (auto &fileName : Utility::createJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
      std::cerr << "Error: " << ex.what() << '\n';

    }
  }
  std::cout << "JSON_Convert_File exited." << '\n';

  exit(EXIT_SUCCESS);
}