//
// Program: Parse_JSON_Files
//
// Description:
//
// Dependencies: C20++, JSONLib.
//
// =============
// INCLUDE FILES
// =============
//
// C++ STL
//
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <format>
//
// JSON includes
//
#include "JSON.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
//
// Logging
//
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
// ====================
// JSON class namespace
// ====================
using namespace JSONLib;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
static constexpr size_t kMaxFileLengthToDisplay = 16 * 1024;
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Prefix current path to test data file name.
/// </summary>
/// <param name="name">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &file)
{
    return ((std::filesystem::current_path() / "testData" / file).string());
}
/// <summary>
/// Parse JSON file and display it reconstituted via stringify.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
    JSON json;
    BufferDestination jsonDestination;
    PLOG_INFO << fileName;
    json.parse(FileSource{fileName});
    json.stringify(jsonDestination);
    if (jsonDestination.getBuffer().size() < kMaxFileLengthToDisplay)
    {
        PLOG_INFO << "[" << jsonDestination.getBuffer();
    }
    PLOG_INFO << "----------------------------OK";
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    std::vector<std::string> fileList{
        "testfile001.json",
        "testfile002.json",
        "testfile003.json",
        "testfile004.json",
        "testfile005.json",
        "large-file.json" // Not kept in GitHub as 24Meg in size.
    };
    //
    // Initialise logging.
    //
    plog::init(plog::debug, "Parse_JSON_Files.log");
    PLOG_INFO << "Parse_JSON_Files started ...";
    PLOG_INFO << JSON().version();
    //
    // For each json parse it, stringify it and display unless its to large.
    //
    for (const auto &fileName : fileList)
    {
        try
        {
            if (const std::string fullFileName{prefixTestDataPath(fileName)}; std::filesystem::exists(fullFileName))
            {
                processJSONFile(fullFileName);
            }
            else
            {
                PLOG_INFO << std::format("File {} not present.", fileName);
            }
        }
        catch (std::exception &ex)
        {
            std::cout << ex.what() << "\n";
        }
    }
    exit(EXIT_SUCCESS);
}