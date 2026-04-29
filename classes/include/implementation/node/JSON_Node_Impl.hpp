#pragma once

#include "JSON_Node.hpp"
#include <variant>

namespace JSON_Lib {

inline Variant &Node::getVariantImpl(Storage &storage)
{
  return std::visit([](auto &value) -> Variant & {
    using ValueType = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<ValueType, std::monostate>) {
      throw Error("Node does not contain a variant.");
    } else if constexpr (std::is_same_v<ValueType, std::unique_ptr<Object>> || std::is_same_v<ValueType, std::unique_ptr<Array>>) {
      return *value;
    } else {
      return value;
    }
  }, storage);
}

inline const Variant &Node::getVariantImpl(const Storage &storage)
{
  return std::visit([](const auto &value) -> const Variant & {
    using ValueType = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<ValueType, std::monostate>) {
      throw Error("Node does not contain a variant.");
    } else if constexpr (std::is_same_v<ValueType, std::unique_ptr<Object>> || std::is_same_v<ValueType, std::unique_ptr<Array>>) {
      return *value;
    } else {
      return value;
    }
  }, storage);
}

}// namespace JSON_Lib
