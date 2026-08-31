#include "catch2/catch_all.hpp"
#include "JSON_Lib.hpp"
#include "JSON_IO.hpp"
#include "INodeVisitor.hpp"
#include "IAction.hpp"
#include "ITranslator.hpp"
#include "implementation/translator/Default_Translator.hpp"
#include "IEncodingHandler.hpp"
#include "implementation/io/encoding/EncodingHandlers.hpp"
#include "implementation/file/JSON_FileIO.hpp"

using namespace JSON_Lib;

TEST_CASE("Verify SOLID Refactor - Segregated INodeVisitor interfaces", "[SOLID][Visitor]")
{
  class CustomNodeCounter : public IAction
  {
  public:
    int count = 0;
    void onNode(Node &) override { count++; }
    void onNode(const Node &) override { count++; }
  };

  JSON json;
  json.parse(BufferSource{ R"({"a": 1, "b": [true, false]})" });

  CustomNodeCounter counter;
  json.traverse(counter);
  REQUIRE(counter.count > 0);
}

TEST_CASE("Verify SOLID Refactor - Custom Translator Injection via JSON::Options", "[SOLID][DIP]")
{
  class CustomTranslator : public ITranslator
  {
  public:
    std::string from(const std::string_view &escapedString) const override
    {
      return std::string(escapedString);
    }
    std::string to(const std::string_view &rawString) const override
    {
      return std::string(rawString);
    }
  };

  JSON::Options options;
  options.setTranslator(std::make_unique<CustomTranslator>());
  JSON json(std::move(options));

  json.parse(BufferSource{ R"({"key":"value"})" });
  REQUIRE(json["key"].get<String>().value() == "value");
}

#if !JSON_LIB_NO_STDIO
TEST_CASE("Verify SOLID Refactor - EncodingHandlerFactory creates correct handlers", "[SOLID][OCP]")
{
  auto utf8Handler = EncodingHandlerFactory::create(JSON::Format::utf8);
  REQUIRE(utf8Handler != nullptr);

  auto utf8BOMHandler = EncodingHandlerFactory::create(JSON::Format::utf8BOM);
  REQUIRE(utf8BOMHandler != nullptr);

  auto utf16BEHandler = EncodingHandlerFactory::create(JSON::Format::utf16BE);
  REQUIRE(utf16BEHandler != nullptr);

  auto utf16LEHandler = EncodingHandlerFactory::create(JSON::Format::utf16LE);
  REQUIRE(utf16LEHandler != nullptr);
}
#endif
