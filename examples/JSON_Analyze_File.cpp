//
// Program: JSON_Analyze_File
//
// Description: For a JSON file parse it then analyze its JNode tree
// and produce a output report of the analysis.
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
#include <set>
// ====
// JSON
// ====
#include "IAction.hpp"
#include "JSON.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Types.hpp"
// =======
// Logging
// =======
#include "plog/Initializers/RollingFileInitializer.h"
#include "plog/Log.h"
// ====================
// JSON class namespace
// ====================
using namespace JSONLib;
// ========================
// LOCAL TYPES/DEFINITIONS
// ========================
//
// JSON Tree Details
//
class JSONDetails : public IAction {
public:
  // ========================
  // Constructors/destructors
  // ========================
  JSONDetails() = default;
  JSONDetails(const JSONDetails &other) = delete;
  JSONDetails &operator=(const JSONDetails &other) = delete;
  JSONDetails(IAction &&JNodeDetails) = delete;
  JSONDetails &operator=(JSONDetails &&other) = delete;
  virtual ~JSONDetails() = default;
  // Add string details to analysis
  virtual void onString(const String &jNodeString) override {
    totalNodes++;
    sizeInBytes += sizeof(String);
    sizeInBytes += jNodeString.string().size();
    unique_strings.insert(jNodeString.string());
    totalStrings++;
  }
  // Add number details to analysis
  virtual void onNumber([[maybe_unused]] const Number &jNodeNumber) override {
    totalNodes++;
    sizeInBytes += sizeof(Number);
  }
  virtual void
  onBoolean([[maybe_unused]] const Boolean &jNodeBoolean) override {
    totalNodes++;
    sizeInBytes += sizeof(Boolean);
  }
  // Add null details to analysis
  virtual void onNull([[maybe_unused]] const Null &jNodeNull) override {
    totalNodes++;
    sizeInBytes += sizeof(Null);
  }
  // Add array details to analysis
  virtual void onArray(const Array &jNodeArray) override {
    totalNodes++;
    sizeInBytes += sizeof(Array);
    totalArrays++;
    maxArraySize = std::max(jNodeArray.size(), maxArraySize);
    for ([[maybe_unused]] auto &bNodeEntry : jNodeArray.array()) {
      sizeInBytes += sizeof(JNode);
    }
  }
  // Add object details to analysis
  virtual void onObject(const Object &jNodeObject) override {
    totalNodes++;
    sizeInBytes += sizeof(Object);
    totalObjects++;
    maxObjectSize = std::max(jNodeObject.objectEntries().size(), maxObjectSize);
    for (auto &[key, node] : jNodeObject.objectEntries()) {
      unique_keys.insert(key);
      sizeInBytes += key.size();
      sizeInBytes += sizeof(Object::Entry);
      totalKeys++;
    }
  }
  // Output analysis details
  void output() {
    PLOG_INFO << "--------------------JNode Sizes---------------------";
    PLOG_INFO << "JNode size " << sizeof(JNode) << " in bytes.";
    PLOG_INFO << "Object size " << sizeof(Object) << " in bytes.";
    PLOG_INFO << "Array size " << sizeof(Array) << " in bytes.";
    PLOG_INFO << "Numeric size " << sizeof(Numeric) << " in bytes.";
    PLOG_INFO << "Number size " << sizeof(Number) << " in bytes.";
    PLOG_INFO << "String size " << sizeof(String) << " in bytes.";
    PLOG_INFO << "Boolean size " << sizeof(Boolean) << " in bytes.";
    PLOG_INFO << "Null size " << sizeof(Null) << " in bytes.";
    PLOG_INFO << "------------------JNode Tree Stats------------------";
    PLOG_INFO << "JNode Tree contains " << totalNodes << " nodes.";
    PLOG_INFO << "JNode Tree size " << sizeInBytes << " in bytes.";
    PLOG_INFO << "JNode Tree total " << totalKeys << " keys.";
    PLOG_INFO << "JNode Tree contains " << unique_keys.size()
              << " unique keys.";
    PLOG_INFO << "JNode Tree total " << totalStrings << " strings.";
    PLOG_INFO << "JNode Tree contains " << unique_strings.size()
              << " unique strings.";
    PLOG_INFO << "JNode Tree contains " << totalArrays << " arrays.";
    PLOG_INFO << "JNode Tree max array size " << maxArraySize << ".";
    PLOG_INFO << "JNode Tree contains " << totalObjects << " objectEntries.";
    PLOG_INFO << "JNode Tree max object size " << maxObjectSize << ".";
    PLOG_INFO << "----------------------------------------------------";
  }

private:
  // JSON analysis data
  int64_t totalNodes{};
  size_t sizeInBytes{};
  int64_t totalKeys{};
  std::set<std::string> unique_keys{};
  int64_t totalStrings{};
  std::set<std::string> unique_strings{};
  size_t maxArraySize{};
  int64_t totalArrays{};
  size_t maxObjectSize{};
  int64_t totalObjects{};
};
// ===============
// LOCAL FUNCTIONS
// ===============
/// <summary>
/// Prefix current path to test data file name.
/// </summary>
/// <param name="name">Test data file name</param>
/// <returns>Full path to test data file</returns>
std::string prefixTestDataPath(const std::string &file) {
  return ((std::filesystem::current_path() / "files" / file).string());
}
/// <summary>
/// Parse JSON file and analyze its JNode Tree.
/// </summary>
/// <param name="fileName">JSON file name</param>
void processJSONFile(const std::string &fileName) {

  PLOG_INFO << "Analyzing " << fileName;
  const JSON json;
  JSONDetails jsonDetails;
  json.parse(FileSource{fileName});
  json.traverse(jsonDetails);
  jsonDetails.output();
  PLOG_INFO << "Finished " << fileName << ".";
}
// // ============================
// // ===== MAIN ENTRY POINT =====
// // ============================
int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  const std::vector<std::string> fileList{
      "testfile001.json", "testfile002.json", "testfile003.json",
      "testfile004.json", "testfile005.json",
      "large-file.json" // Not kept in GitHub as 24Meg in size.
  };
  //
  // Initialise logging.
  //
  plog::init(plog::debug, "JSON_Analyze_File.log");
  PLOG_INFO << "JSON_Analyze_File started ...";
  PLOG_INFO << JSON().version();
  //
  // Analyze JSON file and display its details.
  //
  for (auto &fileName : fileList) {
    try {
      if (const auto fullFileName{prefixTestDataPath(fileName)};
          std::filesystem::exists(fullFileName)) {
        std::cout << "Analyzing " << fullFileName << "\n";
        processJSONFile(fullFileName);
        std::cout << "Finished " << fullFileName << ".\n";
      } else {
        std::cout << "File " << fullFileName << " not present.\n";
      }
    } catch (std::exception &ex) {
      std::cout << ex.what() << "\n";
    }
  }
  PLOG_INFO << "JSON_Analyze_File exited.";
  exit(EXIT_SUCCESS);
}