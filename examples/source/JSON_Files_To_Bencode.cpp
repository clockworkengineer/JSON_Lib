//
// Program:  JSON_Files_To_Bencode
//
// Description: Use JSON_Lib to read in torrent file then write
// it out as JSON using a custom encoder.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include "Bencode_Stringify.hpp"

namespace js = JSON_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    js::JSON json(js::makeStringify<js::Bencode_Stringify>());
    // Initialise logging.
    plog::init(plog::debug, "JSON_Files_To_Bencode.log");
    PLOG_INFO << "JSON_Files_To_Bencode started ...";
    PLOG_INFO << json.version();
    for (const auto &jsonFileName : Utility::createJSONFileList()) {
      json.parse(js::FileSource(jsonFileName));
      json.stringify(js::FileDestination(Utility::createFileName(jsonFileName, ".ben")));
      PLOG_INFO << "Created file " << Utility::createFileName(jsonFileName, ".ben") << " from " << jsonFileName;
    }
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Error Processing Torrent File: [" << ex.what() << "]\n";
  }
  PLOG_INFO << "JSON_Files_To_Bencode exited.";
  exit(EXIT_SUCCESS);
}