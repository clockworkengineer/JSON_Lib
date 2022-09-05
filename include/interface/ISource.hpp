#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace JSONLib {
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
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("ISource Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  ISource() = default;
  ISource(const ISource &other) = delete;
  ISource &operator=(const ISource &other) = delete;
  ISource(ISource &&other) = delete;
  ISource &operator=(ISource &&other) = delete;
  virtual ~ISource() = default;
  // =================
  // Current character
  // =================
  [[nodiscard]] virtual char current() const = 0;
  // ======================
  // Move to next character
  // ======================
  virtual void next() = 0;
  // =======================================
  // Are there still more characters to read
  // ========================================
  [[nodiscard]] virtual bool more() const = 0;
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
  [[nodiscard]] virtual std::size_t position() const = 0;
  // ===================================
  // Is the current character whitespace
  // ===================================
  [[nodiscard]] bool isWS() const
  {
    return (current() == ' ' || current() == '\t' || current() == '\n' || current() == '\r');
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
  [[nodiscard]] bool match(const std::string &targetString)
  {
    long index = 0;
    while (more() && current() == targetString[index]) {
      next();
      if (++index == static_cast<long>(targetString.length())) { return (true); }
    }
    backup(index);
    return (false);
  }
  // ==================================
  // Get current source stream position
  // ==================================
  std::pair<long, long> getPosition() const { return (std::make_pair(m_lineNo, m_column)); }

protected:
  // ========================
  // Backup length characters
  // ========================
  virtual void backup(unsigned long length) = 0;
  // ========================================
  // Current line and column on source stream
  // ========================================
  long m_lineNo = 1;
  long m_column = 1;
};
}// namespace JSONLib