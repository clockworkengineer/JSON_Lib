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
inline std::unique_ptr<JNode> makeObject(Object::EntryList &objects) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<Object>(Object{objects})}));
}
inline std::unique_ptr<JNode> makeArray(Array::ArrayList &array) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<Array>(Array{array})}));
}
inline std::unique_ptr<JNode> makeNumber(const Numeric &number) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<Number>(Number{number})}));
}
inline std::unique_ptr<JNode> makeString(const std::string &string) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<String>(String{string})}));
}
inline std::unique_ptr<JNode> makeBoolean(bool boolean) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<Boolean>(Boolean{boolean})}));
}
inline std::unique_ptr<JNode> makeNull() {
  return (
      std::make_unique<JNode>(JNode{std::make_unique<Null>(Null())}));
}
inline std::unique_ptr<JNode> makeHole() {
  return (
      std::make_unique<JNode>(JNode{std::make_unique<Hole>(Hole())}));
}
}