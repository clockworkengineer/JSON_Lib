// Program: JSON_Unicode_Support.cpp
//
// Description: Demonstrate handling of Unicode/UTF-8 characters in JSON.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

#include <iostream>

int main(int, char **)
{
  try {
        std::cout << "JSON_Unicode_Support started ..." << '\n';

    std::cout << js::JSON().version();
 << '\n';
    // Example JSON with Unicode
    std::string unicodeJSON = R"({\"greeting\": \"こんにちは世界\", \"emoji\": \"😀\"})";
    js::JSON json;
    json.parse(js::BufferSource{ unicodeJSON });

    std::cout << "Greeting: " << js::NRef<js::String>(json["greeting"]).value() << std::endl;
    std::cout << "Emoji: " << js::NRef<js::String>(json["emoji"]).value() << std::endl;

    js::BufferDestination fullDest;
    json.print(fullDest);
    std::cout << "Full JSON: " << fullDest.view() << std::endl;
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << '\n';

    return 1;
  }
  return 0;
}
