//
// Unit Tests: JSON_Lib_Tests_Traverse
//
// Description: Traversal unit tests for JSON class
// using the Catch2 test framework.
//

#include "JSON_Lib_Tests.hpp"

using namespace JSON_Lib;

class JSON_Analyzer : public IAction
{
public:
  JSON_Analyzer() = default;
  virtual ~JSON_Analyzer() = default;
  // Add JNode details to analysis
  virtual void onJNode([[maybe_unused]] const JNode &jNode) override { totalNodes++; }
  // Add string details to analysis
  virtual void onString([[maybe_unused]] const JNode &jNode) override { totalStrings++; }
  // Add number details to analysis
  virtual void onNumber(const JNode &jNode) override
  {
    const Number &jNodeNumber = JRef<Number>(jNode);
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
  virtual void onBoolean([[maybe_unused]] const JNode &jNode) override { totalBoolean++; }
  // Add null details to analysis
  virtual void onNull([[maybe_unused]] const JNode &jNode) override { totalNull++; }
  // Add array details to analysis
  virtual void onArray([[maybe_unused]] const JNode &jNode) override { totalArrays++; }
  // Add object details to analysis
  virtual void onObject([[maybe_unused]] const JNode &jNode) override { totalObjects++; }
  // Non-const api not used
  virtual void onNumber([[maybe_unused]] JNode &jNode) override {}
  virtual void onBoolean([[maybe_unused]] JNode &jNode) override {}
  virtual void onNull([[maybe_unused]] JNode &jNode) override {}
  virtual void onArray([[maybe_unused]] JNode &jNode) override {}
  virtual void onObject([[maybe_unused]] JNode &jNode) override {}
  virtual void onJNode([[maybe_unused]] JNode &jNode) override {}
  virtual void onString([[maybe_unused]] JNode &jNode) override {}

  // JSON analysis data
  // Node
  int64_t totalNodes{};
  // Object
  int64_t totalObjects{};
  // Array
  int64_t totalArrays{};
  // String
  int64_t totalStrings{};
  // Number
  int64_t totalNumbers{};
  int64_t totalInteger{};
  int64_t totalLong{};
  int64_t totalLongLong{};
  int64_t totalFloat{};
  int64_t totalDouble{};
  int64_t totalLongDouble{};
  // Boolean
  int64_t totalBoolean{};
  // Null
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
  }
  SECTION("Decode an LongLong (2147483648) and traverse", "[JSON][Traverse][LongLong]")
  {
    BufferSource source{ "2147483648" };// INT_MAX+1
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    if constexpr (sizeof(int) == sizeof(long)) {
      REQUIRE(analyzer.totalLongLong == 1);
    } else {
      REQUIRE(analyzer.totalLong == 1);
    }
    REQUIRE(analyzer.totalNumbers == 1);
  }

  SECTION("Decode an LongLong (-2147483649) and traverse", "[JSON][Traverse][LongLong]")
  {
    BufferSource source{ "-2147483649" };// INT_MIN-1
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    if constexpr (sizeof(int) == sizeof(long)) {
      REQUIRE(analyzer.totalLongLong == 1);
    } else {
      REQUIRE(analyzer.totalLong == 1);
    }
    REQUIRE(analyzer.totalNumbers == 1);
  }
  SECTION("Decode an float (32.11999) and traverse", "[JSON][Traverse][Float]")
  {
    BufferSource source{ "32.11999" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalFloat == 1);
    REQUIRE(analyzer.totalNumbers == 1);
  }
  SECTION("Decode an double (3.402823466e+39) and traverse", "[JSON][Traverse][Double]")
  {
    BufferSource source{ "3.402823466e+39" };
    bEncode.parse(source);// sizeof(doble) == sizeof(long double)
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalDouble == 1);
    REQUIRE(analyzer.totalNumbers == 1);
  }
  SECTION("Decode an long double (1.7976931348623158e+308) and traverse", "[JSON][Traverse][LongDouble]")
  {
    BufferSource source{ "1.7976931348623158e+308" };
    bEncode.parse(source);// sizeof(doble) == sizeof(long double)
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalDouble == 1);
    REQUIRE(analyzer.totalNumbers == 1);
  }
  SECTION("Decode a bool (true) and traverse", "[JSON][Traverse][Boolean]")
  {
    BufferSource source{ "true" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalBoolean == 1);
  }
  SECTION("Decode a string (\"test\") and traverse", "[JSON][Traverse][String]")
  {
    BufferSource source{ "\"test\"" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 1);
    REQUIRE(analyzer.totalStrings == 1);
  }
  SECTION("Decode a array ([1,2,3,4]) and traverse", "[JSON][Traverse][Array]")
  {
    BufferSource source{ "[1,2,3,4]" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 5);
    REQUIRE(analyzer.totalNumbers == 4);
    REQUIRE(analyzer.totalArrays == 1);
  }
  SECTION("Decode a object({\"one\" : 1, \"two\" : 2, \"three\" : 3 })) and traverse", "[JSON][Traverse][Object]")
  {
    BufferSource source{ "{\"one\" : 1, \"two\" : 2, \"three\" : 3 }" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 4);
    REQUIRE(analyzer.totalNumbers == 3);
    REQUIRE(analyzer.totalObjects == 1);
  }
  SECTION("Decode a null and traverse", "[JSON][Traverse][Null]")
  {
    BufferSource source{ "null" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNull == 1);
  }
  SECTION(
    "Decode a nested array ({\"City\":\"London\",\"Population\":[1,2,3,4,5]}) and traverse", "[JSON][Traverse][Array]")
  {
    BufferSource source{ "{\"City\":\"London\",\"Population\":[1,2,3,4,5]}" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 8);
    REQUIRE(analyzer.totalObjects == 1);
    REQUIRE(analyzer.totalArrays == 1);
    REQUIRE(analyzer.totalStrings == 1);
    REQUIRE(analyzer.totalNumbers == 5);
  }
  SECTION("Decode a nested object ([true,\"Out of time\",7.89043e+18,{\"key\":4444}]) and traverse",
    "[JSON][Traverse][Object]")
  {
    BufferSource source{ "[true,\"Out of time\",7.89043e+18,{\"key\":4444}]" };
    bEncode.parse(source);
    JSON_Analyzer analyzer;
    bEncode.traverse(analyzer);
    REQUIRE(analyzer.totalNodes == 6);
    REQUIRE(analyzer.totalObjects == 1);
    REQUIRE(analyzer.totalArrays == 1);
    REQUIRE(analyzer.totalStrings == 1);
    REQUIRE(analyzer.totalNumbers == 2);
    REQUIRE(analyzer.totalBoolean == 1);
  }
}