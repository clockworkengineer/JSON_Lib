#pragma once

#include "JSON_StoragePolicy.hpp"
#include <algorithm>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace JSON_Lib {

// Object entry
struct ObjectEntry
{
  ObjectEntry(const std::string_view &key, Node &jNode) : key(key), jNode(std::move(jNode)) {}
  ObjectEntry(const std::string_view &key, Node &&jNode) : key(key), jNode(std::move(jNode)) {}
  ObjectEntry(const char *key, Node &&jNode) : key(key), jNode(std::move(jNode)) {}
  ObjectEntry(std::string &&key, Node &&jNode) : key(std::move(key)), jNode(std::move(jNode)) {}
  [[nodiscard]] std::string_view getKey() const { return key; }
  [[nodiscard]] Node &getNode() { return jNode; }
  [[nodiscard]] const Node &getNode() const { return jNode; }

private:
  std::string key;
  Node jNode;
};

struct Object
{
  using Entry = ObjectEntry;
  using Entries = std::vector<Entry>;

  using Index = detail::ObjectIndex;
  // Constructors/Destructors
  Object() = default;
  Object(const Object &other) = default;
  Object &operator=(const Object &other) = default;
  Object(Object &&other) = default;
  Object &operator=(Object &&other) = default;
  ~Object() = default;
  // Add Entry to Object
  template<typename T> void add(T &&entry)
  {
    const auto key = entry.getKey();
#if JSON_LIB_EMBEDDED
    if (contains(key)) { throw Node::Error("Duplicate key used to add object entry."); }
    jNodeObject.emplace_back(std::forward<T>(entry));
    const auto insertedIndex = jNodeObject.size() - 1;
    const auto insertPosition = std::lower_bound(
      jNodeIndex.begin(), jNodeIndex.end(), key,
      [this](const std::size_t lhs, const std::string_view rhs) {
        return jNodeObject[lhs].getKey() < rhs;
      }
    );
    jNodeIndex.insert(insertPosition, insertedIndex);
#else
    if (jNodeIndex.contains(key)) { throw Node::Error("Duplicate key used to add object entry."); }
    jNodeObject.emplace_back(std::forward<T>(entry));
    jNodeIndex.emplace(jNodeObject.back().getKey(), jNodeObject.size() - 1);
#endif
  }
  // Return true if an object contains a given key
  [[nodiscard]] bool contains(const std::string_view &key) const
  {
#if JSON_LIB_EMBEDDED
    return findIndex(key) != jNodeIndex.end();
#else
    return jNodeIndex.find(key) != jNodeIndex.end();
#endif
  }
  // Return number of entries in an object
  [[nodiscard]] int size() const { return static_cast<int>(jNodeObject.size()); }
  // Reserve storage for object entries and index buckets
  void reserve(const std::size_t capacity)
  {
    jNodeObject.reserve(capacity);
    jNodeIndex.reserve(capacity);
  }
  // Return object entry for a given key
  Node &operator[](const std::string_view &key) { return jNodeObject[getIndex(key)].getNode(); }
  const Node &operator[](const std::string_view &key) const { return jNodeObject[getIndex(key)].getNode(); }
  // Find object entry for a given key without throwing
  [[nodiscard]] Node *find(const std::string_view &key)
  {
#if JSON_LIB_EMBEDDED
    const auto it = findIndex(key);
    return it != jNodeIndex.end() ? &jNodeObject[*it].getNode() : nullptr;
#else
    const auto it = jNodeIndex.find(key);
    return it != jNodeIndex.end() ? &jNodeObject[it->second].getNode() : nullptr;
#endif
  }
  [[nodiscard]] const Node *find(const std::string_view &key) const
  {
#if JSON_LIB_EMBEDDED
    const auto it = findIndex(key);
    return it != jNodeIndex.end() ? &jNodeObject[*it].getNode() : nullptr;
#else
    const auto it = jNodeIndex.find(key);
    return it != jNodeIndex.end() ? &jNodeObject[it->second].getNode() : nullptr;
#endif
  }
  // Return reference to base of object entries
  Entries &value() { return jNodeObject; }
  [[nodiscard]] const Entries &value() const { return jNodeObject; }

private:
#if JSON_LIB_EMBEDDED
  [[nodiscard]] auto findIndex(const std::string_view &key) const
  {
    return std::lower_bound(
      jNodeIndex.begin(), jNodeIndex.end(), key,
      [this](const std::size_t lhs, const std::string_view rhs) {
        return jNodeObject[lhs].getKey() < rhs;
      }
    );
  }
#endif

  [[nodiscard]] std::size_t getIndex(const std::string_view &key) const
  {
#if JSON_LIB_EMBEDDED
    const auto it = findIndex(key);
    if (it == jNodeIndex.end() || jNodeObject[*it].getKey() != key) {
      throw Node::Error("Invalid key used to access object.");
    }
    return *it;
#else
    const auto it = jNodeIndex.find(key);
    if (it == jNodeIndex.end()) { throw Node::Error("Invalid key used to access object."); }
    return it->second;
#endif
  }

  // Object entries list
  Entries jNodeObject;
  Index jNodeIndex;
};
}// namespace JSON_Lib