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
class ISource
{
public:
  // =============
  // ISource Error
  // =============
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("ISource", message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~ISource() = default;
  // =================
  // Current character
  // =================
  JSON_LIB_NODISCARD virtual char current() const JSON_LIB_NOEXCEPT = 0;
  // ======================
  // Move to next character
  // ======================
  virtual void next() = 0;
  // =======================================
  // Are there still more characters to read
  // ========================================
  JSON_LIB_NODISCARD virtual bool more() const JSON_LIB_NOEXCEPT = 0;
  // // ========================
  // // Backup length characters
  // // ========================
  // virtual void backup(unsigned long length) = 0;
  // ===================================
  // Reset to beginning of source stream
  // ===================================
  virtual void reset() = 0;
  // ===============================================
  // Current character position within source stream
  // ===============================================
  JSON_LIB_NODISCARD virtual std::size_t position() const = 0;
  // ===================================
  // Is the current character whitespace
  // ===================================
  JSON_LIB_NODISCARD bool isWS() const
  {
    return current() == ' ' || current() == '\t' || current() == '\n' || current() == '\r';
  }
  // ==================================
  // Ignore whitespace on source stream
  // ==================================
  void ignoreWS()
  {
    while (more() && isWS()) { next(); }
  }
  // ===============================================================
  // Is current string a match at the current source stream position
  // ===============================================================
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
  JSON_LIB_NODISCARD std::pair<long, long> getPosition() const JSON_LIB_NOEXCEPT { return std::make_pair(lineNo, column); }

protected:
  // ========================
  // Backup length characters
  // ========================
  virtual void backup(unsigned long length) = 0;
  // ========================================
  // Current line and column on source stream
  // ========================================
  long lineNo = 1;
  long column = 1;
};
}// namespace JSON_Lib