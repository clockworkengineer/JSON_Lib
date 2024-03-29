//
// Program:  JSON_Files_To_XML
//
// Description: Use JSON_Lib to read in torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "Utility.hpp"

#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Converter.hpp"

#include "XML_Stringify.hpp"
#include "XML_Translator.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

namespace js = JSON_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    js::JSON json(std::make_unique<js::XML_Stringify>(js::XML_Translator(js::JSON_Converter())).release());
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