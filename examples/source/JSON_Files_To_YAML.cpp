//
// Program: JSON_Files_To_YAML
//
// Description: Use JSON_Lib to read in a torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include "YAML_Stringify.hpp"

namespace js = JSON_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    const js::JSON json(js::makeStringify<js::YAML_Stringify>());
    // Initialise logging.
    init(plog::debug, "JSON_Files_To_YAML.log");
    PLOG_INFO << "JSON_Files_To_YAML started ...";
    PLOG_INFO << json.version();
    for (const auto &jsonFileName : Utility::createJSONFileList()) {
      json.parse(js::FileSource(jsonFileName));
      json.stringify(js::FileDestination(Utility::createFileName(jsonFileName, ".YAML")));
      PLOG_INFO << "Created file " << Utility::createFileName(jsonFileName, ".YAML") << " from " << jsonFileName;
    }
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "JSON_Files_To_YAML exited.";
  exit(EXIT_SUCCESS);
}
