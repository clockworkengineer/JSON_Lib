//
// Program: JSON_Display_Settings
//
// Description: Parse JSON example settings file and read its JSON tree data
// (JNode) and write out a textual representation of it; this is almost the same
// as stringification, but it is used to provide example code that traverses and
// interprets the JNode tree data.
//
// Dependencies: C++20, PLOG, JSON_Lib.
//

#include "JSON_Utility.hpp"

namespace js = JSON_Lib;

/// <summary>
/// Return settings json file name.
/// </summary>
/// <returns>JSON settings file name.</returns>
std::string jsonSettingsFile() { return (std::filesystem::current_path() / "files" / "settings.json").string(); }
/// <summary>
/// Process settings file top level object entry. This involves just reading the
/// entries JNode data and logging it to a file.
/// </summary>
void processEntry(const js::Object::Entry &entry)
{
  auto key = entry.getKey();
  // Log main entry key
  std::string entryJSON{ "\n[" + std::string(key) + "] = " };
  if (key == "files.exclude") {
    // Read object data (key/boolean pair) and add to log
    entryJSON += "\n{\n";
    for (const auto &file : js::JRef<js::Object>(entry).value()) {
      entryJSON +=
        "\"" + std::string(file.getKey()) + "\" : " + js::JRef<js::Boolean>(file).toString() + ",\n";
    }
    entryJSON.pop_back();
    entryJSON.pop_back();
    entryJSON += "\n}";
  } else if (key == "explorerExclude.backup") {
    // Read null data and add to log
    entryJSON += js::JRef<js::Null>(entry).toString();
  } else if (key == "cSpell.words") {
    // Read array of string data and add to log
    entryJSON += "[";
    for (const auto &word : js::JRef<js::Array>(entry).value()) {
      entryJSON += "\"" + js::JRef<js::String>(word).toString() + "\",";
    }
    entryJSON.pop_back();
    entryJSON += "]";
  } else if (key == "files.associations") {
    // Read object data (key/string pair) and add to log
    entryJSON += "\n{\n";
    for (const auto &file : js::JRef<js::Object>(entry).value()) {
      entryJSON +=
        "\"" + js::JRef<js::String>(file).toString() + "\" : " + "\"" + js::JRef<js::String>(file).toString() + "\",\n";
    }
    entryJSON.pop_back();
    entryJSON.pop_back();
    entryJSON += "\n}";
  } else if (key == "C_Cpp.codeAnalysis.clangTidy.enabled") {
    // Read boolean data and add to log
    entryJSON += js::JRef<js::Boolean>(entry).toString();
  } else {
    throw std::runtime_error("Invalid JSON settings file.");
  }
  PLOG_INFO << entryJSON;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    // Initialise logging.
    init(plog::debug, "JSON_Display_Settings.log");
    PLOG_INFO << "JSON_Display_Settings started ...";
    // Log version
    PLOG_INFO << js::JSON().version();
    // Parse in settings file
    const js::JSON json;
    json.parse(js::FileSource{ jsonSettingsFile() });
    auto &settingsRoot = json.root();
    // JNode root has to be an object
    if (!js::isA<js::Object>(settingsRoot)) { throw std::runtime_error("Invalid JSON settings file."); }
    // Loop and process each top level entry
    PLOG_INFO << "Displaying settings ...";
    for (const auto &entry : js::JRef<js::Object>(settingsRoot).value()) { processEntry(entry); }
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "JSON_Display_Settings exited.";
  exit(EXIT_SUCCESS);
}