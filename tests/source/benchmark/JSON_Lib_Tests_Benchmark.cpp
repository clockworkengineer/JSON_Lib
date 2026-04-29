#include "JSON_Lib_Tests.hpp"

namespace {

static std::string generateLargeArrayJSON(std::size_t count)
{
  std::string json;
  json.reserve(count * 6 + 2);
  json.push_back('[');
  for (std::size_t index = 0; index < count; ++index) {
    if (index != 0) { json.push_back(','); }
    json += std::to_string(index);
  }
  json.push_back(']');
  return json;
}

static std::string generateLargeObjectJSON(std::size_t count)
{
  std::string json;
  json.reserve(count * 16 + 2);
  json.push_back('{');
  for (std::size_t index = 0; index < count; ++index) {
    if (index != 0) { json.push_back(','); }
    json += "\"key";
    json += std::to_string(index);
    json += "\":";
    json += std::to_string(index);
  }
  json.push_back('}');
  return json;
}

struct NoopAction final : public IAction
{
  void onNode(Node &) override {}
  void onNode(const Node &) override {}
  void onString(Node &) override {}
  void onString(const Node &) override {}
  void onNumber(Node &) override {}
  void onNumber(const Node &) override {}
  void onBoolean(Node &) override {}
  void onBoolean(const Node &) override {}
  void onNull(Node &) override {}
  void onNull(const Node &) override {}
  void onArray(Node &) override {}
  void onArray(const Node &) override {}
  void onObject(Node &) override {}
  void onObject(const Node &) override {}
};

}// namespace

TEST_CASE("Benchmark JSON parse/stringify/traverse performance.", "[Benchmark]")
{
  const auto largeArrayJson = generateLargeArrayJSON(10000);
  const auto largeObjectJson = generateLargeObjectJSON(2000);

  BENCHMARK("Parse large numeric array")
  {
    JSON json;
    BufferSource source{ largeArrayJson };
    json.parse(source);
    return json.root().isEmpty();
  };

  BENCHMARK("Stringify large numeric array")
  {
    JSON json;
    BufferSource source{ largeArrayJson };
    json.parse(source);
    BufferDestination destination;
    json.stringify(destination);
    return destination.size();
  };

  BENCHMARK("Parse large object with many keys")
  {
    JSON json;
    BufferSource source{ largeObjectJson };
    json.parse(source);
    return json.root().isEmpty();
  };

  BENCHMARK("Object key lookup in large object")
  {
    JSON json;
    BufferSource source{ largeObjectJson };
    json.parse(source);
    const auto &root = json.root();
    std::size_t hits = 0;
    for (int lookupIndex = 0; lookupIndex < 100; ++lookupIndex) {
      const auto key = std::string("key") + std::to_string(lookupIndex * 10);
      if (!root[key].isEmpty()) { ++hits; }
    }
    return hits;
  };

  BENCHMARK("Traverse large array JSON")
  {
    JSON json;
    BufferSource source{ largeArrayJson };
    json.parse(source);
    NoopAction action;
    json.traverse(action);
    return json.root().isEmpty();
  };

  BENCHMARK("Embedded fixed buffer stringify")
  {
    EmbeddedJSON embedded;
    embedded.parse(BufferSource{ R"({"a":1,"b":2,"c":3})" });
    FixedBufferDestination<64> destination;
    embedded.stringify(destination);
    return destination.size();
  };

  BENCHMARK("Embedded parse and stringify with fixed destination")
  {
    EmbeddedJSON embedded;
    const auto payload = R"({"status":"ok","count":123})";
    embedded.parse(BufferSource{payload});
    FixedBufferDestination<128> destination;
    embedded.stringify(destination);
    return destination.size();
  };
}
