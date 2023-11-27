#pragma once

#include <string>
#include <stdexcept>

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
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("IParser Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  IParser() = default;
  IParser(const IParser &other) = delete;
  IParser &operator=(const IParser &other) = delete;
  IParser(IParser &&other) = delete;
  IParser &operator=(IParser &&other) = delete;
  virtual ~IParser() = default;
  // =================
  // Parse JSON source
  // =================
  [[nodiscard]] virtual JNode parse(ISource &source) const = 0;
};
}// namespace JSON_Lib