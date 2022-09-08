#pragma once
// =======
// C++ STL
// =======
#include <set>
#include <sstream>
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Types.hpp"
#include "IAction.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==================
// JSON Tree Analysis
// ==================
class JSON_Analyzer : public IAction
{
public:
  // ========================
  // Constructors/destructors
  // ========================
  JSON_Analyzer() = default;
  JSON_Analyzer(const JSON_Analyzer &other) = delete;
  JSON_Analyzer &operator=(const JSON_Analyzer &other) = delete;
  JSON_Analyzer(IAction &&JNodeDetails) = delete;
  JSON_Analyzer &operator=(JSON_Analyzer &&other) = delete;
  virtual ~JSON_Analyzer() = default;
  // Add JNode details to analysis
  virtual void onJNode([[maybe_unused]] const JNode &jNode) override { totalNodes++; }
  // Add string details to analysis
  virtual void onString(const String &jNodeString) override
  {
    totalStrings++;
    sizeInBytes += sizeof(String);
    sizeInBytes += jNodeString.getString().size();
    maxStringSize = std::max(jNodeString.getString().size(), maxKeySize);
    uniqueStrings.insert(jNodeString.getString());
  }
  // Add number details to analysis
  virtual void onNumber([[maybe_unused]] const Number &jNodeNumber) override
  {
    totalNumbers++;
    sizeInBytes += sizeof(Number);
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
  virtual void onBoolean([[maybe_unused]] const Boolean &jNodeBoolean) override
  {
    totalBoolean++;
    sizeInBytes += sizeof(Boolean);
  }
  // Add null details to analysis
  virtual void onNull([[maybe_unused]] const Null &jNodeNull) override
  {
    totalNull++;
    sizeInBytes += sizeof(Null);
  }
  // Add array details to analysis
  virtual void onArray(const Array &jNodeArray) override
  {
    totalArrays++;
    sizeInBytes += sizeof(Array);
    maxArraySize = std::max(jNodeArray.size(), maxArraySize);
    for ([[maybe_unused]] auto &jNodeEntry : jNodeArray.getArrayEntries()) { sizeInBytes += sizeof(JNode); }
  }
  // Add object details to analysis
  virtual void onObject(const Object &jNodeObject) override
  {
    totalObjects++;
    sizeInBytes += sizeof(Object);
    maxObjectSize = std::max(jNodeObject.getObjectEntries().size(), maxObjectSize);

    for (auto &entry : jNodeObject.getObjectEntries()) {
      auto &key = JSONLib::JRef<String>(entry.getKey()).getString();
      uniqueKeys.insert(key);
      maxKeySize = std::max(key.size(), maxKeySize);
      sizeInBytes += key.size();
      sizeInBytes += sizeof(Object::Entry);
      totalKeys++;
    }
  }
  // Output analysis details
  std::string dump()
  {
    std::stringstream os;
    os << "\n--------------------JNode Sizes---------------------\n";
    os << "JNode size " << sizeof(JNode) << " in bytes.\n";
    os << "Object size " << sizeof(Object) << " in bytes.\n";
    os << "Object Entry size " << sizeof(Object::Entry) << " in bytes.\n";
    os << "Array size " << sizeof(Array) << " in bytes.\n";
    os << " size " << sizeof(Number) << " in bytes.\n";
    os << "Number::Values size " << sizeof(Number::Values) << " in bytes.\n";
    os << "Number size " << sizeof(Number) << " in bytes.\n";
    os << "String size " << sizeof(String) << " in bytes.\n";
    os << "Boolean size " << sizeof(Boolean) << " in bytes.\n";
    os << "Null size " << sizeof(Null) << " in bytes.\n";
    os << "------------------JSON Tree Stats------------------\n";
    os << "JSON Tree contains " << totalNodes << " nodes.\n";
    os << "JSON Tree size " << sizeInBytes << " in bytes.\n";
    os << "------------------JSON Object Stats------------------\n";
    os << "JSON Tree contains " << totalObjects << " objectEntries.\n";
    os << "JSON Tree max object size " << maxObjectSize << ".\n";
    os << "JSON Tree total " << totalKeys << " keys.\n";
    os << "JSON Tree contains " << uniqueKeys.size() << " unique keys.\n";
    os << "JSON Tree max key size " << maxKeySize << " in bytes.\n";
    os << "------------------JSON Array Stats------------------\n";
    os << "JSON Tree contains " << totalArrays << " arrays.\n";
    os << "JSON Tree max array size " << maxArraySize << ".\n";
    os << "------------------JSON String Stats------------------\n";
    os << "JSON Tree total " << totalStrings << " strings.\n";
    os << "JSON Tree contains " << uniqueStrings.size() << " unique strings.\n";
    os << "JSON Tree max string size " << maxStringSize << " in bytes.\n";
    os << "------------------JSON Number Stats------------------\n";
    os << "JSON Tree contains " << totalNumbers << " numbers.\n";
    os << "JSON Tree contains " << totalInteger << " integers.\n";
    os << "JSON Tree contains " << totalLong << " longs.\n";
    os << "JSON Tree contains " << totalLongLong << " long longs.\n";
    os << "JSON Tree contains " << totalFloat << " floats.\n";
    os << "JSON Tree contains " << totalDouble << " doubles.\n";
    os << "JSON Tree contains " << totalLongDouble << " long doubles.\n";
    os << "------------------JSON Boolean Stats------------------\n";
    os << "JSON Tree contains " << totalBoolean << " booleans.\n";
    os << "------------------JSON Null Stats------------------\n";
    os << "JSON Tree contains " << totalNull << " nulls.\n";
    os << "----------------------------------------------------";
    return (os.str());
  }

private:
  // JSON analysis data
  // Node
  int64_t totalNodes{};
  size_t sizeInBytes{};
  // Object
  int64_t totalObjects{};
  size_t maxObjectSize{};
  int64_t totalKeys{};
  size_t maxKeySize{};
  std::set<std::string> uniqueKeys{};
  // Array
  size_t maxArraySize{};
  int64_t totalArrays{};
  // String
  int64_t totalStrings{};
  std::set<std::string> uniqueStrings{};
  size_t maxStringSize{};
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
}// namespace JSONLib