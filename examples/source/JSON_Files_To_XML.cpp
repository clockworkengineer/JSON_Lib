//
// Program: JSON_Files_To_XML
//
// Description: Use JSON_Lib to read in a torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include "XML_Stringify.hpp"
#include <iostream>

namespace js = JSON_Lib;

int main(int, char **)
{
  try {
    js::JSON json(js::makeStringify<js::XML_Stringify>());
        std::cout << "JSON_Files_To_XML started ..." << '\n';

    std::cout << json.version() << '\n';

    for (const auto &jsonFileName : Utility::createJSONFileList()) {
      json.parse(js::FileSource(jsonFileName));
      json.stringify(js::FileDestination(Utility::createFileName(jsonFileName, ".xml")));
      std::cout << "Created file " << Utility::createFileName(jsonFileName, ".xml") << " from " << jsonFileName << '\n';

    }
  } catch (const std::exception &ex) {
    std::cerr << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  std::cout << "JSON_Files_To_XML exited." << '\n';

  exit(EXIT_SUCCESS);
}