//
// Program:  JSON_Files_To_Bencode
//
// Description: Use JSON_Lib to read in torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, JSON_Lib.
//

#include <stdexcept>

#include "Utility.hpp"
#include "Bencode_Stringify.hpp"

#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    JSON_Lib::JSON json(std::make_unique<JSON_Lib::Bencode_Stringify>().release());
    // Initialise logging.
    plog::init(plog::debug, "JSON_Files_To_Bencode.log");
    PLOG_INFO << "JSON_Files_To_Bencode started ...";
    PLOG_INFO << JSON_Lib::JSON().version();
    for (const auto &jsonFileName : Utility::createJSONFileList()) {
      json.parse(JSON_Lib::FileSource(jsonFileName));
      json.stringify(JSON_Lib::FileDestination(Utility::createFileName(jsonFileName, ".ben")));
      PLOG_INFO << "Created file " << Utility::createFileName(jsonFileName, ".ben") << " from " << jsonFileName;
    }
  } catch (const std::exception &ex) {
    std::cout << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "JSON_Files_To_Bencode exited.";
  exit(EXIT_SUCCESS);
}