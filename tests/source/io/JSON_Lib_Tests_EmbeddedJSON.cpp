#include "JSON_Lib_Tests.hpp"

TEST_CASE("EmbeddedJSON API entrypoint build checks", "[JSON][Embedded][API]")
{
  REQUIRE(EmbeddedJSON::isEmbeddedBuild() == static_cast<bool>(JSON_LIB_EMBEDDED));
  REQUIRE(EmbeddedJSON::isExceptionFreeBuild() == static_cast<bool>(JSON_LIB_NO_EXCEPTIONS));
  REQUIRE(EmbeddedJSON::isNoStdIoBuild() == static_cast<bool>(JSON_LIB_NO_STDIO));
  REQUIRE(EmbeddedJSON::isNoDynamicMemoryBuild() == static_cast<bool>(JSON_LIB_NO_DYNAMIC_MEMORY));
  REQUIRE(EmbeddedJSON::Limits::maxParserDepth() >= 1);
  REQUIRE(EmbeddedJSON::Limits::maxStringLength() >= 1);
}

TEST_CASE("EmbeddedJSON parse and stringify buffer", "[JSON][Embedded][Buffer]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"x":true})" });
  BufferDestination destination;
  embedded.stringify(destination);
  REQUIRE(destination.toString() == R"({"x":true})");
}

TEST_CASE("EmbeddedJSON fixed buffer stringify round-trip", "[JSON][Embedded][Buffer][FixedBuffer]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"embedded":true,"value":"small"})" });
  FixedBufferDestination<64> destination;
  embedded.stringify(destination);
  REQUIRE(destination.toString() == R"({"embedded":true,"value":"small"})");
  REQUIRE(destination.size() <= 64);
}

TEST_CASE("EmbeddedJSON fixed buffer stringify overflow", "[JSON][Embedded][Buffer][FixedBuffer][Overflow]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"overflow":"this will not fit"})" });
  FixedBufferDestination<8> destination;
  REQUIRE_THROWS_AS(embedded.stringify(destination), IDestination::Error);
}

TEST_CASE("EmbeddedJSON validation of embedded build policies", "[JSON][Embedded][Policy]")
{
  REQUIRE(EmbeddedJSON::isEmbeddedBuild() == static_cast<bool>(JSON_LIB_EMBEDDED));
  REQUIRE(EmbeddedJSON::isExceptionFreeBuild() == static_cast<bool>(JSON_LIB_NO_EXCEPTIONS));
  REQUIRE(EmbeddedJSON::isNoStdIoBuild() == static_cast<bool>(JSON_LIB_NO_STDIO));
  REQUIRE(EmbeddedJSON::isNoDynamicMemoryBuild() == static_cast<bool>(JSON_LIB_NO_DYNAMIC_MEMORY));
  REQUIRE(EmbeddedJSON::Limits::maxParserDepth() >= 1);
  REQUIRE(EmbeddedJSON::Limits::maxStringLength() >= 1);
}

TEST_CASE("FixedBufferSource parse simple JSON", "[JSON][Embedded][FixedBufferSource]")
{
  constexpr char json[] = R"({"key":42})";
  EmbeddedJSON embedded;
  embedded.parse(FixedBufferSource{ json, sizeof(json) - 1 });
  BufferDestination destination;
  embedded.stringify(destination);
  REQUIRE(destination.toString() == R"({"key":42})");
}

TEST_CASE("FixedBufferSource parse and round-trip to FixedBufferDestination", "[JSON][Embedded][FixedBufferSource]")
{
  constexpr char json[] = R"({"a":1,"b":true,"c":"hello"})";
  EmbeddedJSON embedded;
  embedded.parse(FixedBufferSource{ json, sizeof(json) - 1 });
  FixedBufferDestination<64> destination;
  embedded.stringify(destination);
  REQUIRE(destination.toString() == std::string(json));
}

TEST_CASE("FixedBufferSource reset re-parses correctly", "[JSON][Embedded][FixedBufferSource]")
{
  constexpr char json[] = R"([1,2,3])";
  FixedBufferSource source{ json, sizeof(json) - 1 };
  EmbeddedJSON first;
  first.parse(source);
  BufferDestination d1;
  first.stringify(d1);
  // reset and re-parse
  source.reset();
  EmbeddedJSON second;
  second.parse(source);
  BufferDestination d2;
  second.stringify(d2);
  REQUIRE(d1.toString() == d2.toString());
}

TEST_CASE("FixedBufferSource rejects null pointer", "[JSON][Embedded][FixedBufferSource]")
{
  REQUIRE_THROWS_AS((FixedBufferSource{ nullptr, 10 }), ISource::Error);
}

TEST_CASE("FixedBufferSource rejects zero length", "[JSON][Embedded][FixedBufferSource]")
{
  constexpr char json[] = "{}";
  REQUIRE_THROWS_AS((FixedBufferSource{ json, 0 }), ISource::Error);
}

#if JSON_LIB_NO_EXCEPTIONS
TEST_CASE("FixedBufferDestination overflow sets flag when NO_EXCEPTIONS", "[JSON][Embedded][FixedBufferDestination][NoExceptions]")
{
  FixedBufferDestination<4> dest;
  dest.add("12345"); // 5 bytes > capacity 4
  REQUIRE(dest.overflowed());
  dest.clearOverflow();
  REQUIRE_FALSE(dest.overflowed());
}
#else
TEST_CASE("FixedBufferDestination overflow throws in normal build", "[JSON][Embedded][FixedBufferDestination][Overflow]")
{
  FixedBufferDestination<4> dest;
  REQUIRE_THROWS_AS(dest.add("12345"), IDestination::Error);
}
#endif

TEST_CASE("EmbeddedJSON parseNoThrow returns Ok on valid JSON", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  const auto result = embedded.parseNoThrow(BufferSource{ R"({"x":1})" });
  REQUIRE(result.ok());
}

TEST_CASE("EmbeddedJSON stringifyNoThrow returns Ok after parse", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"x":1})" });
  BufferDestination destination;
  const auto result = embedded.stringifyNoThrow(destination);
  REQUIRE(result.ok());
  REQUIRE(destination.toString() == R"({"x":1})");
}

TEST_CASE("EmbeddedJSON stringifyNoThrow returns InvalidInput on empty JSON", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  BufferDestination destination;
  const auto result = embedded.stringifyNoThrow(destination);
  REQUIRE_FALSE(result.ok());
  REQUIRE(result.status == Status::InvalidInput);
}

TEST_CASE("EmbeddedJSON printNoThrow returns Ok after parse", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"([1,2,3])" });
  BufferDestination destination;
  const auto result = embedded.printNoThrow(destination);
  REQUIRE(result.ok());
}

TEST_CASE("EmbeddedJSON parseNoThrow rvalue overload", "[JSON][Embedded][NoThrow]")
{
  EmbeddedJSON embedded;
  const auto result = embedded.parseNoThrow(BufferSource{ R"(true)" });
  REQUIRE(result.ok());
}

TEST_CASE("EmbeddedJSON stringify integer JSON produces correct output", "[JSON][Embedded][Integer]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{ R"({"a":1,"b":-99,"c":0})" });
  BufferDestination destination;
  embedded.stringify(destination);
  const auto str = destination.toString();
  REQUIRE(str.find("\"a\":1") != std::string::npos);
  REQUIRE(str.find("\"b\":-99") != std::string::npos);
  REQUIRE(str.find("\"c\":0") != std::string::npos);
}

#if JSON_LIB_NO_EXCEPTIONS
TEST_CASE("FixedBufferSource null pointer sets valid false under NO_EXCEPTIONS", "[JSON][Embedded][FixedBufferSource][NoExceptions]")
{
  FixedBufferSource src{nullptr, 0};
  REQUIRE_FALSE(src.valid());
  REQUIRE_FALSE(src.more());
  EmbeddedJSON embedded;
  const auto result = embedded.parseNoThrow(src);
  REQUIRE_FALSE(result.ok());
  REQUIRE(result.status == Status::InvalidInput);
}
#else
TEST_CASE("FixedBufferSource null pointer throws in normal build", "[JSON][Embedded][FixedBufferSource]")
{
  REQUIRE_THROWS_AS((FixedBufferSource{nullptr, 0}), ISource::Error);
}
#endif

TEST_CASE("EmbeddedJSON::Limits compile-time constants match expected defaults", "[JSON][Embedded][Limits]")
{
  // kMaxParserDepth reflects JSON_LIB_MAX_PARSER_DEPTH (0 → default 10)
  REQUIRE(EmbeddedJSON::Limits::kMaxParserDepth == EmbeddedJSON::Limits::maxParserDepth());
  // kMaxStringLength reflects JSON_LIB_MAX_STRING_LENGTH (0 → default 16384)
  REQUIRE(EmbeddedJSON::Limits::kMaxStringLength == EmbeddedJSON::Limits::maxStringLength());
  // Sanity: defaults are positive
  REQUIRE(EmbeddedJSON::Limits::kMaxParserDepth > 0);
  REQUIRE(EmbeddedJSON::Limits::kMaxStringLength > 0);
}

// Minimal IAction for traverseNoThrow tests
namespace {
struct CountingAction final : IAction
{
  void onNode(Node &) override          { ++nodes; }
  void onNode(const Node &) override    { ++nodes; }
  void onString(const Node &) override  {}
  void onNumber(const Node &) override  {}
  void onBoolean(const Node &) override {}
  void onNull(const Node &) override    {}
  void onArray(const Node &) override   {}
  void onObject(const Node &) override  {}
  int nodes{0};
};
} // namespace

TEST_CASE("EmbeddedJSON traverseNoThrow on populated tree succeeds", "[JSON][Embedded][Traverse]")
{
  EmbeddedJSON embedded;
  embedded.parse(BufferSource{R"({"a":1,"b":true})"});
  CountingAction action;
  const auto result = embedded.traverseNoThrow(action);
  REQUIRE(result.ok());
  REQUIRE(action.nodes > 0);
}

TEST_CASE("EmbeddedJSON traverseNoThrow on empty JSON returns InvalidInput", "[JSON][Embedded][Traverse]")
{
  EmbeddedJSON embedded;// no parse — empty root
  CountingAction action;
  const auto result = embedded.traverseNoThrow(action);
  REQUIRE_FALSE(result.ok());
  REQUIRE(result.status == Status::InvalidInput);
}
