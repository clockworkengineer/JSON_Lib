//
// Program: parse_json_file
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
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Prefix path to test data file name.
/// </summary>
/// <param name="name">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &file)
{
    std::filesystem::path currentPath = std::filesystem::current_path() / "testData" / file;
    return (currentPath.string());
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
                                      "large-file.json"};
    JSON json;
    //
    // For each torrent file extract its information and display
    //
    for (auto &fileName : fileList)
    {
        try
        {
            BufferDestination jsonDestination;
            std::cout << prefixTestDataPath(fileName) << "\n";
            json.parse(FileSource{prefixTestDataPath(fileName)});
            json.stringify(jsonDestination);
            // std::cout << "[" << jsonDestination.getBuffer() << "]\n";
            std::cout << "----------------------------OK\n";
        }
        catch (std::exception &ex)
        {
            std::cout << ex.what() << "\n";
        }
    }
    exit(EXIT_SUCCESS);
}