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
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
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
  virtual void add(const std::string &bytes) override
  {
    for (auto b : bytes) { m_buffer.push_back(b); }
  }
  virtual void add(const char ch) override { m_buffer.push_back(ch); }
  virtual void clear() override { m_buffer.clear(); }
  [[nodiscard]] const std::string &getBuffer() const { return (m_buffer); }
  /// <summary>
  /// Create an JSON file and write JSON from buffer to it.
  /// </summary>
  /// <param name="jsonFileName">JSON file name</param>
  /// <returns></returns>
  void toFile(const std::string &jsonFileName)
  {
    std::remove(jsonFileName.c_str());
    std::ofstream jsonFile; 
    jsonFile.open(jsonFileName, std::ios::binary);
    jsonFile << m_buffer;
    jsonFile.close();
  }
  /// <summary>
  /// Return reference to buffer base.
  /// </summary>
  /// <returns>Reference to start of buffer.</returns>
  const std::string &buffer() { return (m_buffer); }
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
}// namespace JSON_Lib
