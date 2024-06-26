#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace JSON_Lib {

// Object error
struct ObjectError final : std::runtime_error
{
  explicit ObjectError(const std::string &message) : std::runtime_error("Object Error: " + message) {}
};

// Object entry
struct ObjectEntry
{
  ObjectEntry(const std::string &key, JNode &jNode) : key(JNode::make<String>(key)), jNode(std::move(jNode)) {}
  ObjectEntry(const std::string &key, JNode &&jNode) : key(JNode::make<String>(key)), jNode(std::move(jNode)) {}
  JNode &getKey() { return key; }
  [[nodiscard]] const JNode &getKey() const { return key; }
  JNode &getJNode() { return jNode; }
  [[nodiscard]] const JNode &getJNode() const { return jNode; }

private:
  JNode key;
  JNode jNode;
};

struct Object : Variant
{
  using Error = ObjectError;
  using Entry = ObjectEntry;
  using Entries = std::vector<Entry>;
  // Constructors/Destructors
  Object() : Variant(Type::object) {}
  Object(const Object &other) = default;
  Object &operator=(const Object &other) = default;
  Object(Object &&other) = default;
  Object &operator=(Object &&other) = default;
  ~Object() = default;
  // Add Entry to Object
  template<typename T> void add(T &&entry) { jNodeObject.emplace_back(std::forward<T>(entry)); }
  // Return true if an object contains a given key
  [[nodiscard]] bool contains(const std::string &key) const
  {
    try {
      [[maybe_unused]] auto _ = findKey(jNodeObject, key);
    } catch ([[maybe_unused]] const Error &e) {
      return false;
    }
    return true;
  }
  // Return number of entries in an object
  [[nodiscard]] int size() const { return static_cast<int>(jNodeObject.size()); }
  // Return object entry for a given key
  JNode &operator[](const std::string &key) { return findKey(jNodeObject, key)->getJNode(); }
  const JNode &operator[](const std::string &key) const { return findKey(jNodeObject, key)->getJNode(); }
  // Return reference to base of object entries
  Entries &value() { return jNodeObject; }
  [[nodiscard]] const Entries &value() const { return jNodeObject; }

private:
  // Search for a given entry given a key and object list
  [[nodiscard]] static Entries::iterator findKey(Entries &object, const std::string &key);
  [[nodiscard]] static Entries::const_iterator findKey(const Entries &object, const std::string &key);

  // Object entries list
  Entries jNodeObject;
};

inline Object::Entries::iterator Object::findKey(Entries &object, const std::string &key)
{
  auto it = std::ranges::find_if(
    object, [&key](Entry &entry) -> bool { return static_cast<String &>(entry.getKey().getVariant()).value() == key; });
  if (it == object.end()) { throw Object::Error("Invalid key used to access object."); }
  return it;
}
inline Object::Entries::const_iterator Object::findKey(const Entries &object, const std::string &key)
{
  auto it = std::ranges::find_if(object, [&key](const Entry &entry) -> bool {
    return static_cast<const String &>(entry.getKey().getVariant()).value() == key;
  });
  if (it == object.end()) { throw Object::Error("Invalid key used to access object."); }
  return it;
}
}// namespace JSON_Lib