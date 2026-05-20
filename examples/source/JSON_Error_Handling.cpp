// Program: JSON_Error_Handling.cpp
//
// Description: Demonstrate robust error handling for malformed JSON.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include <iostream>

namespace js = JSON_Lib;

int main(int, char **)
{
    std::cout << "JSON_Error_Handling started ..." << '\n';

  std::cout << js::JSON().version() << '\n';

  std::string badJSON = "{\"key\": 123, }"; // Malformed JSON (trailing comma)
  try {
    js::JSON json;
    json.parse(js::BufferSource{ badJSON });
    std::cout << "Parsed JSON successfully (unexpected)." << '\n';

  } catch (const std::exception &ex) {
    std::cerr << "Caught parse error: " << ex.what() << '\n';

  }
  return 0;
}
