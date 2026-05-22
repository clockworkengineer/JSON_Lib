// Program: JSON_Config_Loader.cpp
//
// Description: Load, modify, and save application configuration from JSON.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

int main(int, char **)
{
  try {
        std::cout << "JSON_Config_Loader started ..." << '\n';

    std::cout << js::JSON().version() << '\n';

    std::string configFile = "files/app_config.json";
    js::JSON json;
    json.parse(js::FileSource{ configFile });
    // Modify a config value
    json["window"]["width"] = 1024;
    json["window"]["height"] = 768;
    json.stringify(js::FileDestination{ configFile });
    std::cout << "Config updated and saved." << '\n';

    return 0;
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';

    return 1;
  }
}
