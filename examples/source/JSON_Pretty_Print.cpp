// Program: JSON_Pretty_Print.cpp
//
// Description: Demonstrate pretty-printing and minifying JSON using JSON_Lib.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    init(plog::debug, "JSON_Pretty_Print.log");
    PLOG_INFO << "JSON_Pretty_Print started ...";
    PLOG_INFO << js::JSON().version();
    std::string inputFile = "files/testfile001.json";
    std::string prettyFile = "files/testfile001_pretty.json";
    std::string minifiedFile = "files/testfile001_min.json";

    js::JSON json;
    json.parse(js::FileSource{ inputFile });

    // Pretty-print
    std::ofstream prettyOut(prettyFile);
    prettyOut << json.stringify(js::StringifyFormat::Pretty);
    prettyOut.close();
    PLOG_INFO << "Pretty-printed JSON written to: " << prettyFile;

    // Minify
    std::ofstream minOut(minifiedFile);
    minOut << json.stringify(js::StringifyFormat::Minified);
    minOut.close();
    PLOG_INFO << "Minified JSON written to: " << minifiedFile;
  }
  catch (const std::exception &ex) {
    PLOG_ERROR << ex.what();
    return 1;
  }
  return 0;
}
