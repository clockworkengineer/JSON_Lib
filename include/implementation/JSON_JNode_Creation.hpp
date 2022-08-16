#pragma once
// =======
// C++ STL
// =======
#include <memory>
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==============
// JNode Creation
// ==============
inline JNode makeObject() {
  return (JNode{std::make_unique<Object>()});
}
inline JNode makeObject(Object::EntryList &objectEntries) {
  return (JNode{std::make_unique<Object>(Object{objectEntries})});
}
inline JNode makeArray() {
  return (JNode{std::make_unique<Array>()});
}
inline JNode makeArray(Array::ArrayList &array) {
  return (JNode{std::make_unique<Array>(Array{array})});
}
inline JNode makeNumber(const Numeric &number) {
  return (JNode{std::make_unique<Number>(Number{number})});
}
inline JNode makeString(const std::string &string) {
  return (JNode{std::make_unique<String>(String{string})});
}
inline JNode makeBoolean(bool boolean) {
  return (JNode{std::make_unique<Boolean>(Boolean{boolean})});
}
inline JNode makeNull() { return (JNode{std::make_unique<Null>(Null())}); }
inline JNode makeHole() { return (JNode{std::make_unique<Hole>(Hole())}); }
} // namespace JSONLib