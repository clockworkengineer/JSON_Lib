#pragma once
// =======
// C++ STL
// =======
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// =====
// JNode
// =====
struct JNode
{
  // JNode Error
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("JNode Error: " + message) {}
  };
  // Constructors/Destructors
  JNode() = default;
  template<typename T> explicit JNode(T value);
  JNode(const JSON::ArrayInitializerList &array);
  JNode(const JSON::ObjectInitializerList &object);
  JNode(const JNode &other) = delete;
  JNode &operator=(const JNode &other) = delete;
  JNode(JNode &&other) = default;
  JNode &operator=(JNode &&other) = default;
  ~JNode() = default;
  // Assignment operators
  template<typename T> JNode &operator=(T value) { return (*this = JNode(value)); }
  // Interrogate variant
  bool isEmpty() const { return (m_variant == nullptr); }
  bool isObject() const { return (m_variant->getType() == Variant::Type::object); }
  bool isArray() const { return (m_variant->getType() == Variant::Type::array); }
  bool isNumber() const { return (m_variant->getType() == Variant::Type::number); }
  bool isString() const { return (m_variant->getType() == Variant::Type::string); }
  bool isBoolean() const { return (m_variant->getType() == Variant::Type::boolean); }
  bool isNull() const { return (m_variant->getType() == Variant::Type::null); }
  bool isHole() const { return (m_variant->getType() == Variant::Type::hole); }
  // Indexing operators
  JNode &operator[](const std::string &key);
  const JNode &operator[](const std::string &key) const;
  JNode &operator[](std::size_t index);
  const JNode &operator[](std::size_t index) const;
  // Get reference to JNode variant
  Variant &getVariant() { return (*m_variant); }
  const Variant &getVariant() const { return (*m_variant); }

private:
  static JNode internalTypeToJNode(const JSON::InternalType &type);
  std::unique_ptr<Variant> m_variant;
};
/// <summary>
/// Create JNode from passed in type.
/// </summary>
/// <param name="type">Variant encapsulating type and value to be turned into JNode.</param>
/// <returns>JNode for passed in type.</returns>
inline JNode JNode::internalTypeToJNode(const JSON::InternalType &type)
{
  if (auto pValue = std::get_if<int>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<long>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<long long>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<float>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<double>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<long double>(&type)) { return (JNode(*pValue)); }
  if (auto pValue = std::get_if<std::string>(&type)) { return (JNode((*pValue))); }
  if (auto pValue = std::get_if<bool>(&type)) { return (JNode((*pValue))); }
  if (auto pValue = std::get_if<std::nullptr_t>(&type)) { return (JNode(nullptr)); }
  if (auto pValue = std::get_if<JNode>(&type)) { return (std::move(*const_cast<JNode *>(pValue))); }
  throw Error("JNode of non-existant type could not be created.");
}
}// namespace JSON_Lib