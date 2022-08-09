//
// Program: JSON_Display_Settings
//
// Description:
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
/// Return directory name containing JSON files.
/// </summary>
/// <returns>JSON file directory</returns>
std::string jsonFileDirectory() {
  return ((fs::current_path() / "files").string());
}
void processEntry(const json::Object::Entry &entry) {
  std::string entryJSON;
  PLOG_INFO << "[" << entry.getKey() << "]";
  if (entry.getKey() == "files.exclude") {
    entryJSON += "\n{\n";
    for (const auto &file : json::JRef<json::Object>(entry).objectEntries()) {
      entryJSON += "\"" + file.getKey() +
                   "\" : " + json::JRef<json::Boolean>(file).toString() + ",\n";
    }
    entryJSON.pop_back();
    entryJSON.pop_back();
    entryJSON += "\n}";
  } else if (entry.getKey() == "explorerExclude.backup") {
    entryJSON += json::JRef<json::Null>(entry).toString();
  } else if (entry.getKey() == "cSpell.words") {
    entryJSON += "[";
    for (const auto &word : json::JRef<json::Array>(entry).array()) {
      entryJSON += "\"" + json::JRef<json::String>(word).toString() + "\",";
    }
    entryJSON.pop_back();
    entryJSON += "]";
  } else if (entry.getKey() == "files.associations") {
    entryJSON += "\n{\n";
    for (const auto &file : json::JRef<json::Object>(entry).objectEntries()) {
      entryJSON += "\"" + file.getKey() + "\" : " + "\"" +
                   json::JRef<json::String>(file).toString() + "\",\n";
    }
    entryJSON.pop_back();
    entryJSON.pop_back();
    entryJSON += "\n}";
  } else if (entry.getKey() == "C_Cpp.codeAnalysis.clangTidy.enabled") {
    entryJSON += json::JRef<json::Boolean>(entry).toString();
  } else {
    throw std::runtime_error("Invalid JSON settings file.");
  }
  PLOG_INFO << entryJSON;
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "JSON_Display_Settings.log");
    PLOG_INFO << "JSON_Display_Settings started ...";
    PLOG_INFO << json::JSON().version();
    const json::JSON json;
    json.parse(json::FileSource{
        (fs::path(jsonFileDirectory()) / "settings.json").string()});
    auto &settingsRoot = json.root();
    if (settingsRoot.getType() != json::JNodeType::object) {
      throw std::runtime_error("Invalid JSON settings file.");
    }
    PLOG_INFO << "Displaying settings ...";
    for (const auto &entry :
         json::JRef<json::Object>(settingsRoot).objectEntries()) {
      processEntry(entry);
    }
  } catch (std::exception &e) {
    PLOG_ERROR << "Error: " << e.what();
  }
  PLOG_INFO << "JSON_Display_Settings exited.";
  exit(EXIT_SUCCESS);
}