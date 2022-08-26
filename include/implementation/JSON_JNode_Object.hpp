#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <unordered_map>
#include <vector>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ======
// Object
// ======
struct Object : Variant
{
  // Object entry
  struct Entry
  {
    Entry(const std::string &key, JNode &jNode) { initEntry(key, jNode); }
    Entry(const std::string &key, JNode &&jNode) { initEntry(key, jNode); }
    std::string &getKey() { return (m_keys[m_keyID]); }
    const std::string &getKey() const { return (m_keys[m_keyID]); }
    JNode &getJNode() { return (m_jNode); }
    const JNode &getJNode() const { return (m_jNode); }

  private:
    void initEntry(const std::string &key, JNode &jNode)
    {
      auto entry = std::find_if(m_keys.begin(),
        m_keys.end(),
        [&key](const std::pair<int64_t, std::string> &keyEntry) -> bool { return (keyEntry.second == key); });
      if (entry == m_keys.end()) {
        m_keyID = m_currentKeyID++;
        m_keys[m_keyID] = key;
      } else {
        m_keyID = entry->first;
      }
      m_jNode = std::move(jNode);
    }
    int64_t m_keyID;
    JNode m_jNode;
    inline static int64_t m_currentKeyID{};
    inline static std::unordered_map<int64_t, std::string> m_keys;
  };
  // Object entry list
  using EntryList = std::vector<Object::Entry>;
  // Constructors/Destructors
  Object() : Variant(JNode::Type::object) {}
  explicit Object(EntryList &objectEntries)
    : Variant(JNode::Type::object), m_jsonObjectEntries(std::move(objectEntries))
  {}
  Object(const Object &other) = delete;
  Object &operator=(const Object &other) = delete;
  Object(Object &&other) = default;
  Object &operator=(Object &&other) = default;
  ~Object() = default;

  // Search for a given entry given a key and object list
  [[nodiscard]] auto findKey(const std::string &key, EntryList &objectEntries)
  {
    auto entry = std::find_if(objectEntries.begin(), objectEntries.end(), [&key](const Entry &entry) -> bool {
      return (entry.getKey() == key);
    });
    if (entry == objectEntries.end()) { throw JNode::Error("Invalid key used to access object."); }
    return (entry);
  }
  [[nodiscard]] auto findKey(const std::string &key, const EntryList &objectEntries) const
  {
    auto entry = std::find_if(objectEntries.begin(), objectEntries.end(), [&key](const Entry &entry) -> bool {
      return (entry.getKey() == key);
    });
    if (entry == objectEntries.end()) { throw JNode::Error("Invalid key used to access object."); }
    return (entry);
  }
  // Return true if an object contains a given key
  [[nodiscard]] bool contains(const std::string &key) const
  {
    try {
      [[maybe_unused]] auto entry = findKey(key, m_jsonObjectEntries);
    } catch ([[maybe_unused]] const JNode::Error &e) {
      return (false);
    }
    return (true);
  }
  // Return number of entries in an object
  [[nodiscard]] int size() const { return (static_cast<int>(m_jsonObjectEntries.size())); }
  // Return object entry for a given key
  JNode &operator[](const std::string &key) { return (findKey(key, m_jsonObjectEntries)->getJNode()); }
  const JNode &operator[](const std::string &key) const { return (findKey(key, m_jsonObjectEntries)->getJNode()); }
  // Return reference to base of object entries
  EntryList &getObjectEntries() { return (m_jsonObjectEntries); }
  [[nodiscard]] const EntryList &getObjectEntries() const { return (m_jsonObjectEntries); }
  // Make Object JNode
  static JNode make() { return (JNode{ std::make_unique<Object>() }); }
  static JNode make(Object::EntryList &objectEntries)
  {
    return (JNode{ std::make_unique<Object>(Object{ objectEntries }) });
  }

private:
  EntryList m_jsonObjectEntries;
};
}// namespace JSONLib