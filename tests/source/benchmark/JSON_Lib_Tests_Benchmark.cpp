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

TEST_CASE("Benchmark object/array operations and pretty-print.", "[Benchmark][DRY]")
{
  const auto largeObjectJson = generateLargeObjectJSON(2000);
  const auto largeArrayJson = generateLargeArrayJSON(10000);

  BENCHMARK("Object contains() — 1000 hits in 2000-key object")
  {
    JSON json;
    BufferSource source{ largeObjectJson };
    json.parse(source);
    const auto &obj = NRef<Object>(json.root());
    std::size_t hits = 0;
    for (int i = 0; i < 1000; ++i) {
      if (obj.contains(std::string("key") + std::to_string(i))) { ++hits; }
    }
    return hits;
  };

  BENCHMARK("Object find() — 1000 hits in 2000-key object")
  {
    JSON json;
    BufferSource source{ largeObjectJson };
    json.parse(source);
    const auto &obj = NRef<Object>(json.root());
    std::size_t hits = 0;
    for (int i = 0; i < 1000; ++i) {
      if (obj.find(std::string("key") + std::to_string(i)) != nullptr) { ++hits; }
    }
    return hits;
  };

  BENCHMARK("Object find() — 1000 misses in 2000-key object")
  {
    JSON json;
    BufferSource source{ largeObjectJson };
    json.parse(source);
    const auto &obj = NRef<Object>(json.root());
    std::size_t misses = 0;
    for (int i = 0; i < 1000; ++i) {
      if (obj.find(std::string("absent") + std::to_string(i)) == nullptr) { ++misses; }
    }
    return misses;
  };

  BENCHMARK("Array random-access via operator[] — 10000 elements")
  {
    JSON json;
    BufferSource source{ largeArrayJson };
    json.parse(source);
    const auto &arr = NRef<Array>(json.root());
    std::size_t sum = 0;
    for (std::size_t i = 0; i < arr.size(); i += 100) {
      sum += static_cast<std::size_t>(NRef<Number>(arr[i]).value<long long>());
    }
    return sum;
  };

  BENCHMARK("Pretty-print large object")
  {
    JSON json;
    BufferSource source{ largeObjectJson };
    json.parse(source);
    BufferDestination destination;
    json.print(destination);
    return destination.size();
  };

  BENCHMARK("Pretty-print large array")
  {
    JSON json;
    BufferSource source{ largeArrayJson };
    json.parse(source);
    BufferDestination destination;
    json.print(destination);
    return destination.size();
  };

  BENCHMARK("Stringify large object (compact)")
  {
    JSON json;
    BufferSource source{ largeObjectJson };
    json.parse(source);
    BufferDestination destination;
    json.stringify(destination);
    return destination.size();
  };
}
