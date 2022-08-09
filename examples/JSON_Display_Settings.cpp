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
void processEntry(const JSONLib::Object::Entry &entry) {
  PLOG_INFO << "[" << entry.getKey() << "]";

  if (entry.getKey() == "files.exclude") {
    if (entry.getJNode().getType() != json::JNodeType::object) {
      throw std::runtime_error("Invalid JSON settings file.");
    }
    for (const auto &file :
         json::JRef<json::Object>(entry.getJNode()).objectEntries()) {
      if (file.getJNode().getType() == json::JNodeType::boolean) {
        PLOG_INFO << "{" << file.getKey() << " : "
                  << json::JRef<json::Boolean>(file.getJNode()).toString()
                  << "}";
      } else {
        throw std::runtime_error("Invalid JSON settings file.");
      }
    }
  } else if (entry.getKey() == "explorerExclude.backup") {
    if (entry.getJNode().getType() != json::JNodeType::null) {
      throw std::runtime_error("Invalid JSON settings file.");
    }
    return;
  } else if (entry.getKey() == "cSpell.words") {
    if (entry.getJNode().getType() != json::JNodeType::array) {
      throw std::runtime_error("Invalid JSON settings file.");
    }
    for (const auto &word : json::JRef<json::Array>(entry.getJNode()).array()) {
      if (word.getType() != json::JNodeType::string) {
        throw std::runtime_error("Invalid JSON settings file.");
      }
      PLOG_INFO << "["
                << "\"" << json::JRef<json::String>(word).toString() << "\""
                << "]";
    }
  } else if (entry.getKey() == "files.associations") {
    if (entry.getJNode().getType() != json::JNodeType::object) {
      throw std::runtime_error("Invalid JSON settings file.");
    }
    for (const auto &file :
         json::JRef<json::Object>(entry.getJNode()).objectEntries()) {
      if (file.getJNode().getType() != json::JNodeType::string) {
        throw std::runtime_error("Invalid JSON settings file.");
      }
      PLOG_INFO << "{" << file.getKey() << " : "
                << "\"" << json::JRef<json::String>(file.getJNode()).toString()
                << "\""
                << "}";
    }
  } else if (entry.getKey() == "C_Cpp.codeAnalysis.clangTidy.enabled") {
    if (entry.getJNode().getType() != json::JNodeType::boolean) {
      throw std::runtime_error("Invalid JSON settings file.");
    }
    return;
  }
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
    PLOG_INFO << "Top level keys ...";
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