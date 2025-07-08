#pragma once

#include <set>
#include <sstream>

#include "JSON.hpp"
#include "JSON_Core.hpp"

class JSON_Analyzer : public JSON_Lib::IAction
{
public:
  JSON_Analyzer() = default;
  ~JSON_Analyzer() override = default;
  // Add Node details to analysis
  void onNode([[maybe_unused]] const JSON_Lib::Node &jNode) override { totalNodes++; }
  // Add string details to analysis
  void onString(const JSON_Lib::Node &jNode) override
  {
    const auto &jNodeString = NRef<JSON_Lib::String>(jNode);
    totalStrings++;
    sizeInBytes += sizeof(JSON_Lib::String);
    sizeInBytes += jNodeString.value().size();
    maxStringSize = std::max(jNodeString.value().size(), maxKeySize);
    uniqueStrings.insert(std::string(jNodeString.value()));
  }
  // Add number details to analysis
  void onNumber(const JSON_Lib::Node &jNode) override
  {
    const auto &jNodeNumber = NRef<JSON_Lib::Number>(jNode);
    totalNumbers++;
    sizeInBytes += sizeof(JSON_Lib::Number);
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
  void onBoolean([[maybe_unused]] const JSON_Lib::Node &jNode) override
  {
    totalBoolean++;
    sizeInBytes += sizeof(JSON_Lib::Boolean);
  }
  // Add null details to analysis
  void onNull([[maybe_unused]] const JSON_Lib::Node &jNode) override
  {
    totalNull++;
    sizeInBytes += sizeof(JSON_Lib::Null);
  }
  // Add array details to analysis
  void onArray(const JSON_Lib::Node &jNode) override
  {
    const auto &jNodeArray = NRef<JSON_Lib::Array>(jNode);
    totalArrays++;
    sizeInBytes += sizeof(JSON_Lib::Array);
    maxArraySize = std::max(jNodeArray.size(), maxArraySize);
    for ([[maybe_unused]] auto &jNodeEntry : jNodeArray.value()) { sizeInBytes += sizeof(JSON_Lib::Node); }
  }
  // Add object details to analysis
  void onObject(const JSON_Lib::Node &jNode) override
  {
    const auto &jNodeObject = NRef<JSON_Lib::Object>(jNode);
    totalObjects++;
    sizeInBytes += sizeof(JSON_Lib::Object);
    maxObjectSize = std::max(jNodeObject.value().size(), maxObjectSize);
    for (auto &entry : jNodeObject.value()) {
      auto key = entry.getKey();
      uniqueKeys.insert(std::string(key));
      maxKeySize = std::max(key.size(), maxKeySize);
      sizeInBytes += key.size();
      sizeInBytes += sizeof(JSON_Lib::Object::Entry);
      totalKeys++;
    }
  }
  // Output analysis details
  std::string dump()
  {
    std::stringstream os;
    os << "\n------------------JSON Tree Stats------------------\n";
    os << "JSON Tree contains " << totalNodes << " nodes.\n";
    os << "JSON Tree size " << sizeInBytes << " in bytes.\n";
    os << "------------------JSON JSON_Lib::Object Stats------------------\n";
    os << "JSON Tree contains " << totalObjects << " objectEntries.\n";
    os << "JSON Tree max object size " << maxObjectSize << ".\n";
    os << "JSON Tree total " << totalKeys << " keys.\n";
    os << "JSON Tree contains " << uniqueKeys.size() << " unique keys.\n";
    os << "JSON Tree max key size " << maxKeySize << " in bytes.\n";
    os << "------------------JSON JSON_Lib::Array Stats------------------\n";
    os << "JSON Tree contains " << totalArrays << " arrays.\n";
    os << "JSON Tree max array size " << maxArraySize << ".\n";
    os << "------------------JSON JSON_Lib::String Stats------------------\n";
    os << "JSON Tree total " << totalStrings << " strings.\n";
    os << "JSON Tree contains " << uniqueStrings.size() << " unique strings.\n";
    os << "JSON Tree max string size " << maxStringSize << " in bytes.\n";
    os << "------------------JSON JSON_Lib::Number Stats------------------\n";
    os << "JSON Tree contains " << totalNumbers << " numbers.\n";
    os << "JSON Tree contains " << totalInteger << " integers.\n";
    os << "JSON Tree contains " << totalLong << " longs.\n";
    os << "JSON Tree contains " << totalLongLong << " long longs.\n";
    os << "JSON Tree contains " << totalFloat << " floats.\n";
    os << "JSON Tree contains " << totalDouble << " doubles.\n";
    os << "JSON Tree contains " << totalLongDouble << " long doubles.\n";
    os << "------------------JSON JSON_Lib::Boolean Stats------------------\n";
    os << "JSON Tree contains " << totalBoolean << " booleans.\n";
    os << "------------------JSON JSON_Lib::Null Stats------------------\n";
    os << "JSON Tree contains " << totalNull << " nulls.\n";
    os << "----------------------------------------------------";
    return (os.str());
  }
  static std::string dumpNodeSizes()
  {
    std::stringstream os;
    os << "\n--------------------JSON_Lib::Node Sizes---------------------\n";
    os << "JSON_Lib::Node size " << sizeof(JSON_Lib::Node) << " in bytes.\n";
    os << "JSON_Lib::Object size " << sizeof(JSON_Lib::Object) << " in bytes.\n";
    os << "JSON_Lib::Object Entry size " << sizeof(JSON_Lib::Object::Entry) << " in bytes.\n";
    os << "JSON_Lib::Array size " << sizeof(JSON_Lib::Array) << " in bytes.\n";
    os << "JSON_Lib::Number::Values size " << sizeof(JSON_Lib::Number::Values) << " in bytes.\n";
    os << "JSON_Lib::Number size " << sizeof(JSON_Lib::Number) << " in bytes.\n";
    os << "JSON_Lib::String size " << sizeof(JSON_Lib::String) << " in bytes.\n";
    os << "JSON_Lib::Boolean size " << sizeof(JSON_Lib::Boolean) << " in bytes.\n";
    os << "JSON_Lib::Null size " << sizeof(JSON_Lib::Null) << " in bytes.\n";
    return (os.str());
  }
  static std::string dumpNumericSizes()
  {
    std::stringstream os;
    os << "\n--------------------System Numeric Sizes---------------------\n";
    os << "Short size " << sizeof(short) << " in bytes.\n";
    os << "Int size " << sizeof(int) << " in bytes.\n";
    os << "Long Long size " << sizeof(long long) << " in bytes.\n";
    os << "Float size " << sizeof(float) << " in bytes.\n";
    os << "Double size " << sizeof(double) << " in bytes.\n";
    os << "Long Double size " << sizeof(long double) << " in bytes.\n";
    return (os.str());
  }

private:
  // JSON analysis data
  // Node
  int64_t totalNodes{};
  size_t sizeInBytes{};
  // JSON_Lib::Object
  int64_t totalObjects{};
  size_t maxObjectSize{};
  int64_t totalKeys{};
  size_t maxKeySize{};
  std::set<std::string> uniqueKeys{};
  // JSON_Lib::Array
  size_t maxArraySize{};
  int64_t totalArrays{};
  // JSON_Lib::String
  int64_t totalStrings{};
  std::set<std::string> uniqueStrings{};
  size_t maxStringSize{};
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