#pragma once

namespace JSON_Lib {

// Construct JNode from raw values
template<typename T> JNode::JNode(T value)
{
  if constexpr (std::is_same_v<T, bool>) {
    *this = JNode::make<Boolean>(value);
  } else if constexpr (std::is_arithmetic_v<T>) {
    *this = JNode::make<Number>(value);
  } else if constexpr (std::is_same_v<T, std::nullptr_t>) {
    *this = make<Null>();
  } else if constexpr (std::is_same_v<T, const char *> || std::is_same_v<T, std::string>) {
    *this = JNode::make<String>(value);
  } else if constexpr (std::is_convertible_v<T, std::unique_ptr<Variant>>) {
    jNodeVariant = std::move(value);
  }
}
// Convert an initializer list type to JMode
static JNode typeToJNode(const JSON::InitializerListTypes &type)
{
  if (const auto pValue = std::get_if<int>(&type)) { return JNode(*pValue); }
  if (const auto pValue = std::get_if<long>(&type)) { return JNode(*pValue); }
  if (const auto pValue = std::get_if<long long>(&type)) { return JNode(*pValue); }
  if (const auto pValue = std::get_if<float>(&type)) { return JNode(*pValue); }
  if (const auto pValue = std::get_if<double>(&type)) { return JNode(*pValue); }
  if (const auto pValue = std::get_if<long double>(&type)) { return JNode(*pValue); }
  if (const auto pValue = std::get_if<bool>(&type)) { return JNode(*pValue); }
  if (const auto pValue = std::get_if<std::string>(&type)) { return JNode(*pValue); }
  if ([[maybe_unused]] auto pValue = std::get_if<std::nullptr_t>(&type)) { return JNode(nullptr); }
  if (const auto pValue = std::get_if<JNode>(&type)) { return std::move(*const_cast<JNode *>(pValue)); }
  throw JNode::Error("JNode for unsupported type could not be created.");
}
// Construct JNode Array from initializer list
inline JNode::JNode(const JSON::ArrayInitializer &array)
{
  *this = make<Array>();
  for (const auto &entry : array) { JRef<Array>(*this).add(typeToJNode(entry)); }
}
// Construct JNode Object from initializer list
inline JNode::JNode(const JSON::ObjectInitializer &object)
{
  *this = make<Object>();
  for (const auto &[fst, snd] : object) { JRef<Object>(*this).add(Object::Entry(fst, typeToJNode(snd))); }
}

}// namespace JSON_Lib
