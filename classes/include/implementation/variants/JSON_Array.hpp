#pragma once

#include <string>
#include <vector>

namespace JSON_Lib {
// =====
// Array
// =====
struct Array : Variant
{
  // Constructors/Destructors
  Array() : Variant(Type::array) {}
  Array(const Array &other) = default;
  Array &operator=(const Array &other) = default;
  Array(Array &&other) = default;
  Array &operator=(Array &&other) = default;
  ~Array() = default;
  // Add array element
  void add(JNode &jNode) { jNodeArray.emplace_back(std::move(jNode)); }
  void add(JNode &&jNode) { jNodeArray.emplace_back(std::move(jNode)); }
  // Return the size of array
  [[nodiscard]] std::size_t size() const { return jNodeArray.size(); }
  // Return reference to array base
  std::vector<JNode> &value() { return jNodeArray; }
  [[nodiscard]] const std::vector<JNode> &value() const { return jNodeArray; }
  // Array indexing operators
  JNode &operator[](const std::size_t index)
  {
    if (index < jNodeArray.size()) { return jNodeArray[index]; }
    throw JNode::Error("Invalid index used to access array.");
  }
  const JNode &operator[](const std::size_t index) const
  {
    if (index < jNodeArray.size()) { return jNodeArray[index]; }
    throw JNode::Error("Invalid index used to access array.");
  }
  // Resize Array
  void resize(const std::size_t index)
  {
    jNodeArray.resize(index + 1);
    for (auto &entry : jNodeArray) {
      if (entry.isEmpty()) { entry = JNode::make<Hole>(); }
    }
  }

private:
  // Array entries list
  std::vector<JNode> jNodeArray;
};
}// namespace JSON_Lib