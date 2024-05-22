#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace JSON_Lib {
// ======
// Object
// ======
struct Object : Variant
{
  // Object entry
  struct Entry
  {
    Entry(const std::string &key, JNode &jNode) : key(JNode::make<String>(key)), jNode(std::move(jNode)) {}
    Entry(const std::string &key, JNode &&jNode) : key(JNode::make<String>(key)), jNode(std::move(jNode)) {}
    JNode &getKey() { return key; }
    [[nodiscard]] const JNode &getKey() const { return key; }
    JNode &getJNode() { return jNode; }
    [[nodiscard]] const JNode &getJNode() const { return jNode; }

  private:
    JNode key;
    JNode jNode;
  };
  // Constructors/Destructors
  Object() : Variant(Type::object) {}
  Object(const Object &other) = default;
  Object &operator=(const Object &other) = default;
  Object(Object &&other) = default;
  Object &operator=(Object &&other) = default;
  ~Object() = default;
  // Add array element
  void add(Entry &entry) { jNodeObject.emplace_back(std::move(entry)); }
  void add(Entry &&entry) { jNodeObject.emplace_back(std::move(entry)); }
  // Return true if an object contains a given key
  [[nodiscard]] bool contains(const std::string &key) const
  {
    try {
      [[maybe_unused]] auto entry = findKey(key);
    } catch ([[maybe_unused]] const JNode::Error &e) {
      return false;
    }
    return true;
  }
  // Return number of entries in an object
  [[nodiscard]] int size() const { return static_cast<int>(jNodeObject.size()); }
  // Return object entry for a given key
  JNode &operator[](const std::string &key) { return findKey(key)->getJNode(); }
  const JNode &operator[](const std::string &key) const { return findKey(key)->getJNode(); }
  // Return reference to base of object entries
  std::vector<Entry> &value() { return jNodeObject; }
  [[nodiscard]] const std::vector<Entry> &value() const { return jNodeObject; }

private:
  // Search for a given entry given a key and object list
  [[nodiscard]] std::vector<Entry>::iterator findKey(const std::string &key)
  {
    const auto keyEntry = std::ranges::find_if(jNodeObject, [&key](Entry &entry) -> bool {
      return static_cast<String &>(entry.getKey().getVariant()).value() == key;
    });
    if (keyEntry == jNodeObject.end()) { throw JNode::Error("Invalid key used to access object."); }
    return keyEntry;
  }
  [[nodiscard]] std::vector<Entry>::const_iterator findKey(const std::string &key) const
  {
    const auto keyEntry = std::ranges::find_if(jNodeObject, [&key](const Entry &entry) -> bool {
      return static_cast<const String &>(entry.getKey().getVariant()).value() == key;
    });
    if (keyEntry == jNodeObject.end()) { throw JNode::Error("Invalid key used to access object."); }
    return keyEntry;
  }
  // Object entries list
  std::vector<Entry> jNodeObject;
};
}// namespace JSON_Lib