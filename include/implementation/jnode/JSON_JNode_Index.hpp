#pragma once

namespace JSON_Lib {

// Object
inline JNode &JNode::operator[](const std::string &key)
{
  if (this->isHole()) {
    *this = JNode::make<Object>();
    JRef<Object>(*this).add(Object::Entry(key, JNode::make<Hole>()));
    return (JRef<Object>(*this).value().back().getJNode());
  }
  return (JRef<Object>(*this)[key]);
}
inline const JNode &JNode::operator[](const std::string &key) const { return (JRef<const Object>(*this)[key]); }
// Array
inline JNode &JNode::operator[](std::size_t index)
{
  try {
    if (this->isHole()) { *this = JNode::make<Array>(); }
    return (JRef<Array>(*this)[index]);
  } catch ([[maybe_unused]] const JNode::Error &error) {
    JRef<Array>(*this).resize(index);
    return (JRef<Array>(*this)[index]);
  }
}
inline const JNode &JNode::operator[](std::size_t index) const { return (JRef<Array>(*this)[index]); }

}// namespace JSON_Lib
