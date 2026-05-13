#pragma once

#include <string_view>
#include <utility>
#include "implementation/common/JSON_Attributes.hpp"
#include "JSON_ErrorBase.hpp"

namespace JSON_Lib {
  
// ========================
// JSON character constants
// ========================
constexpr char kCarriageReturn{ 0x0D };
constexpr char kLineFeed{ 0x0A };

// =======================================================
// Interface for reading source stream during JSON parsing
// =======================================================
/// @brief Interface for a character-oriented input stream consumed by the JSON parser.
///
/// Implement this interface to feed JSON text from any source — buffer,
/// file, network stream, etc. — into the parser.
class ISource
{
public:
  /// @brief Exception type thrown by ISource implementations.
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("ISource", message)) {}
  };
  virtual ~ISource() = default;

  /// @brief Return the character at the current read position.
  /// @return The current character.
  JSON_LIB_NODISCARD virtual char current() const JSON_LIB_NOEXCEPT = 0;

  /// @brief Advance the read position to the next character.
  virtual void next() = 0;

  /// @brief Return @c true if there are more characters to read.
  /// @return @c true while the stream is not exhausted.
  JSON_LIB_NODISCARD virtual bool more() const JSON_LIB_NOEXCEPT = 0;

  /// @brief Reset the read position to the beginning of the stream.
  virtual void reset() = 0;

  /// @brief Return the current byte offset within the source stream.
  /// @return Zero-based byte position of the current character.
  JSON_LIB_NODISCARD virtual std::size_t position() const = 0;

  /// @brief Return @c true if the current character is ASCII whitespace.
  /// @return @c true for space, tab, newline, or carriage-return.
  JSON_LIB_NODISCARD bool isWS() const
  {
    return current() == ' ' || current() == '\t' || current() == '\n' || current() == '\r';
  }

  /// @brief Advance past all leading whitespace characters.
  void ignoreWS()
  {
    while (more() && isWS()) { next(); }
  }
  // ===============================================================
  // Is current string a match at the current source stream position
  // ===============================================================
  /// @brief Test whether @p targetString matches the bytes at the current position.
  ///
  /// If the match succeeds the stream is advanced past the matched characters;
  /// otherwise the stream position is restored.
  /// @param targetString The literal string to match.
  /// @return @c true if all characters of @p targetString matched.
  JSON_LIB_NODISCARD bool match(const std::string_view &targetString)
  {
    long index = 0;
    while (more() && current() == targetString[index]) {
      next();
      if (++index == static_cast<long>(targetString.size())) { return true; }
    }
    backup(index);
    return false;
  }
  // ==================================
  // Get current source stream position
  // ==================================
  /// @brief Return the current 1-based line and column numbers.
  /// @return A pair (lineNo, column) reflecting the current read position.
  JSON_LIB_NODISCARD std::pair<long, long> getPosition() const JSON_LIB_NOEXCEPT { return std::make_pair(lineNo, column); }

protected:
  /// @brief Retreat the stream position by @p length characters.
  /// @param length Number of characters to back up.
  virtual void backup(unsigned long length) = 0;
  long lineNo = 1; ///< Current 1-based line number.
  long column  = 1; ///< Current 1-based column number.
};
}// namespace JSON_Lib