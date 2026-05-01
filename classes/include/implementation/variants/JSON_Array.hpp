#pragma once

#include <vector>

namespace JSON_Lib {

struct Array
{
  using Entry = Node;
  using Entries = std::vector<Entry>;
  // Constructors/Destructors
  Array() = default;
  Array(const Array &other) = default;
  Array &operator=(const Array &other) = default;
  Array(Array &&other) = default;
  Array &operator=(Array &&other) = default;
  ~Array() = default;
  // Add array element
  void add(Entry bNode) { jNodeArray.emplace_back(std::move(bNode)); }
  // Reserve storage for array elements
  void reserve(const std::size_t capacity) { jNodeArray.reserve(capacity); }
  // Return the size of array
  [[nodiscard]] std::size_t size() const { return jNodeArray.size(); }
  // Return the current capacity of array storage
  [[nodiscard]] std::size_t capacity() const { return jNodeArray.capacity(); }
  // Return reference to array base
  Entries &value() { return jNodeArray; }
  [[nodiscard]] const Entries &value() const { return jNodeArray; }
  // Array indexing operators
  Node &operator[](const std::size_t index) { return jNodeArray[checkBounds(index)]; }
  const Node &operator[](const std::size_t index) const { return jNodeArray[checkBounds(index)]; }
  // Resize Array — newly default-constructed Nodes are always empty, so fill all new slots
  void resize(const std::size_t index)
  {
    const auto oldSize = jNodeArray.size();
    jNodeArray.resize(index + 1);
    for (auto i = oldSize; i < jNodeArray.size(); ++i) { jNodeArray[i] = Node::make<Hole>(); }
  }

private:
  [[nodiscard]] std::size_t checkBounds(const std::size_t index) const
  {
    if (index < jNodeArray.size()) { return index; }
    throw Node::Error("Invalid index used to access array.");
  }

  // Array entries list
  Entries jNodeArray;
};

}// namespace JSON_Lib