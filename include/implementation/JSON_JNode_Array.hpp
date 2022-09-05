#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <vector>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// =====
// Array
// =====
struct Array : Variant
{
  // Array entry list
  using ArrayList = std::vector<JNode>;
  // Constructors/Destructors
  Array() : Variant(JNode::Type::array) {}
  explicit Array(ArrayList &array) : Variant(JNode::Type::array), m_array(std::move(array)) {}
  Array(const Array &other) = delete;
  Array &operator=(const Array &other) = delete;
  Array(Array &&other) = default;
  Array &operator=(Array &&other) = default;
  ~Array() = default;
  // Return the size of array
  [[nodiscard]] std::size_t size() const { return (m_array.size()); }
  // Return reference to array base
  ArrayList &getArrayEntries() { return (m_array); }
  [[nodiscard]] const ArrayList &getArrayEntries() const { return (m_array); }
  // Array indexing operators
  JNode &operator[](std::size_t index)
  {
    if ((index >= 0) && (index < (static_cast<int>(m_array.size())))) { return (m_array[index]); }
    throw JNode::Error("Invalid index used to access array.");
  }
  const JNode &operator[](std::size_t index) const
  {
    if ((index >= 0) && (index < (static_cast<int>(m_array.size())))) { return (m_array[index]); }
    throw JNode::Error("Invalid index used to access array.");
  }
  // Resize Array
  void resize(std::size_t index)
  {
    m_array.resize(index + 1);
    for (auto &entry : m_array) {
      if (entry.isEmpty()) { entry = Hole::make(); }
    }
  }
  // Make Array JNode
  static JNode make() { return (JNode{ std::make_unique<Array>() }); }
  static JNode make(Array::ArrayList &array) { return (JNode{ std::make_unique<Array>(Array{ array }) }); }

private:
  // Array entries list
  ArrayList m_array;
};
}// namespace JSONLib