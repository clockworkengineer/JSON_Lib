#pragma once

namespace JSON_Lib {

// Object
inline Node &Node::operator[](const std::string_view &key)
{
  if (isA<Hole>(*this)) {
    *this = make<Object>();
    JRef<Object>(*this).add(Object::Entry(key, make<Hole>()));
    return JRef<Object>(*this).value().back().getNode();
  }
  return JRef<Object>(*this)[key];
}
inline const Node &Node::operator[](const std::string_view &key) const { return JRef<const Object>(*this)[key]; }
// Array
inline Node &Node::operator[](const std::size_t index)
{
  try {
    if (isA<Hole>(*this)) { *this = make<Array>(); }
    return JRef<Array>(*this)[index];
  } catch ([[maybe_unused]] const Error &error) {
    JRef<Array>(*this).resize(index);
    return JRef<Array>(*this)[index];
  }
}
inline const Node &Node::operator[](const std::size_t index) const { return JRef<Array>(*this)[index]; }

}// namespace JSON_Lib
