#pragma once

#include <string>
#include <stdexcept>

namespace JSON_Lib {

// ====================
// Forward declarations
// ====================
class IDestination;
struct JNode;

// ==================================
// Interface for JSON stringification
// ==================================
class IStringify
{
public:
  // ================
  // IStringify Error
  // ================
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("IStringify Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IStringify() = default;
  // ====================
  // Stringify JNode tree
  // ====================
  virtual void stringify(const JNode &jNode, IDestination &destination, long indent) const = 0;
  // =========================
  // Set/Get print indentation
  // =========================
  [[nodiscard]] virtual long getIndent() const = 0;
  virtual void setIndent(long indent) = 0;
};
// Make custom stringify to pass to JSON constructor: Note pointer is tidied up internally.
template <typename T> IStringify *makeStringify() {
  return std::make_unique<T>().release();
}
}// namespace JSON_Lib