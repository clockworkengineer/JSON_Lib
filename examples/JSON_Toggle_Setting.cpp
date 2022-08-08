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
  // Initialise logging.
  plog::init(plog::debug, "JSON_Toggle_Setting.log");
  PLOG_INFO << "JSON_Toggle_Setting started ...";
  PLOG_INFO << json::JSON().version();
  PLOG_INFO << "JSON_Toggle_Setting exited.";
  exit(EXIT_SUCCESS);
}