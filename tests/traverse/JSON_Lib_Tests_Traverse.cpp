//
// Unit Tests: JSON_Lib_Tests_Travsere
//
// Description: JSON traversal of unit tests for JSON class
// using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

class JSON_Analyzer : public JSON_Lib::IAction
{
public:
  JSON_Analyzer() = default;
  virtual ~JSON_Analyzer() = default;
  // Add JNode details to analysis
  virtual void onJNode([[maybe_unused]] const JSON_Lib::JNode &jNode) override { totalNodes++; }
  // Add string details to analysis
  virtual void onString([[maybe_unused]] const JSON_Lib::JNode &jNode) override { totalStrings++; }
  // Add number details to analysis
  virtual void onNumber(const JSON_Lib::JNode &jNode) override
  {
    const JSON_Lib::Number &jNodeNumber = JRef<JSON_Lib::Number>(jNode);
    totalNumbers++;

    if (jNodeNumber.is<int>()) {
      totalInteger++;
    } else if (jNodeNumber.is<long>()) {
      totalLong++;
    } else if (jNodeNumber.is<long long>()) {
      totalLongLong++;
    } else if (jNodeNumber.is<float>()) {
      totalFloat++;
    } else if (jNodeNumber.is<double>()) {
      totalDouble++;
    } else if (jNodeNumber.is<long double>()) {
      totalLongDouble++;
    }
  }
  virtual void onBoolean([[maybe_unused]] const JSON_Lib::JNode &jNode) override { totalBoolean++; }
  // Add null details to analysis
  virtual void onNull([[maybe_unused]] const JSON_Lib::JNode &jNode) override { totalNull++; }
  // Add array details to analysis
  virtual void onArray([[maybe_unused]] const JSON_Lib::JNode &jNode) override { totalArrays++; }
  // Add object details to analysis
  virtual void onObject([[maybe_unused]] const JSON_Lib::JNode &jNode) override { totalObjects++; }
  // Non-const api not used
  virtual void onNumber([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onBoolean([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onNull([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onArray([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onObject([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onJNode([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onString([[maybe_unused]] JSON_Lib::JNode &jNode) override {}

  // JSON analysis data
  // Node
  int64_t totalNodes{};
  // JSON_Lib::Object
  int64_t totalObjects{};
  // JSON_Lib::Array
  int64_t totalArrays{};
  // JSON_Lib::String
  int64_t totalStrings{};
  // JSON_Lib::Number
  int64_t totalNumbers{};
  int64_t totalInteger{};
  int64_t totalLong{};
  int64_t totalLongLong{};
  int64_t totalFloat{};
  int64_t totalDouble{};
  int64_t totalLongDouble{};
  // JSON_Lib::Boolean
  int64_t totalBoolean{};
  // JSON_Lib::Null
  int64_t totalNull{};
};

TEST_CASE("JSON BNode tree traverse tests ", "[JSON][Traverse]")
{
  const JSON bEncode;
  SECTION("Decode an Integer (266) and traverse", "[JSON][Traverse][Integer]")
  {
    BufferSource source{ "266" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalInteger == 1);
    REQUIRE(analyzer.totalNumbers == 1);
    REQUIRE(analyzer.totalObjects == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalArrays == 0);
  }

  SECTION("Decode an Integer (-266) and traverse", "[JSON][Traverse][Integer]")
  {
    BufferSource source{ "-266" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalInteger == 1);
    REQUIRE(analyzer.totalNumbers == 1);
    REQUIRE(analyzer.totalObjects == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalArrays == 0);
  }

  SECTION("Decode an Long (2147483647) and traverse", "[JSON][Traverse][Long]")
  {
    BufferSource source{ "2147483647" };// sizeof(int) == sizeof(long)
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalInteger == 1);
    REQUIRE(analyzer.totalNumbers == 1);
    REQUIRE(analyzer.totalObjects == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalArrays == 0);
  }
  SECTION("Decode an Long (-2147483648) and traverse", "[JSON][Traverse][Long]")
  {
    BufferSource source{ "-2147483648" };// sizeof(int) == sizeof(long)
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalInteger == 1);
    REQUIRE(analyzer.totalNumbers == 1);
    REQUIRE(analyzer.totalObjects == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalArrays == 0);
  }
  SECTION("Decode an LongLong (2147483648) and traverse", "[JSON][Traverse][LongLong]")
  {
    BufferSource source{ "2147483648" };// INT_MAX+1
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalInteger == 0);
    REQUIRE(analyzer.totalLongLong == 1);
    REQUIRE(analyzer.totalNumbers == 1);
    REQUIRE(analyzer.totalObjects == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalArrays == 0);
  }

  SECTION("Decode an LongLong (-2147483649) and traverse", "[JSON][Traverse][LongLong]")
  {
    BufferSource source{ "-2147483649" };// INT_MIN-1
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalInteger == 0);
    REQUIRE(analyzer.totalLongLong == 1);
    REQUIRE(analyzer.totalNumbers == 1);
    REQUIRE(analyzer.totalObjects == 0);
    REQUIRE(analyzer.totalStrings == 0);
    REQUIRE(analyzer.totalArrays == 0);
  }
}