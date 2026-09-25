#include "JSON_Lib_Tests.hpp"

#if defined(JSON_LIB_JSON_TEST_SUITE_PATH)

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

struct TestSuiteFile {
  fs::path path;
  std::string filename;
};

std::vector<TestSuiteFile> getTestSuiteFiles(const std::string &prefix)
{
  std::vector<TestSuiteFile> result;
  std::error_code ec;
  const fs::path dirPath{ JSON_LIB_JSON_TEST_SUITE_PATH };

  if (!fs::is_directory(dirPath, ec) || ec) {
    return result;
  }

  for (const auto &entry : fs::directory_iterator(dirPath, ec)) {
    if (ec) { break; }
    if (entry.is_regular_file()) {
      const auto filename = entry.path().filename().string();
      if (filename.rfind(prefix, 0) == 0 && entry.path().extension() == ".json") {
        result.push_back({ entry.path(), filename });
      }
    }
  }

  std::sort(result.begin(), result.end(), [](const TestSuiteFile &a, const TestSuiteFile &b) {
    return a.filename < b.filename;
  });

  return result;
}

struct ParseOutcome {
  bool accepted = false;
  std::string errorMessage;
};

ParseOutcome parseTestFile(const fs::path &filePath)
{
  ParseOutcome outcome;
  std::ifstream in(filePath, std::ios::binary);
  if (!in.is_open()) {
    outcome.accepted = false;
    outcome.errorMessage = "Failed to open file: " + filePath.string();
    return outcome;
  }

  const std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

  try {
    BufferSource src{ content };
    JSON json;
    json.parse(src);
    outcome.accepted = true;
  } catch (const std::exception &ex) {
    outcome.accepted = false;
    outcome.errorMessage = ex.what();
  } catch (...) {
    outcome.accepted = false;
    outcome.errorMessage = "Unknown non-standard exception during parse.";
  }

  return outcome;
}

bool isDocumentedDesignDeviation(const std::string &filename)
{
  return filename == "y_object_duplicated_key.json" ||
         filename == "y_object_duplicated_key_and_value.json" ||
         filename == "y_object_escaped_null_in_key.json" ||
         filename == "y_string_null_escape.json" ||
         filename == "n_string_unescaped_ctrl_char.json" ||
         filename == "n_string_unescaped_newline.json" ||
         filename == "n_string_unescaped_tab.json";
}

} // namespace

TEST_CASE("Official JSONTestSuite - Required Valid Documents (y_*)", "[JSON][JSONTestSuite][valid]")
{
  const auto validFiles = getTestSuiteFiles("y_");
  INFO("Testing " << validFiles.size() << " valid JSONTestSuite files from: " << JSON_LIB_JSON_TEST_SUITE_PATH);
  REQUIRE_FALSE(validFiles.empty());

  for (const auto &testFile : validFiles) {
    const auto outcome = parseTestFile(testFile.path);
    INFO("File: " << testFile.filename << (outcome.errorMessage.empty() ? "" : " | " + outcome.errorMessage));
    if (isDocumentedDesignDeviation(testFile.filename)) {
      // Documented deliberate security deviation (see docs/conformance.md §3.1 and §3.2):
      // - Duplicate keys are rejected for strict object key uniqueness
      // - Escaped null characters are rejected to prevent C-string truncation attacks
      CHECK_FALSE(outcome.accepted);
      if (testFile.filename.find("duplicated_key") != std::string::npos) {
        CHECK(outcome.errorMessage.find("Duplicate key") != std::string::npos);
      } else if (testFile.filename.find("null") != std::string::npos) {
        CHECK(outcome.errorMessage.find("null character") != std::string::npos);
      }
    } else {
      CHECK(outcome.accepted);
    }
  }
}

TEST_CASE("Official JSONTestSuite - Required Invalid Documents (n_*)", "[JSON][JSONTestSuite][invalid]")
{
  const auto invalidFiles = getTestSuiteFiles("n_");
  INFO("Testing " << invalidFiles.size() << " invalid JSONTestSuite files from: " << JSON_LIB_JSON_TEST_SUITE_PATH);
  REQUIRE_FALSE(invalidFiles.empty());

  for (const auto &testFile : invalidFiles) {
    const auto outcome = parseTestFile(testFile.path);
    INFO("File: " << testFile.filename << " was expected to be rejected by parser but was accepted.");
    if (isDocumentedDesignDeviation(testFile.filename)) {
      // Documented deliberate dialect choice (see docs/conformance.md §3.3):
      // Permissive acceptance of unescaped control characters for XML/YAML translator compatibility
      CHECK(outcome.accepted);
    } else {
      CHECK_FALSE(outcome.accepted);
    }
  }
}

TEST_CASE("Official JSONTestSuite - Implementation Defined Documents (i_*)", "[JSON][JSONTestSuite][implementation_defined]")
{
  const auto implFiles = getTestSuiteFiles("i_");
  INFO("Testing " << implFiles.size() << " implementation-defined JSONTestSuite files from: " << JSON_LIB_JSON_TEST_SUITE_PATH);
  REQUIRE_FALSE(implFiles.empty());

  for (const auto &testFile : implFiles) {
    const auto outcome = parseTestFile(testFile.path);
    // Implementation-defined documents may be accepted or rejected, but must complete without crashing
    INFO("File: " << testFile.filename << " completed with outcome: " << (outcome.accepted ? "accepted" : "rejected"));
    CHECK(true);
  }
}

#endif // JSON_LIB_JSON_TEST_SUITE_PATH
