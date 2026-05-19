#include "JSON_Lib_Tests.hpp"

namespace {

struct CustomParser final : public IParser
{
  Node parse(ISource &source) override
  {
    while (source.more()) { source.next(); }
    return Node(JSON::ObjectInitializer{{"custom", true}, {"sourceInjected", "yes"}});
  }
};

struct CustomStringify final : public IStringify
{
  void stringify(const Node &jNode, IDestination &destination, unsigned long) const override
  {
    destination.add("[CUSTOM]");
    destination.add(jNode.isEmpty() ? "empty" : "nonempty");
  }
  long getIndent() const JSON_LIB_NOEXCEPT override { return 0; }
};

} // namespace

TEST_CASE("Custom backend injection through JSON constructor", "[JSON][Custom][Injection]")
{
  SECTION("Custom stringify backend is used when provided")
  {
    JSON json(std::make_unique<CustomStringify>(), nullptr);
    json.parse(BufferSource{R"({"a":1})"});

    BufferDestination destination;
    json.stringify(destination);

    REQUIRE(destination.toString() == "[CUSTOM]nonempty");
  }

  SECTION("Custom parser backend is used when provided and default stringify falls back")
  {
    JSON json(nullptr, std::make_unique<CustomParser>());
    json.parse(BufferSource{R"({"a":1})"});

    REQUIRE(json.contains("custom"));
    REQUIRE(NRef<Boolean>(json["custom"]).value() == true);

    BufferDestination destination;
    json.stringify(destination);
    REQUIRE(destination.toString() == R"({"custom":true,"sourceInjected":"yes"})");
  }

  SECTION("JSON constructor fallback uses default backends when nullptrs are provided")
  {
    JSON json(nullptr, nullptr);
    json.parse(BufferSource{R"({"a":1})"});

    BufferDestination destination;
    json.stringify(destination);
    REQUIRE(destination.toString() == R"({"a":1})");
  }

  SECTION("JSON::Options can configure backend injection")
  {
    JSON::Options options;
    options.setStringify(std::make_unique<CustomStringify>());
    JSON json(std::move(options));
    json.parse(BufferSource{R"({"a":1})"});

    BufferDestination destination;
    json.stringify(destination);
    REQUIRE(destination.toString() == "[CUSTOM]nonempty");
  }
}
