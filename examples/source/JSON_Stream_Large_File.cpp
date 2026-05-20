// Program: JSON_Stream_Large_File.cpp
//
// Description: Demonstrate streaming a large JSON file for memory efficiency.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include <iostream>

namespace js = JSON_Lib;

int main(int, char **)
{
  try {
        std::cout << "JSON_Stream_Large_File started ..." << '\n';

    std::cout << js::JSON().version() << '\n';

    std::string largeFile = "files/large.json";
    // TODO: Replace with actual streaming logic if supported by JSON_Lib
    js::JSON json;
    json.parse(js::FileSource{ largeFile });
    std::cout << "Parsed large file (streaming logic not implemented)." << '\n';

    return 0;
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';

    return 1;
  }
}
