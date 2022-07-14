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
inline std::unique_ptr<JNode> makeObject(JNodeObject::ObjectList &objects) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<JNodeObject>(JNodeObject{objects})}));
}
inline std::unique_ptr<JNode> makeArray(JNodeArray::ArrayList &array) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<JNodeArray>(JNodeArray{array})}));
}
inline std::unique_ptr<JNode> makeNumber(const JNodeNumeric &number) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<JNodeNumber>(JNodeNumber{number})}));
}
inline std::unique_ptr<JNode> makeString(const std::string &string) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<JNodeString>(JNodeString{string})}));
}
inline std::unique_ptr<JNode> makeBoolean(bool boolean) {
  return (std::make_unique<JNode>(
      JNode{std::make_unique<JNodeBoolean>(JNodeBoolean{boolean})}));
}
inline std::unique_ptr<JNode> makeNull() {
  return (
      std::make_unique<JNode>(JNode{std::make_unique<JNodeNull>(JNodeNull())}));
}
inline std::unique_ptr<JNode> makeHole() {
  return (
      std::make_unique<JNode>(JNode{std::make_unique<JNodeHole>(JNodeHole())}));
}
}