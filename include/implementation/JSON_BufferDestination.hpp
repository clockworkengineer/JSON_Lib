#pragma once
// =======
// C++ STL
// =======
#include <fstream>
#include <string>
// =====================
// Destination interface
// =====================
#include "IDestination.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ================
// CLASS DEFINITION
// ================
class BufferDestination : public IDestination
{
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  BufferDestination() = default;
  BufferDestination(const BufferDestination &other) = delete;
  BufferDestination &operator=(const BufferDestination &other) = delete;
  BufferDestination(BufferDestination &&other) = delete;
  BufferDestination &operator=(BufferDestination &&other) = delete;
  virtual ~BufferDestination() = default;
  // ==============
  // PUBLIC METHODS
  // ==============
  void add(const std::string &bytes) override
  {
    for (auto b : bytes) { m_buffer.push_back(b); }
  }
  void add(const char ch) override { m_buffer.push_back(ch); }
  void clear() override { m_buffer.clear(); }
  [[nodiscard]] const std::string &getBuffer() const { return (m_buffer); }
  // ================
  // PUBLIC VARIABLES
  // ================
private:
  // ===========================
  // PRIVATE TYPES AND CONSTANTS
  // ===========================
  // ===============
  // PRIVATE METHODS
  // ===============
  // =================
  // PRIVATE VARIABLES
  // =================
  std::string m_buffer;
};
}// namespace JSONLib
