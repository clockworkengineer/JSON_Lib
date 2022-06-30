//
// Program: Analyze_JSON_File
//
// Description:
//
// Dependencies: C20++, JSONLib.
//
// =============
// INCLUDE FILES
// =============
// =======
// C++ STL
// =======
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <format>
#include <chrono>
#include <set>
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
#include "plog/Log.h"
#include "plog/Initializers/RollingFileInitializer.h"
// ====================
// JSON class namespace
// ====================
using namespace JSONLib;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
//
// JNode Note Tree Details
//
struct JNodeDetails
{
    int64_t totalNodes{};
    int64_t sizeInBytes{};
    int64_t totalKeys{};
    std::set<std::string> unique_keys{};
    int64_t totalStrings{};
    std::set<std::string> unique_strings{};
};
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
/// Output JNode tree details
/// </summary>
/// <param name="jNodeDetails">result of JNode tree analysis</param>
void outputAnalysis(const JNodeDetails &jNodeDetails)
{
    PLOG_INFO << std::format("--------------------JNode Sizes---------------------");
    PLOG_INFO << std::format("JNodeObject size {} in bytes.", sizeof(JNodeObject));
    PLOG_INFO << std::format("JNodeArray size {} in bytes.", sizeof(JNodeArray));
    PLOG_INFO << std::format("JNodeNumber size {} in bytes.", sizeof(JNodeNumber));
    PLOG_INFO << std::format("JNodeString size {} in bytes.", sizeof(JNodeString));
    PLOG_INFO << std::format("JNodeBoolean size {} in bytes.", sizeof(JNodeBoolean));
    PLOG_INFO << std::format("JNodeNull size {} in bytes.", sizeof(JNodeNull));
    PLOG_INFO << std::format("------------------JNode Tree Stats------------------");
    PLOG_INFO << std::format("JNode Tree contains {} nodes.", jNodeDetails.totalNodes);
    PLOG_INFO << std::format("JNode Tree size {} in bytes.", jNodeDetails.sizeInBytes);
    PLOG_INFO << std::format("JNode total {} keys.", jNodeDetails.totalKeys);
    PLOG_INFO << std::format("JNode contains {} unique keys.", jNodeDetails.unique_keys.size());
    PLOG_INFO << std::format("JNode total {} strings.", jNodeDetails.totalStrings);
    PLOG_INFO << std::format("JNode contains {} unique strings.", jNodeDetails.unique_strings.size());
    PLOG_INFO << std::format("----------------------------------------------------");
}
/// <summary>
/// Recursively analyzes JNode tree details
/// </summary>
/// <param name="jNode">Current JNode</param>
/// <param name="jNodeDetails">Result of JNode tree analysis</param>
void analyzeJNodes(const JNode &jNode, JNodeDetails &jNodeDetails)
{
    jNodeDetails.totalNodes++;
    switch (jNode.getNodeType())
    {
    case JNodeType::number:
        jNodeDetails.sizeInBytes += sizeof(JNodeNumber);
        jNodeDetails.sizeInBytes += JNodeRef<JNodeNumber>(jNode).number().size();
        break;
    case JNodeType::string:
        jNodeDetails.sizeInBytes += sizeof(JNodeString);
        jNodeDetails.sizeInBytes += JNodeRef<JNodeString>(jNode).string().size();
        jNodeDetails.unique_strings.insert(JNodeRef<JNodeString>(jNode).string());
        jNodeDetails.totalStrings++;
        break;
    case JNodeType::boolean:
        jNodeDetails.sizeInBytes += sizeof(JNodeBoolean);
        break;
    case JNodeType::null:
        jNodeDetails.sizeInBytes += sizeof(JNodeNull);
        break;
    case JNodeType::object:
    {
        jNodeDetails.sizeInBytes += sizeof(JNodeObject);
        for (auto &[key, jNodePtr] : JNodeRef<JNodeObject>(jNode).objects())
        {
            jNodeDetails.unique_keys.insert(key);
            analyzeJNodes(JNodeRef<JNodeObject>(jNode)[key], jNodeDetails);
            jNodeDetails.sizeInBytes += key.size();
            jNodeDetails.sizeInBytes += sizeof(JNodeObject::KeyValuePair);
            jNodeDetails.totalKeys++;
        }
        break;
    }
    case JNodeType::array:
    {
        jNodeDetails.sizeInBytes += sizeof(JNodeArray);
        for (auto &bNodeEntry : JNodeRef<JNodeArray>(jNode).array())
        {
            analyzeJNodes(JNodeRef<JNode>(*bNodeEntry), jNodeDetails);
            jNodeDetails.sizeInBytes += sizeof(JNode::Ptr);
        }
        break;
    }
    default:
        throw Error("Unknown JNode type encountered during stringification.");
    }
}
/// <summary>
/// Parse JSON file and analyze its JNode Tree.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName)
{
    std::cout << std::format("Analyzing {}\n", fileName);
    PLOG_INFO << std::format("Analyzing {}", fileName);
    JSON json;
    JNodeDetails jNodeDetails;
    json.parse(FileSource{fileName});
    analyzeJNodes(json.root(), jNodeDetails);
    outputAnalysis(jNodeDetails);
    PLOG_INFO << std::format("Finished {}.", fileName);
    std::cout << std::format("Finished {}.\n", fileName);
}
// ============================
// ===== MAIN ENTRY POINT =====
// ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    const std::vector<std::string> fileList{
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
    plog::init(plog::debug, "Analyze_JSON_File.log");
    PLOG_INFO << "Analyze_JSON_File started ...";
    PLOG_INFO << JSON().version();
    //
    // Analyze JSON file and display its details.
    //
    for (auto &fileName : fileList)
    {
        try
        {
            if (const auto fullFileName{prefixTestDataPath(fileName)}; std::filesystem::exists(fullFileName))
            {
                processJSONFile(fullFileName);
            }
            else
            {
                PLOG_INFO << std::format("File {} not present.", fullFileName);
            }
        }
        catch (std::exception &ex)
        {
            std::cout << ex.what() << "\n";
        }
    }
    exit(EXIT_SUCCESS);
}