//
// Program: JSON_Toggle_Setting
//
// Description: Parse JSON example settings file and toggle the clang-tidy
// enabled setting before writing it back to the settings file.
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

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
  try {
    // Initialise logging.
    init(plog::debug, "JSON_Toggle_Setting.log");
    PLOG_INFO << "JSON_Toggle_Setting started ...";
    // Log version
    PLOG_INFO << js::JSON().version();
    // Parse in settings file
    js::JSON json;
    json.parse(js::FileSource{ jsonSettingsFile() });
    auto &settingsRoot = json.root();
    // JNode root has to be an object
    if (!js::isA<js::Object>(settingsRoot)) { throw std::runtime_error("Invalid JSON settings file."); }
    // Check key exists
    if (!js::JRef<js::Object>(settingsRoot).contains("C_Cpp.codeAnalysis.clangTidy.enabled")) {
      throw std::runtime_error("Object missing key 'C_Cpp.codeAnalysis.clangTidy.enabled' .");
    }
    // Reference code analysis enabled flag
    auto &enabled = js::JRef<js::Boolean>(settingsRoot["C_Cpp.codeAnalysis.clangTidy.enabled"]).value();
    // Toggle it
    enabled = !enabled;
    // Write back settings with toggled flag
    json.print(js::FileDestination{ jsonSettingsFile() });
  } catch (std::exception &ex) {
    PLOG_ERROR << "Error: " << ex.what();
  }
  PLOG_INFO << "JSON_Toggle_Setting exited.";
  exit(EXIT_SUCCESS);
}