//
// Program: JSON_Toggle_Setting
//
// Description: Parse JSON example settings file and toggle the clang-tidy
// enabled setting before writing it back to the settings file.
//
// Dependencies: C20++, PLOG, JSON_Lib.
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
namespace json = JSON_Lib;
namespace fs = std::filesystem;
// =======================
// LOCAL TYPES/DEFINITIONS
// =======================
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Return settings json file name.
/// </summary>
/// <returns>JSON settings file name.</returns>
std::string jsonSettingsFile() { return ((fs::current_path() / "files" / "settings.json").string()); }
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    // Initialise logging.
    plog::init(plog::debug, "JSON_Toggle_Setting.log");
    PLOG_INFO << "JSON_Toggle_Setting started ...";
    // Log version
    PLOG_INFO << json::JSON().version();
    // Parse in settings file
    json::JSON json;
    json.parse(json::FileSource{ jsonSettingsFile() });
    auto &settingsRoot = json.root();
    // JNode root has to be an object
    if (!settingsRoot.isObject()) { throw std::runtime_error("Invalid JSON settings file."); }
    // Check key exists
    if (!json::JRef<json::Object>(settingsRoot).contains("C_Cpp.codeAnalysis.clangTidy.enabled")) {
      throw std::runtime_error("Object missing key 'C_Cpp.codeAnalysis.clangTidy.enabled' .");
    }
    // Reference code analysis enabled flag
    auto &enabled = json::JRef<json::Boolean>(settingsRoot["C_Cpp.codeAnalysis.clangTidy.enabled"]).getBoolean();
    // Toggle it
    enabled = !enabled;
    // Write back settings with toggled flag
    json.print(json::FileDestination{ jsonSettingsFile() });
  } catch (std::exception &e) {
    PLOG_ERROR << "Error: " << e.what();
  }
  PLOG_INFO << "JSON_Toggle_Setting exited.";
  exit(EXIT_SUCCESS);
}