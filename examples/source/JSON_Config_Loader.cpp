// Program: JSON_Config_Loader.cpp
//
// Description: Load, modify, and save application configuration from JSON.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    init(plog::debug, "JSON_Config_Loader.log");
    PLOG_INFO << "JSON_Config_Loader started ...";
    PLOG_INFO << js::JSON().version();
    std::string configFile = "files/app_config.json";
    js::JSON json;
    json.parse(js::FileSource{ configFile });
    // Modify a config value
    json["window"]["width"] = 1024;
    json["window"]["height"] = 768;
    json.stringify(js::FileDestination{ configFile });
    PLOG_INFO << "Config updated and saved.";
    return 0;
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
    return 1;
  }
}
