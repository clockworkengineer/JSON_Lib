#pragma once

#include "implementation/common/JSON_Attributes.hpp"
#include "JSON_ErrorBase.hpp"

namespace JSON_Lib {

// ====================================================
// Interface for translation to/from escaped characters
// ====================================================
/// @brief Interface for translating between raw UTF-8 strings and JSON escape sequences.
///
/// Implement this interface to supply a custom escape/unescape strategy,
/// for example to support non-standard encodings or extended Unicode ranges.
class ITranslator
{
public:
  /// @brief Exception type thrown by ITranslator implementations.
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("ITranslator", message)) {}
  };
  virtual ~ITranslator() = default;

  /// @brief Convert a JSON-escaped string to its raw UTF-8 representation.
  ///
  /// Processes all @c \\uXXXX, @c \\n, @c \\t and other JSON escape
  /// sequences and returns the equivalent UTF-8 byte sequence.
  /// @param escapedString A string that may contain JSON escape sequences.
  /// @return The decoded UTF-8 string.
  /// @throws ITranslator::Error on malformed escape sequences.
  JSON_LIB_NODISCARD virtual std::string from(const std::string_view &escapedString) const = 0;

  /// @brief Convert a raw UTF-8 string to its JSON-escaped representation.
  ///
  /// Replaces characters that must be escaped in JSON (e.g. @c \", @c \\,
  /// control characters, and non-BMP code points via surrogate pairs).
  /// @param rawString A raw UTF-8 string.
  /// @return The JSON-escaped string, suitable for embedding inside JSON quotes.
  /// @throws ITranslator::Error on invalid UTF-8 input.
  JSON_LIB_NODISCARD virtual std::string to(const std::string_view &rawString) const = 0;
};
}// namespace JSON_Lib