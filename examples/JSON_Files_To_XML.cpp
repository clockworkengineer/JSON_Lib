//
// Program:  JSON_Files_To_XML
//
// Description: Use JSON_Lib to read in torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, JSON_Lib.
//

#include <stdexcept>

#include "Utility.hpp"
#include "XML_Stringify.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    JSON_Lib::JSON json(std::make_unique<JSON_Lib::XML_Stringify>().release());
    // Initialise logging.
    plog::init(plog::debug, "JSON_Files_To_XML.log");
    PLOG_INFO << "JSON_Files_To_XML started ...";
    PLOG_INFO << json.version();
    for (const auto &jsonFileName : Utility::createJSONFileList()) {
      json.parse(JSON_Lib::FileSource(jsonFileName));
      json.stringify(JSON_Lib::FileDestination(Utility::createFileName(jsonFileName, ".xml")));
      PLOG_INFO << "Created file " << Utility::createFileName(jsonFileName, ".xml") << " from " << jsonFileName;
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "JSON_Files_To_XML exited.";
  exit(EXIT_SUCCESS);
}