#pragma once
// =======
// C++ STL
// =======
#include <set>
#include <sstream>

// ====
// JSON
// ====
#include "IAction.hpp"
#include "JSON.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Types.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==================
// JSON Tree Analysis
// ==================
class JSON_Analyzer : public IAction {
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
  // Add string details to analysis
  virtual void onString(const String &jNodeString) override {
    totalNodes++;
    totalStrings++;
    sizeInBytes += sizeof(String);
    sizeInBytes += jNodeString.string().size();
    uniqueStrings.insert(jNodeString.string());
  }
  // Add number details to analysis
  virtual void onNumber([[maybe_unused]] const Number &jNodeNumber) override {
    totalNodes++;
    totalNumbers++;
    sizeInBytes += sizeof(Number);
  }
  virtual void
  onBoolean([[maybe_unused]] const Boolean &jNodeBoolean) override {
    totalNodes++;
    totalBoolean++;
    sizeInBytes += sizeof(Boolean);
  }
  // Add null details to analysis
  virtual void onNull([[maybe_unused]] const Null &jNodeNull) override {
    totalNodes++;
    totalNull++;
    sizeInBytes += sizeof(Null);
  }
  // Add array details to analysis
  virtual void onArray(const Array &jNodeArray) override {
    totalNodes++;
    totalArrays++;
    sizeInBytes += sizeof(Array);
    maxArraySize = std::max(jNodeArray.size(), maxArraySize);
    for ([[maybe_unused]] auto &bNodeEntry : jNodeArray.array()) {
      sizeInBytes += sizeof(JNode);
    }
  }
  // Add object details to analysis
  virtual void onObject(const Object &jNodeObject) override {
    totalNodes++;
    totalObjects++;
    sizeInBytes += sizeof(Object);
    maxObjectSize = std::max(jNodeObject.objectEntries().size(), maxObjectSize);
    for (auto &entry : jNodeObject.objectEntries()) {
      uniqueKeys.insert(entry.getKey());
      sizeInBytes += entry.getKey().size();
      sizeInBytes += sizeof(Object::Entry);
      totalKeys++;
    }
  }
  // Output analysis details
  std::string dump() {
    std::stringstream os;
    os << "\n--------------------JNode Sizes---------------------\n";
    os << "JNode size " << sizeof(JNode) << " in bytes.\n";
    os << "Object size " << sizeof(Object) << " in bytes.\n";
    os << "Object Entry size " << sizeof(Object::Entry) << " in bytes.\n";
    os << "Array size " << sizeof(Array) << " in bytes.\n";
    os << "Numeric size " << sizeof(Numeric) << " in bytes.\n";
    os << "Number size " << sizeof(Number) << " in bytes.\n";
    os << "String size " << sizeof(String) << " in bytes.\n";
    os << "Boolean size " << sizeof(Boolean) << " in bytes.\n";
    os << "Null size " << sizeof(Null) << " in bytes.\n";
    os << "------------------JSON Tree Stats------------------\n";
    os << "JSON Tree contains " << totalNodes << " nodes.\n";
    os << "JSON Tree size " << sizeInBytes << " in bytes.\n";
    os << "JSON Tree contains " << totalObjects << " objectEntries.\n";
    os << "JSON Tree max object size " << maxObjectSize << ".\n";
    os << "JSON Tree total " << totalKeys << " keys.\n";
    os << "JSON Tree contains " << uniqueKeys.size() << " unique keys.\n";
    os << "JSON Tree contains " << totalArrays << " arrays.\n";
    os << "JSON Tree max array size " << maxArraySize << ".\n";
    os << "JSON Tree total " << totalStrings << " strings.\n";
    os << "JSON Tree contains " << uniqueStrings.size() << " unique strings.\n";
    os << "JSON Tree contains " << totalNumbers << " numbers.\n";
    os << "JSON Tree contains " << totalBoolean << " booleans.\n";
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
  std::set<std::string> uniqueKeys{};
  // Array
  size_t maxArraySize{};
  int64_t totalArrays{};
  // String
  int64_t totalStrings{};
  std::set<std::string> uniqueStrings{};
  // Number
  int64_t totalNumbers{};
  // Boolean
  int64_t totalBoolean{};
  // Null
  int64_t totalNull{};
};
} // namespace JSONLib