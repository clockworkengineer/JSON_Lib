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
  struct Error : public std::runtime_error
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
  virtual long getIndent() const = 0;
  virtual void setIndent(long indent) = 0;
};
}// namespace JSON_Lib