//
// Program:  JSON_Files_To_XML
//
// Description: Use JSON_Lib to read in torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "XML_Stringify.hpp"

namespace js = JSON_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    js::JSON json(js::makeStringify<js::XML_Stringify>());
    // Initialise logging.
    plog::init(plog::debug, "JSON_Files_To_XML.log");
    PLOG_INFO << "JSON_Files_To_XML started ...";
    PLOG_INFO << json.version();
    for (const auto &jsonFileName : Utility::createJSONFileList()) {
      json.parse(js::FileSource(jsonFileName));
      json.stringify(js::FileDestination(Utility::createFileName(jsonFileName, ".xml")));
      PLOG_INFO << "Created file " << Utility::createFileName(jsonFileName, ".xml") << " from " << jsonFileName;
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "JSON_Files_To_XML exited.";
  exit(EXIT_SUCCESS);
}