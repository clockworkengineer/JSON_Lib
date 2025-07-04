#pragma once

namespace JSON_Lib {

// Construct Node from raw values
template<typename T> Node::Node(T value)
{
  if constexpr (std::is_same_v<T, bool>) {
    *this = Node::make<Boolean>(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    *this = Node::make<Number>(value);
  } else if constexpr (std::is_same_v<T, std::nullptr_t>) {
    *this = make<Null>();
  } else if constexpr (std::is_same_v<T, const char *> || std::is_same_v<T, std::string>) {
    *this = Node::make<String>(value);
  } else if constexpr (std::is_convertible_v<T, std::unique_ptr<Variant>>) {
    jNodeVariant = std::move(value);
  }
}
// Convert an initializer list type to JMode
static Node typeToNode(const JSON::InitializerListTypes &type)
{
  if (const auto pValue = std::get_if<int>(&type)) { return Node(*pValue); }
  if (const auto pValue = std::get_if<long>(&type)) { return Node(*pValue); }
  if (const auto pValue = std::get_if<long long>(&type)) { return Node(*pValue); }
  if (const auto pValue = std::get_if<float>(&type)) { return Node(*pValue); }
  if (const auto pValue = std::get_if<double>(&type)) { return Node(*pValue); }
  if (const auto pValue = std::get_if<long double>(&type)) { return Node(*pValue); }
  if (const auto pValue = std::get_if<bool>(&type)) { return Node(*pValue); }
  if (const auto pValue = std::get_if<std::string>(&type)) { return Node(*pValue); }
  if ([[maybe_unused]] auto pValue = std::get_if<std::nullptr_t>(&type)) { return Node(nullptr); }
  if (const auto pValue = std::get_if<Node>(&type)) { return std::move(*const_cast<Node *>(pValue)); }
  throw Node::Error("Node for unsupported type could not be created.");
}
// Construct Node Array from initializer list
inline Node::Node(const JSON::ArrayInitializer &array)
{
  *this = make<Array>();
  for (const auto &entry : array) { JRef<Array>(*this).add(typeToNode(entry)); }
}
// Construct Node Object from initializer list
inline Node::Node(const JSON::ObjectInitializer &object)
{
  *this = make<Object>();
  for (const auto &[fst, snd] : object) { JRef<Object>(*this).add(Object::Entry(fst, typeToNode(snd))); }
}

}// namespace JSON_Lib
