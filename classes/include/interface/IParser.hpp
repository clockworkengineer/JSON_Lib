#pragma once

#include <memory>
#include "implementation/common/JSON_Error.hpp"

namespace JSON_Lib {

// ====================
// Forward declarations
// ====================

class ISource;
struct Node;

// =========================
// Interface for JSON parser
// =========================
class IParser
{
public:
  // =============
  // IParser Error
  // =============
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("IParser", message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IParser() = default;
  // =================
  // Parse JSON source
  // =================
  virtual Node parse(ISource &source) = 0;
  virtual Result<Node> parseResult(ISource &source);
};
}// namespace JSON_Lib