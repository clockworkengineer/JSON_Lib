#pragma once

#include "JSON_Config.hpp"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace JSON_Lib {
namespace detail {

#if !JSON_LIB_EMBEDDED

/// @brief Transparent hash for string keys allowing string_view lookup.
struct StringHash
{
  using is_transparent = void;
  [[nodiscard]] std::size_t operator()(std::string_view value) const noexcept
  {
    return std::hash<std::string_view>{}(value);
  }
  [[nodiscard]] std::size_t operator()(const std::string &value) const noexcept
  {
    return std::hash<std::string_view>{}(value);
  }
};

/// @brief Transparent equality comparator for string keys.
struct StringEqual
{
  using is_transparent = void;
  [[nodiscard]] bool operator()(std::string_view lhs, std::string_view rhs) const noexcept { return lhs == rhs; }
  [[nodiscard]] bool operator()(const std::string &lhs, const std::string &rhs) const noexcept { return lhs == rhs; }
};

/// @brief Hash-map object index (desktop build).
using ObjectIndex = std::unordered_map<std::string, std::size_t, StringHash, StringEqual>;

#else

/// @brief Sorted-vector object index (embedded build).
using ObjectIndex = std::vector<std::size_t>;

#endif

}// namespace detail
}// namespace JSON_Lib
