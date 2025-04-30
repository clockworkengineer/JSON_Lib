#pragma once

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
    explicit Error(const std::string_view &message) : std::runtime_error(std::string("IStringify Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IStringify() = default;
  // ====================
  // Stringify JNode tree
  // ====================
  virtual void stringify(const JNode &jNode, IDestination &destination, unsigned long indent) const = 0;
  // =========================
  // Set/Get print indentation
  // =========================
  [[nodiscard]] virtual long getIndent() const { return 0; }
  virtual void setIndent([[maybe_unused]] long indent)  {}
};
// Make custom stringify to pass to JSON constructor: Pointer is tidied up internally.
template <typename T> IStringify *makeStringify() {
  return std::make_unique<T>().release();
}
}// namespace JSON_Lib