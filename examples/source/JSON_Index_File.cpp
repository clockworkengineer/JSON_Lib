//
// Program: JSON_Index_File
//
// Description: For a each JSON file in a directory parse it, then create a JSON
// array of all its keys, write them away to an index file and create a new JSON file
// using indexes instead of key values (note the index are just numeric string values
// starting at "0" and incrementing by one for each new key).
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include "JSON_Indexer.hpp"

namespace js = JSON_Lib;

/// <summary>
/// Parse JSON file, traverse its JSON tree to produce an index of
/// object keys, modify the object keys to be these index values and
/// write away JSON array of key values to .idx file plus the modified
/// JSON with index values to a file with extension ".new".
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
  std::cout << "Indexing " << fileName << '\n';

  js::JSON json;
  JSON_Indexer jsonIndexer{ fileName };
  json.parse(js::FileSource{ fileName });
  json.traverse(jsonIndexer);
  json.stringify(js::FileDestination{ fileName + ".new" });
  jsonIndexer.save();
  std::cout << "Finished " << fileName << "." << '\n';

}

int main(int, char **)
{
    std::cout << "JSON_Index_File started ..." << '\n';

  std::cout << js::JSON().version() << '\n';

  // Analyze JSON files.
  for (auto &fileName : Utility::createJSONFileList()) {
    try {
      processJSONFile(fileName);
    } catch (std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';

    }
  }
  std::cout << "JSON_Index_File exited." << '\n';

  exit(EXIT_SUCCESS);
}