// Program: JSON_Unicode_Support.cpp
//
// Description: Demonstrate handling of Unicode/UTF-8 characters in JSON.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    init(plog::debug, "JSON_Unicode_Support.log");
    PLOG_INFO << "JSON_Unicode_Support started ...";
    PLOG_INFO << js::JSON().version();

    // Example JSON with Unicode
    std::string unicodeJSON = R"({\"greeting\": \"こんにちは世界\", \"emoji\": \"😀\"})";
    js::JSON json;
    json.parse(js::BufferSource{ unicodeJSON });

    std::cout << "Greeting: " << json["greeting"].asString() << std::endl;
    std::cout << "Emoji: " << json["emoji"].asString() << std::endl;
    std::cout << "Full JSON: " << json.stringify(js::StringifyFormat::Pretty) << std::endl;
  }
  catch (const std::exception &ex) {
    PLOG_ERROR << ex.what();
    return 1;
  }
  return 0;
}
