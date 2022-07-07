// //
// // Program: JSON_Analyze_File
// //
// // Description: For a JSON file parse it then analyze its JNode tree
// // and produce a output report of the analysis.
// //
// // Dependencies: C20++, PLOG, JSONLib.
// //
// // =============
// // INCLUDE FILES
// // =============
// // =======
// // C++ STL
// // =======
// #include <cstdlib>
// #include <iostream>
// #include <filesystem>
// #include <chrono>
// #include <set>
// // ====
// // JSON
// // ====
// #include "JSON.hpp"
// #include "JSON_Types.hpp"
// #include "JSON_Sources.hpp"
// #include "JSON_Destinations.hpp"
// // =======
// // Logging
// // =======
// #include "plog/Log.h"
// #include "plog/Initializers/RollingFileInitializer.h"
// // ====================
// // JSON class namespace
// // ====================
// using namespace JSONLib;
// // ========================
// // LOCAL TYPES/DEFINITIONS
// // ========================
// //
// // JNode Tree Details
// //
// struct JNodeDetails
// {
//     int64_t totalNodes{};
//     size_t sizeInBytes{};
//     int64_t totalKeys{};
//     std::set<std::string> unique_keys{};
//     int64_t totalStrings{};
//     std::set<std::string> unique_strings{};
//     size_t maxArraySize{};
//     int64_t totalArrays{};
//     size_t maxObjectSize{};
//     int64_t totalObjects{};
// };
// // ===============
// // LOCAL FUNCTIONS
// // ===============
// /// <summary>
// /// Prefix current path to test data file name.
// /// </summary>
// /// <param name="name">Test data file name</param>
// /// <returns>Full path to test data file</returns>
// std::string prefixTestDataPath(const std::string &file)
// {
//     return ((std::filesystem::current_path() / "files" / file).string());
// }
// /// <summary>
// /// Output JNode tree details.
// /// </summary>
// /// <param name="jNodeDetails">result of JNode tree analysis</param>
// void outputAnalysis(const JNodeDetails &jNodeDetails)
// {
//     PLOG_INFO << "--------------------JNode Sizes---------------------";
//     PLOG_INFO << "JNodeObject size "<< sizeof(JNodeObject) << "in bytes.";
//     PLOG_INFO << "JNodeArray size "<< sizeof(JNodeArray) << " in bytes.";
//     PLOG_INFO << "JNodeNumber size "<< sizeof(JNodeNumber) << " in bytes.";
//     PLOG_INFO << "JNodeString size "<< sizeof(JNodeString) << " in bytes.";
//     PLOG_INFO << "JNodeBoolean size "<< sizeof(JNodeBoolean) << " in bytes.";
//     PLOG_INFO << "JNodeNull size "<< sizeof(JNodeNull) << " in bytes.";
//     PLOG_INFO << "------------------JNode Tree Stats------------------";
//     PLOG_INFO << "JNode Tree contains "<<jNodeDetails.totalNodes << " nodes.";
//     PLOG_INFO << "JNode Tree size "<< jNodeDetails.sizeInBytes << " in bytes.";
//     PLOG_INFO << "JNode Tree total "<< jNodeDetails.totalKeys << " keys.";
//     PLOG_INFO << "JNode Tree contains "<< jNodeDetails.unique_keys.size()<< " unique keys.";
//     PLOG_INFO << "JNode Tree total "<< jNodeDetails.totalStrings << " strings.";
//     PLOG_INFO << "JNode Tree contains "<<jNodeDetails.unique_strings.size() << " unique strings.";
//     PLOG_INFO << "JNode Tree contains "<< jNodeDetails.totalArrays << " arrays.";
//     PLOG_INFO << "JNode Tree max array size "<<jNodeDetails.maxArraySize << ".";
//     PLOG_INFO << "JNode Tree contains "<< jNodeDetails.totalObjects << " objects.";
//     PLOG_INFO << "JNode Tree max object size "<< jNodeDetails.maxObjectSize<< ".";
//     PLOG_INFO << "----------------------------------------------------";
// }
// /// <summary>
// /// Recursively analyzes JNode tree.
// /// </summary>
// /// <param name="jNode">Current JNode</param>
// /// <param name="jNodeDetails">Result of JNode tree analysis</param>
// void analyzeJNode(const JNode &jNode, JNodeDetails &jNodeDetails)
// {
//     jNodeDetails.totalNodes++;
//     switch (jNode.getNodeType())
//     {
//     case JNodeType::number:
//         jNodeDetails.sizeInBytes += sizeof(JNodeNumber);
//         jNodeDetails.sizeInBytes += JNodeRef<JNodeNumber>(jNode).number().size();
//         break;
//     case JNodeType::string:
//         jNodeDetails.sizeInBytes += sizeof(JNodeString);
//         jNodeDetails.sizeInBytes += JNodeRef<JNodeString>(jNode).string().size();
//         jNodeDetails.unique_strings.insert(JNodeRef<JNodeString>(jNode).string());
//         jNodeDetails.totalStrings++;
//         break;
//     case JNodeType::boolean:
//         jNodeDetails.sizeInBytes += sizeof(JNodeBoolean);
//         break;
//     case JNodeType::null:
//         jNodeDetails.sizeInBytes += sizeof(JNodeNull);
//         break;
//     case JNodeType::object:
//     {
//         jNodeDetails.sizeInBytes += sizeof(JNodeObject);
//         jNodeDetails.totalObjects++;
//         jNodeDetails.maxObjectSize = std::max(JNodeRef<JNodeObject>(jNode).objects().size(), jNodeDetails.maxObjectSize);
//         for (auto &[key, jNodePtr] : JNodeRef<JNodeObject>(jNode).objects())
//         {
//             analyzeJNode(JNodeRef<JNodeObject>(jNode)[key], jNodeDetails);
//             jNodeDetails.unique_keys.insert(key);
//             jNodeDetails.sizeInBytes += key.size();
//             jNodeDetails.sizeInBytes += sizeof(JNodeObjectEntry);
//             jNodeDetails.totalKeys++;
//         }
//         break;
//     }
//     case JNodeType::array:
//     {
//         jNodeDetails.sizeInBytes += sizeof(JNodeArray);
//         jNodeDetails.totalArrays++;
//         jNodeDetails.maxArraySize = std::max(JNodeRef<JNodeArray>(jNode).array().size(), jNodeDetails.maxArraySize);
//         for (auto &bNodeEntry : JNodeRef<JNodeArray>(jNode).array())
//         {
//             analyzeJNode(JNodeRef<JNode>(*bNodeEntry), jNodeDetails);
//             jNodeDetails.sizeInBytes += sizeof(JNode::Ptr);
//         }
//         break;
//     }
//     default:
//         throw Error("Unknown JNode type encountered during stringification.");
//     }
// }
// /// <summary>
// /// Analyzes JNode tree and outputs its details.
// /// </summary>
// /// <param name="jNode">Current JNode</param>
// /// <param name="jNodeDetails">Root of JNode</param>
// void analyzeJNodeTree(const JNode &jNodeRoot)
// {
//     JNodeDetails jNodeDetails;
//     analyzeJNode(jNodeRoot, jNodeDetails);
//     outputAnalysis(jNodeDetails);
// }
// /// <summary>
// /// Parse JSON file and analyze its JNode Tree.
// /// </summary>
// /// <param name="fileName">JSON file name</param>
// void processJSONFile(const std::string &fileName)
// {
//     std::cout << "Analyzing "<< fileName << "\n";
//     PLOG_INFO << "Analyzing "<< fileName;
//     JSON json;
//     json.parse(FileSource{fileName});
//     analyzeJNodeTree(json.root());
//     PLOG_INFO << "Finished "<< fileName << ".";
//     std::cout << "Finished " << fileName << ".\n";
// }
// // ============================
// // ===== MAIN ENTRY POINT =====
// // ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
//     const std::vector<std::string> fileList{
//         "testfile001.json",
//         "testfile002.json",
//         "testfile003.json",
//         "testfile004.json",
//         "testfile005.json",
//         "large-file.json" // Not kept in GitHub as 24Meg in size.
//     };
//     //
//     // Initialise logging.
//     //
//     plog::init(plog::debug, "Analyze_JSON_File.log");
//     PLOG_INFO << "Analyze_JSON_File started ...";
//     PLOG_INFO << JSON().version();
//     //
//     // Analyze JSON file and display its details.
//     //
//     for (auto &fileName : fileList)
//     {
//         try
//         {
//             if (const auto fullFileName{prefixTestDataPath(fileName)}; std::filesystem::exists(fullFileName))
//             {
//                 processJSONFile(fullFileName);
//             }
//             else
//             {
//                 PLOG_INFO << "File "<< fullFileName << " not present.";
//             }
//         }
//         catch (std::exception &ex)
//         {
//             std::cout << ex.what() << "\n";
//         }
//     }
    // exit(EXIT_SUCCESS);
}