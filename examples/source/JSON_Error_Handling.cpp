// Program: JSON_Error_Handling.cpp
//
// Description: Demonstrate robust error handling for malformed JSON.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

int main(int, char **)
{
  init(plog::debug, "JSON_Error_Handling.log");
  PLOG_INFO << "JSON_Error_Handling started ...";
  PLOG_INFO << js::JSON().version();
  std::string badJSON = "{\"key\": 123, }"; // Malformed JSON (trailing comma)
  try {
    js::JSON json;
    json.parse(js::BufferSource{ badJSON });
    PLOG_INFO << "Parsed JSON successfully (unexpected).";
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Caught parse error: " << ex.what();
  }
  return 0;
}
