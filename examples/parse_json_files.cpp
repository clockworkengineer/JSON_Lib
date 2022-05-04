//
// Program: parse_json_files
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
//
// JSON includes
//
#include "JSON.hpp"
#include "JSONSources.hpp"
#include "JSONDestinations.hpp"
// =======================
// Bencode class namespace
// =======================
using namespace JSONLib;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
constexpr size_t kMaxFileLengthToDisplay = 16 * 1024;
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
    std::filesystem::path currentPath = std::filesystem::current_path() / "testData" / file;
    return (currentPath.string());
}
/// <summary>
/// Parse JSON file and display it reconstituted via stringify.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
    JSON json;
    BufferDestination jsonDestination;
    std::cout << fileName << "\n";
    json.parse(FileSource{fileName});
    json.stringify(jsonDestination);
    if (jsonDestination.getBuffer().size() < kMaxFileLengthToDisplay)
    {
        std::cout << "[" << jsonDestination.getBuffer() << "]\n";
    }
    std::cout << "----------------------------OK\n";
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    std::vector<std::string> fileList{"testfile001.json",
                                      "testfile002.json",
                                      "testfile003.json",
                                      "testfile004.json",
                                      "testfile005.json",
                                      "large-file.json" // Not kept in GitHub as 24Meg in size.
                                      };
    //
    // For each json parse it, stringify it and display unless its to large.
    //
    for (auto &fileName : fileList)
    {
        try
        {
            if (std::string fullFileName = {prefixTestDataPath(fileName)}; std::filesystem::exists(fullFileName))
            {
                processJSONFile(fullFileName);
            }
        }
        catch (std::exception &ex)
        {
            std::cout << ex.what() << "\n";
        }
    }
    exit(EXIT_SUCCESS);
}