#pragma once

#include "implementation/common/JSON_Attributes.hpp"
#include "JSON_ErrorBase.hpp"

namespace JSON_Lib {

// ====================
// Forward declarations
// ====================
class IDestination;
struct Node;

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
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("IStringify", message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IStringify() = default;
  // ====================
  // Stringify Node tree
  // ====================
  virtual void stringify(const Node &jNode, IDestination &destination, unsigned long indent) const = 0;
  // =========================
  // Set/Get print indentation
  // =========================
  JSON_LIB_NODISCARD virtual long getIndent() const JSON_LIB_NOEXCEPT { return 0; }
  virtual void setIndent(long) JSON_LIB_NOEXCEPT {}
};
// Make a custom stringify to pass to the JSON constructor.
template <typename T> std::unique_ptr<IStringify> makeStringify() {
  return std::make_unique<T>();
}
}// namespace JSON_Lib