#pragma once

namespace JSON_Lib {

// Object
inline Node &Node::operator[](const std::string_view &key)
{
  if (isA<Hole>(*this)) {
    *this = make<Object>();
    NRef<Object>(*this).add(Object::Entry(key, make<Hole>()));
    return NRef<Object>(*this).value().back().getNode();
  }
  return NRef<Object>(*this)[key];
}
inline const Node &Node::operator[](const std::string_view &key) const { return NRef<const Object>(*this)[key]; }
// Array
inline Node &Node::operator[](const std::size_t index)
{
  try {
    if (isA<Hole>(*this)) { *this = make<Array>(); }
    return NRef<Array>(*this)[index];
  } catch ([[maybe_unused]] const Error &error) {
    NRef<Array>(*this).resize(index);
    return NRef<Array>(*this)[index];
  }
}
inline const Node &Node::operator[](const std::size_t index) const { return NRef<Array>(*this)[index]; }

}// namespace JSON_Lib
