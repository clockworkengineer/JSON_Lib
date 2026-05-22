//
// Program: JSON_Files_To_YAML
//
// Description: Use JSON_Lib to read in a torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include "YAML_Stringify.hpp"

namespace js = JSON_Lib;

int main(int, char **)
{
  try {
    js::JSON json(js::makeStringify<js::YAML_Stringify>());
        std::cout << "JSON_Files_To_YAML started ..." << '\n';

    std::cout << json.version() << '\n';

    for (const auto &jsonFileName : Utility::createJSONFileList()) {
      json.parse(js::FileSource(jsonFileName));
      json.stringify(js::FileDestination(Utility::createFileName(jsonFileName, ".YAML")));
      std::cout << "Created file " << Utility::createFileName(jsonFileName, ".YAML") << " from " << jsonFileName << '\n';

    }
  } catch (const std::exception &ex) {
    std::cerr << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  std::cout << "JSON_Files_To_YAML exited." << '\n';

  exit(EXIT_SUCCESS);
}
