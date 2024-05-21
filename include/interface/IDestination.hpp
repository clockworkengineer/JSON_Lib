#pragma once

#include <string>
#include <stdexcept>

namespace JSON_Lib {
  
// ====================================================================
// Interface for writing destination stream during JSON stringification
// ====================================================================
class IDestination
{
public:
  // ==================
  // IDestination Error
  // ==================
  struct Error final : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("IDestination Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IDestination() = default;
  // ========================
  // Add bytes to destination
  // ========================
  virtual void add(const std::string &bytes) = 0;
  // ============================
  // Add character to destination
  // ============================
  virtual void add(char ch) = 0;
  // ============================
  // Clear the curent destination
  // ===========================
  virtual void clear() = 0;
};
}// namespace JSON_Lib
