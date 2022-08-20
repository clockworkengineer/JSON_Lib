//
// Program: JSON_Display_Settings
//
// Description: Parse JSON example settings file and read its Bencode tree data
// (JNode) and write out a textual representation of it; this is almost the same
// as stringification but it is used to provide example code that traverses and
// interperets the JNode tree data.
//
// Dependencies: C20++, PLOG, JSONLib.
//
// =============
// INCLUDE FILES
// =============
// =======
// C++ STL
// =======
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Types.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
// =======
// Logging
// =======
#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"
// ==========
// NAMESPACES
// ==========
namespace json = JSONLib;
namespace fs = std::filesystem;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Return settings json file name.
/// </summary>
/// <returns>JSON settings file name.</returns>
std::string jsonSettingsFile() { return ((fs::current_path() / "files" / "settings.json").string()); }
/// <summary>
/// Process settings file top level object entry. This involves jusr reading the
/// entries JNode data and logging it to a file.
/// </summary>
void processEntry(const json::Object::Entry &entry)
{
  // Log main entry key
  std::string entryJSON{ "\n[" + entry.getKey() + "] = " };
  if (entry.getKey() == "files.exclude") {
    // Read object data (key/boolean pair) and add to log
    entryJSON += "\n{\n";
    for (const auto &file : json::JRef<json::Object>(entry).getObjectEntries()) {
      entryJSON += "\"" + file.getKey() + "\" : " + json::JRef<json::Boolean>(file).toString() + ",\n";
    }
    entryJSON.pop_back();
    entryJSON.pop_back();
    entryJSON += "\n}";
  } else if (entry.getKey() == "explorerExclude.backup") {
    // Read null data and add to log
    entryJSON += json::JRef<json::Null>(entry).toString();
  } else if (entry.getKey() == "cSpell.words") {
    // Read array of string data and add to log
    entryJSON += "[";
    for (const auto &word : json::JRef<json::Array>(entry).getArrayEntries()) {
      entryJSON += "\"" + json::JRef<json::String>(word).toString() + "\",";
    }
    entryJSON.pop_back();
    entryJSON += "]";
  } else if (entry.getKey() == "files.associations") {
    // Read object data (key/string pair) and add to log
    entryJSON += "\n{\n";
    for (const auto &file : json::JRef<json::Object>(entry).getObjectEntries()) {
      entryJSON += "\"" + file.getKey() + "\" : " + "\"" + json::JRef<json::String>(file).toString() + "\",\n";
    }
    entryJSON.pop_back();
    entryJSON.pop_back();
    entryJSON += "\n}";
  } else if (entry.getKey() == "C_Cpp.codeAnalysis.clangTidy.enabled") {
    // Read boolean data and add to log
    entryJSON += json::JRef<json::Boolean>(entry).toString();
  } else {
    throw std::runtime_error("Invalid JSON settings file.");
  }
  PLOG_INFO << entryJSON;
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    // Initialise logging.
    plog::init(plog::debug, "JSON_Display_Settings.log");
    PLOG_INFO << "JSON_Display_Settings started ...";
    // Log version
    PLOG_INFO << json::JSON().version();
    // Parse in settings file
    const json::JSON json;
    json.parse(json::FileSource{ jsonSettingsFile() });
    auto &settingsRoot = json.root();
    // JNode root has to be an object
    if (settingsRoot.getType() != json::JNodeType::object) { throw std::runtime_error("Invalid JSON settings file."); }
    // Loop and process each top level entry
    PLOG_INFO << "Displaying settings ...";
    for (const auto &entry : json::JRef<json::Object>(settingsRoot).getObjectEntries()) { processEntry(entry); }
  } catch (std::exception &e) {
    PLOG_ERROR << "Error: " << e.what();
  }
  PLOG_INFO << "JSON_Display_Settings exited.";
  exit(EXIT_SUCCESS);
}