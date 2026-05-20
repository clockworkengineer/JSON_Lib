//
// Program: JSON_Display_Settings
//
// Description: Parse JSON example settings file and read its JSON tree data
// (Node) and write out a textual representation of it; this is almost the same
// as stringification, but it is used to provide example code that traverses and
// interprets the Node tree data.
//
// Dependencies: C++20, JSON_Lib.
//

#include "JSON_Utility.hpp"
#include <iostream>

namespace js = JSON_Lib;

/// <summary>
/// Return settings json file name.
/// </summary>
/// <returns>JSON settings file name.</returns>
std::string jsonSettingsFile() { return (std::filesystem::current_path() / "files" / "settings.json").string(); }
/// <summary>
/// Process settings file top level object entry. This involves just reading the
/// entries Node data and logging it to a file.
/// </summary>
void processEntry(const js::Object::Entry &entry)
{
  auto key = entry.getKey();
  // Log main entry key
  std::string entryJSON{ "\n[" + std::string(key) + "] = " };
  if (key == "files.exclude") {
    // Read object data (key/boolean pair) and add to log
    entryJSON += "\n{\n";
    for (const auto &file : js::NRef<js::Object>(entry).value()) {
      entryJSON +=
        "\"" + std::string(file.getKey()) + "\" : " + js::NRef<js::Boolean>(file).toString() + ",\n";
    }
    entryJSON.pop_back();
    entryJSON.pop_back();
    entryJSON += "\n}";
  } else if (key == "explorerExclude.backup") {
    // Read null data and add to log
    entryJSON += js::NRef<js::Null>(entry).toString();
  } else if (key == "cSpell.words") {
    // Read array of string data and add to log
    entryJSON += "[";
    for (const auto &word : js::NRef<js::Array>(entry).value()) {
      entryJSON += "\"" + js::NRef<js::String>(word).toString() + "\",";
    }
    entryJSON.pop_back();
    entryJSON += "]";
  } else if (key == "files.associations") {
    // Read object data (key/string pair) and add to log
    entryJSON += "\n{\n";
    for (const auto &file : js::NRef<js::Object>(entry).value()) {
      entryJSON +=
        "\"" + js::NRef<js::String>(file).toString() + "\" : " + "\"" + js::NRef<js::String>(file).toString() + "\",\n";
    }
    entryJSON.pop_back();
    entryJSON.pop_back();
    entryJSON += "\n}";
  } else if (key == "C_Cpp.codeAnalysis.clangTidy.enabled") {
    // Read boolean data and add to log
    entryJSON += js::NRef<js::Boolean>(entry).toString();
  } else {
    throw std::runtime_error("Invalid JSON settings file.");
  }
  std::cout << entryJSON << '\n';

}

int main(int, char **)
{
  try {
        std::cout << "JSON_Display_Settings started ..." << '\n';

    // Log version
    std::cout << js::JSON().version() << '\n';

    // Parse in settings file
    js::JSON json;
    json.parse(js::FileSource{ jsonSettingsFile() });
    auto &settingsRoot = json.root();
    // Node root has to be an object
    if (!js::isA<js::Object>(settingsRoot)) { throw std::runtime_error("Invalid JSON settings file."); }
    // Loop and process each top level entry
    std::cout << "Displaying settings ..." << '\n';

    for (const auto &entry : js::NRef<js::Object>(settingsRoot).value()) { processEntry(entry); }
  } catch (std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';

  }
  std::cout << "JSON_Display_Settings exited." << '\n';

  exit(EXIT_SUCCESS);
}