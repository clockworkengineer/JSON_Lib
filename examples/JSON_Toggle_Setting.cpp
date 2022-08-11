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
/// Return directory name containing JSON files.
/// </summary>
/// <returns>JSON file directory</returns>
std::string jsonFileDirectory() {
  return ((fs::current_path() / "files").string());
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
    json.parse(json::FileSource{
        (fs::path(jsonFileDirectory()) / "settings.json").string()});
    auto &settingsRoot = json.root();
    // BNode root has to be an object
    if (settingsRoot.getType() != json::JNodeType::object) {
      throw std::runtime_error("Invalid JSON settings file.");
    }
    if (settingsRoot["C_Cpp.codeAnalysis.clangTidy.enabled"].getType() ==
        json::JNodeType::boolean) {
      auto &enabled = json::JRef<json::Boolean>(
                          settingsRoot["C_Cpp.codeAnalysis.clangTidy.enabled"])
                          .boolean();
      PLOG_INFO << "Before = " << enabled;
      enabled = !enabled;
      PLOG_INFO << "After = " << enabled;
      json.stringify(json::FileDestination{
          (fs::path(jsonFileDirectory()) / "settings.json").string()});
    }
  } catch (std::exception &e) {
    PLOG_ERROR << "Error: " << e.what();
  }
  PLOG_INFO << "JSON_Toggle_Setting exited.";
  exit(EXIT_SUCCESS);
}