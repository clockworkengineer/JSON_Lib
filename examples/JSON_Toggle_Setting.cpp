//
// Program: JSON_Toggle_Setting
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
std::string jsonSettingsFile() {
  return ((fs::current_path() / "files" / "settings.json").string());
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    // Initialise logging.
    plog::init(plog::debug, "JSON_Toggle_Setting.log");
    PLOG_INFO << "JSON_Toggle_Setting started ...";
    // Log version
    PLOG_INFO << json::JSON().version();
    // Parse in settings file
    json::JSON json;
    json.parse(json::FileSource{jsonSettingsFile()});
    auto &settingsRoot = json.root();
    // BNode root has to be an object
    if (settingsRoot.getType() != json::JNodeType::object) {
      throw std::runtime_error("Invalid JSON settings file.");
    }
    // Reference code analysis enabled flag
    auto &enabled = json::JRef<json::Boolean>(
                        settingsRoot["C_Cpp.codeAnalysis.clangTidy.enabled"])
                        .boolean();
    // Toggle it
    enabled = !enabled;
    // Write back settings with toggled flag
    json.stringify(json::FileDestination{jsonSettingsFile()});
  } catch (std::exception &e) {
    PLOG_ERROR << "Error: " << e.what();
  }
  PLOG_INFO << "JSON_Toggle_Setting exited.";
  exit(EXIT_SUCCESS);
}