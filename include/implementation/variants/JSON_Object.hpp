#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <unordered_map>
#include <vector>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ======
// Object
// ======
struct Object : Variant
{
  // Object entry
  struct Entry
  {
    Entry(const std::string &key, JNode &jNode) : m_key(JNode::make<String>(key)), m_jNode(std::move(jNode)) {}
    Entry(const std::string &key, JNode &&jNode) : m_key(JNode::make<String>(key)), m_jNode(std::move(jNode)) {}
    JNode &getKey() { return (m_key); }
    const JNode &getKey() const { return (m_key); }
    JNode &getJNode() { return (m_jNode); }
    const JNode &getJNode() const { return (m_jNode); }

  private:
    JNode m_key;
    JNode m_jNode;
  };
  // Constructors/Destructors
  Object() : Variant(Variant::Type::object) {}
  explicit Object(std::vector<Entry> &objectEntries) : Variant(Variant::Type::object), m_object(std::move(objectEntries)) {}
  Object(const Object &other) = default;
  Object &operator=(const Object &other) = default;
  Object(Object &&other) = default;
  Object &operator=(Object &&other) = default;
  ~Object() = default;
  // Return true if an object contains a given key
  [[nodiscard]] bool contains(const std::string &key) const
  {
    try {
      [[maybe_unused]] auto entry = findKey(key);
    } catch ([[maybe_unused]] const JNode::Error &e) {
      return (false);
    }
    return (true);
  }
  // Return number of entries in an object
  [[nodiscard]] int size() const { return (static_cast<int>(m_object.size())); }
  // Return object entry for a given key
  JNode &operator[](const std::string &key) { return (findKey(key)->getJNode()); }
  const JNode &operator[](const std::string &key) const { return (findKey(key)->getJNode()); }
  // Return reference to base of object entries
  std::vector<Entry> &getObjectEntries() { return (m_object); }
  [[nodiscard]] const std::vector<Entry> &getObjectEntries() const { return (m_object); }

private:
  // Search for a given entry given a key and object list
  [[nodiscard]] std::vector<Entry>::iterator findKey(const std::string &key)
  {
    auto entry = std::find_if(m_object.begin(), m_object.end(), [&key](Entry &entry) -> bool {
      return (static_cast<String &>(entry.getKey().getVariant()).getString() == key);
    });
    if (entry == m_object.end()) { throw JNode::Error("Invalid key used to access object."); }
    return (entry);
  }
  [[nodiscard]] std::vector<Entry>::const_iterator findKey(const std::string &key) const
  {
    auto entry = std::find_if(m_object.begin(), m_object.end(), [&key](const Entry &entry) -> bool {
      return (static_cast<const String &>(entry.getKey().getVariant()).getString() == key);
    });
    if (entry == m_object.end()) { throw JNode::Error("Invalid key used to access object."); }
    return (entry);
  }
  // Object entries list
  std::vector<Entry> m_object;
};
}// namespace JSON_Lib