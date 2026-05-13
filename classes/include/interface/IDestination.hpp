#pragma once

#include "JSON_ErrorBase.hpp"

namespace JSON_Lib {
  
// ====================================================================
// Interface for writing destination stream during JSON stringification
// ====================================================================
/// @brief Interface for an output sink consumed during JSON stringification.
///
/// Implementations may write to an in-memory buffer, a file, a network
/// socket, or any other byte-oriented output channel.
class IDestination
{
public:
  /// @brief Exception type thrown by IDestination implementations.
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("IDestination", message)) {}
  };
  virtual ~IDestination() = default;

  /// @brief Append a std::string to the destination.
  /// @param bytes The string to append.
  virtual void add(const std::string &bytes) = 0;
  /// @brief Append a null-terminated C string to the destination.
  /// @param bytes Pointer to the null-terminated character sequence.
  virtual void add(const char *bytes) = 0;
  /// @brief Append a string_view to the destination.
  /// @param bytes The string_view to append.
  virtual void add(const std::string_view &bytes) = 0;
  /// @brief Append a single character to the destination.
  /// @param ch The character to append.
  virtual void add(char ch) = 0;
  /// @brief Reset the destination to an empty state.
  virtual void clear() = 0;
  /// @brief Return the last character that was written.
  /// @return The most recently written character, or '\0' if nothing written.
  virtual char last() = 0;
};
}// namespace JSON_Lib
