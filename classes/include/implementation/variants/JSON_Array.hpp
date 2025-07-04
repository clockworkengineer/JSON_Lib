#pragma once

namespace JSON_Lib {

struct Array : Variant
{
  using Entry = Node;
  using Entries = std::vector<Entry>;
  // Constructors/Destructors
  Array() : Variant(Type::array) {}
  Array(const Array &other) = default;
  Array &operator=(const Array &other) = default;
  Array(Array &&other) = default;
  Array &operator=(Array &&other) = default;
  ~Array() = default;
  // Add array element
  void add(Entry bNode) { jNodeArray.emplace_back(std::move(bNode)); }
  // Return the size of array
  [[nodiscard]] std::size_t size() const { return jNodeArray.size(); }
  // Return reference to array base
  Entries &value() { return jNodeArray; }
  [[nodiscard]] const Entries &value() const { return jNodeArray; }
  // Array indexing operators
  Node &operator[](const std::size_t index)
  {
    if (index < jNodeArray.size()) { return jNodeArray[index]; }
    throw Node::Error("Invalid index used to access array.");
  }
  const Node &operator[](const std::size_t index) const
  {
    if (index < jNodeArray.size()) { return jNodeArray[index]; }
    throw Node::Error("Invalid index used to access array.");
  }
  // Resize Array
  void resize(const std::size_t index)
  {
    jNodeArray.resize(index + 1);
    for (auto &entry : jNodeArray) {
      if (entry.isEmpty()) { entry = Node::make<Hole>(); }
    }
  }

private:
  // Array entries list
  Entries jNodeArray;
};

}// namespace JSON_Lib