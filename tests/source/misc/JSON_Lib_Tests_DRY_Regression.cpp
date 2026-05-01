#include "JSON_Lib_Tests.hpp"

// -----------------------------------------------------------------------
// Phase 5.1 regression tests — cover every code path touched by the
// DRY refactor (Phases 1-4).  All tests must pass with the same
// baseline: 1830 assertions, 75 test cases (pre-refactor).
// -----------------------------------------------------------------------

using namespace JSON_Lib;

// =====================================================================
// Object lookup (unified lookupKey / contains / find / operator[])
// =====================================================================

TEST_CASE("Object lookup via contains, find and operator[].", "[DRY][Object][Lookup]")
{
  // Build object directly through Node API
  const JSON json;
  json.parse(BufferSource{ R"({"alpha":1,"beta":2,"gamma":3})" });
  const Node &root = json.root();

  SECTION("contains() returns true for present key.", "[DRY][Object][Lookup][contains]")
  {
    REQUIRE(NRef<Object>(root).contains("alpha"));
    REQUIRE(NRef<Object>(root).contains("beta"));
    REQUIRE(NRef<Object>(root).contains("gamma"));
  }
  SECTION("contains() returns false for absent key.", "[DRY][Object][Lookup][contains]")
  {
    REQUIRE_FALSE(NRef<Object>(root).contains("delta"));
    REQUIRE_FALSE(NRef<Object>(root).contains(""));
  }
  SECTION("find() returns non-null pointer for present key.", "[DRY][Object][Lookup][find]")
  {
    const Node *n = NRef<Object>(root).find("beta");
    REQUIRE(n != nullptr);
    REQUIRE(NRef<Number>(*n).value<int>() == 2);
  }
  SECTION("find() returns nullptr for absent key.", "[DRY][Object][Lookup][find]")
  {
    REQUIRE(NRef<Object>(root).find("missing") == nullptr);
  }
  SECTION("operator[] retrieves correct value.", "[DRY][Object][Lookup][index]")
  {
    REQUIRE(NRef<Number>(NRef<Object>(root)["gamma"]).value<int>() == 3);
  }
  SECTION("operator[] throws for absent key.", "[DRY][Object][Lookup][index][exception]")
  {
    REQUIRE_THROWS_AS(NRef<Object>(root)["nope"], Node::Error);
  }
  SECTION("Duplicate key throws on add.", "[DRY][Object][Lookup][duplicate]")
  {
    REQUIRE_THROWS_AS(json.parse(BufferSource{ R"({"x":1,"x":2})" }), Node::Error);
  }
}

TEST_CASE("Object lookup with keys containing special characters.", "[DRY][Object][Lookup][special]")
{
  const JSON json;
  json.parse(BufferSource{ R"({"key with spaces":true,"key/slash":false})" });
  const Node &root = json.root();

  SECTION("Key with spaces found.", "[DRY][Object][Lookup][special]")
  {
    REQUIRE(NRef<Object>(root).contains("key with spaces"));
    REQUIRE(NRef<Boolean>(NRef<Object>(root)["key with spaces"]).value() == true);
  }
  SECTION("Key with slash found.", "[DRY][Object][Lookup][special]")
  {
    REQUIRE(NRef<Object>(root).contains("key/slash"));
    REQUIRE(NRef<Boolean>(NRef<Object>(root)["key/slash"]).value() == false);
  }
}

// =====================================================================
// Node move semantics (jNodeEmpty removed — isEmpty() via monostate)
// =====================================================================

TEST_CASE("Node move semantics leave source empty.", "[DRY][Node][Move]")
{
  SECTION("Move constructor empties source.", "[DRY][Node][Move][Ctor]")
  {
    Node src(42);
    REQUIRE_FALSE(src.isEmpty());
    Node dst(std::move(src));
    REQUIRE(src.isEmpty());
    REQUIRE_FALSE(dst.isEmpty());
    REQUIRE(NRef<Number>(dst).value<int>() == 42);
  }
  SECTION("Move assignment empties source.", "[DRY][Node][Move][Assign]")
  {
    Node src("hello");
    REQUIRE_FALSE(src.isEmpty());
    Node dst;
    REQUIRE(dst.isEmpty());
    dst = std::move(src);
    REQUIRE(src.isEmpty());
    REQUIRE_FALSE(dst.isEmpty());
    REQUIRE(NRef<String>(dst).value() == "hello");
  }
  SECTION("Default-constructed Node is empty.", "[DRY][Node][Move][Empty]")
  {
    Node jNode;
    REQUIRE(jNode.isEmpty());
  }
  SECTION("isEmpty() false for every concrete type.", "[DRY][Node][Move][NotEmpty]")
  {
    REQUIRE_FALSE(Node(42).isEmpty());
    REQUIRE_FALSE(Node(3.14).isEmpty());
    REQUIRE_FALSE(Node(true).isEmpty());
    REQUIRE_FALSE(Node(nullptr).isEmpty());
    REQUIRE_FALSE(Node("str").isEmpty());
  }
}

// =====================================================================
// Node::visit dispatch (overloaded helper — Phase 3.3)
// =====================================================================

TEST_CASE("Node::visit dispatches to correct type.", "[DRY][Node][Visit]")
{
  SECTION("visit dispatches Number.", "[DRY][Node][Visit][Number]")
  {
    Node n(99);
    bool called = false;
    n.visit(overloaded{
      [&](const Number &num) { called = true; REQUIRE(num.value<int>() == 99); },
      [](const auto &) {}
    });
    REQUIRE(called);
  }
  SECTION("visit dispatches String.", "[DRY][Node][Visit][String]")
  {
    Node n("world");
    bool called = false;
    n.visit(overloaded{
      [&](const String &s) { called = true; REQUIRE(s.value() == "world"); },
      [](const auto &) {}
    });
    REQUIRE(called);
  }
  SECTION("visit dispatches Boolean.", "[DRY][Node][Visit][Boolean]")
  {
    Node n(false);
    bool called = false;
    n.visit(overloaded{
      [&](const Boolean &b) { called = true; REQUIRE(b.value() == false); },
      [](const auto &) {}
    });
    REQUIRE(called);
  }
  SECTION("visit dispatches Object (unwraps unique_ptr).", "[DRY][Node][Visit][Object]")
  {
    const JSON json;
    json.parse(BufferSource{ R"({"k":1})" });
    bool called = false;
    json.root().visit(overloaded{
      [&](const Object &obj) { called = true; REQUIRE(obj.size() == 1); },
      [](const auto &) {}
    });
    REQUIRE(called);
  }
  SECTION("visit dispatches Array (unwraps unique_ptr).", "[DRY][Node][Visit][Array]")
  {
    const JSON json;
    json.parse(BufferSource{ R"([1,2,3])" });
    bool called = false;
    json.root().visit(overloaded{
      [&](const Array &arr) { called = true; REQUIRE(arr.size() == 3); },
      [](const auto &) {}
    });
    REQUIRE(called);
  }
}

// =====================================================================
// Stringify helpers — compact vs pretty-print round-trips
// (addCommaNewline / addPrettyTrailer — Phase 3.2)
// =====================================================================

TEST_CASE("Compact and pretty-print stringify round-trips.", "[DRY][Stringify][RoundTrip]")
{
  const JSON json;

  SECTION("Compact object round-trip.", "[DRY][Stringify][RoundTrip][Object][Compact]")
  {
    const std::string src{ R"({"a":1,"b":true,"c":"x"})" };
    json.parse(BufferSource{ src });
    BufferDestination dst;
    json.stringify(dst);
    REQUIRE(dst.toString() == src);
  }
  SECTION("Compact array round-trip.", "[DRY][Stringify][RoundTrip][Array][Compact]")
  {
    const std::string src{ R"([1,2,3,4,5])" };
    json.parse(BufferSource{ src });
    BufferDestination dst;
    json.stringify(dst);
    REQUIRE(dst.toString() == src);
  }
  SECTION("Pretty-print object round-trip.", "[DRY][Stringify][RoundTrip][Object][Pretty]")
  {
    const std::string src{ "{\n    \"x\": 1,\n    \"y\": 2\n}" };
    json.parse(BufferSource{ src });
    BufferDestination dst;
    json.print(dst);
    REQUIRE(dst.toString() == src);
  }
  SECTION("Pretty-print array round-trip.", "[DRY][Stringify][RoundTrip][Array][Pretty]")
  {
    const std::string src{ "[\n    1,\n    2,\n    3\n]" };
    json.parse(BufferSource{ src });
    BufferDestination dst;
    json.print(dst);
    REQUIRE(dst.toString() == src);
  }
  SECTION("Empty object compact.", "[DRY][Stringify][RoundTrip][Empty]")
  {
    json.parse(BufferSource{ "{}" });
    BufferDestination dst;
    json.stringify(dst);
    REQUIRE(dst.toString() == "{}");
  }
  SECTION("Empty array compact.", "[DRY][Stringify][RoundTrip][Empty]")
  {
    json.parse(BufferSource{ "[]" });
    BufferDestination dst;
    json.stringify(dst);
    REQUIRE(dst.toString() == "[]");
  }
  SECTION("Nested object pretty-print round-trip.", "[DRY][Stringify][RoundTrip][Nested][Pretty]")
  {
    const std::string src{ "{\n    \"outer\": {\n        \"inner\": 42\n    }\n}" };
    json.parse(BufferSource{ src });
    BufferDestination dst;
    json.print(dst);
    REQUIRE(dst.toString() == src);
  }
  SECTION("Single-element array compact — no trailing comma.", "[DRY][Stringify][RoundTrip][Single]")
  {
    json.parse(BufferSource{ R"([99])" });
    BufferDestination dst;
    json.stringify(dst);
    REQUIRE(dst.toString() == "[99]");
  }
  SECTION("Single-key object compact — no trailing comma.", "[DRY][Stringify][RoundTrip][Single]")
  {
    json.parse(BufferSource{ R"({"k":0})" });
    BufferDestination dst;
    json.stringify(dst);
    REQUIRE(dst.toString() == R"({"k":0})");
  }
}
