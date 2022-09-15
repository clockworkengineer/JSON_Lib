#pragma once
// =======
// C++ STL
// =======
#include <string>
#include <stdexcept>
// =========
// NAMESPACE
// =========
namespace JSON_Lib {
// ====================================================================
// Interface for writing destination stream during JSON stringification
// ====================================================================
class IDestination {
public:
  // ==================
  // IDestination Error
  // ==================
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("IDestination Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  IDestination() = default;
  IDestination(const IDestination &other) = delete;
  IDestination &operator=(const IDestination &other) = delete;
  IDestination(IDestination &&other) = delete;
  IDestination &operator=(IDestination &&other) = delete;
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
} // namespace JSON_Lib
