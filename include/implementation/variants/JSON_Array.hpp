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
  Array() : Variant(Variant::Type::array) {}
  Array(const Array &other) = default;
  Array &operator=(const Array &other) = default;
  Array(Array &&other) = default;
  Array &operator=(Array &&other) = default;
  ~Array() = default;
  // Add array element
  void add(JNode &jNode) { array.emplace_back(std::move(jNode)); }
  void add(JNode &&jNode) { array.emplace_back(std::move(jNode)); }
  // Return the size of array
  [[nodiscard]] std::size_t size() const { return (array.size()); }
  // Return reference to array base
  std::vector<JNode> &getArrayEntries() { return (array); }
  [[nodiscard]] const std::vector<JNode> &getArrayEntries() const { return (array); }
  // Array indexing operators
  JNode &operator[](std::size_t index)
  {
    if ((index >= 0) && (index < (static_cast<std::size_t>(array.size())))) { return (array[index]); }
    throw JNode::Error("Invalid index used to access array.");
  }
  const JNode &operator[](std::size_t index) const
  {
    if ((index >= 0) && (index < (static_cast<std::size_t>(array.size())))) { return (array[index]); }
    throw JNode::Error("Invalid index used to access array.");
  }
  // Resize Array
  void resize(std::size_t index)
  {
    array.resize(index + 1);
    for (auto &entry : array) {
      if (entry.isEmpty()) { entry = JNode::make<Hole>(); }
    }
  }

private:
  // Array entries list
  std::vector<JNode> array;
};
}// namespace JSON_Lib