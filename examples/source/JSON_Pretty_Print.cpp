// Program: JSON_Pretty_Print.cpp
//
// Description: Demonstrate pretty-printing and minifying JSON using JSON_Lib.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

int main(int, char **)
{
  try {
        std::cout << "JSON_Pretty_Print started ..." << '\n';

    std::cout << js::JSON().version() << '\n';

    std::string inputFile = "files/testfile001.json";
    std::string prettyFile = "files/testfile001_pretty.json";
    std::string minifiedFile = "files/testfile001_min.json";

    js::JSON json;
    json.parse(js::FileSource{ inputFile });

    // Pretty-print
    js::BufferDestination prettyDest;
    json.print(prettyDest);
    std::ofstream prettyOut(prettyFile);
    prettyOut << prettyDest.view();
    prettyOut.close();
    std::cout << "Pretty-printed JSON written to: " << prettyFile << '\n';
    // Minify
    js::BufferDestination minDest;
    json.stringify(minDest);
    std::ofstream minOut(minifiedFile);
    minOut << minDest.view();
    minOut.close();
    std::cout << "Minified JSON written to: " << minifiedFile << '\n';

  } catch (const std::exception &ex) {
    std::cerr << ex.what() << '\n';

    return 1;
  }
  return 0;
}
