#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <vector>
// =================
// LIBRARY NAMESPACE
// =================
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
  void add(JNode &jNode) { m_array.emplace_back(std::move(jNode)); }
  void add(JNode &&jNode) { m_array.emplace_back(std::move(jNode)); }
  // Return the size of array
  [[nodiscard]] std::size_t size() const { return (m_array.size()); }
  // Return reference to array base
  std::vector<JNode> &getArrayEntries() { return (m_array); }
  [[nodiscard]] const std::vector<JNode> &getArrayEntries() const { return (m_array); }
  // Array indexing operators
  JNode &operator[](std::size_t index)
  {
    if ((index >= 0) && (index < (static_cast<std::size_t>(m_array.size())))) { return (m_array[index]); }
    throw JNode::Error("Invalid index used to access array.");
  }
  const JNode &operator[](std::size_t index) const
  {
    if ((index >= 0) && (index < (static_cast<std::size_t>(m_array.size())))) { return (m_array[index]); }
    throw JNode::Error("Invalid index used to access array.");
  }
  // Resize Array
  void resize(std::size_t index)
  {
    m_array.resize(index + 1);
    for (auto &entry : m_array) {
      if (entry.isEmpty()) { entry = JNode::make<Hole>(); }
    }
  }

private:
  // Array entries list
  std::vector<JNode> m_array;
};
}// namespace JSON_Lib