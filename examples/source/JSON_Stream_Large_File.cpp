// Program: JSON_Stream_Large_File.cpp
//
// Description: Demonstrate streaming a large JSON file for memory efficiency.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    init(plog::debug, "JSON_Stream_Large_File.log");
    PLOG_INFO << "JSON_Stream_Large_File started ...";
    PLOG_INFO << js::JSON().version();
    std::string largeFile = "files/large.json";
    // TODO: Replace with actual streaming logic if supported by JSON_Lib
    js::JSON json;
    json.parse(js::FileSource{ largeFile });
    PLOG_INFO << "Parsed large file (streaming logic not implemented).";
    return 0;
  } catch (const std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
    return 1;
  }
}
