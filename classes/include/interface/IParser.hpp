#pragma once

namespace JSON_Lib {

// ====================
// Forward declarations
// ====================

class ISource;
struct JNode;

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
    explicit Error(const std::string_view &message) : std::runtime_error(std::string("IParser Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IParser() = default;
  // =================
  // Parse JSON source
  // =================
virtual JNode parse(ISource &source) = 0;
};
}// namespace JSON_Lib