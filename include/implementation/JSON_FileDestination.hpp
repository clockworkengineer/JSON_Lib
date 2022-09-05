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
class FileDestination : public IDestination
{
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  explicit FileDestination(const std::string &filename) : m_filename(filename)
  {
    m_destination.open(filename.c_str(), std::ios_base::binary | std::ios_base::trunc);
  }
  FileDestination() = delete;
  FileDestination(const FileDestination &other) = delete;
  FileDestination &operator=(const FileDestination &other) = delete;
  FileDestination(FileDestination &&other) = delete;
  FileDestination &operator=(FileDestination &&other) = delete;
  virtual ~FileDestination() = default;
  // ==============
  // PUBLIC METHODS
  // ==============
  virtual void add(const char ch) override
  {
    if (ch == '\n') {
      m_destination.write("\r\n", 2);
    } else {
      m_destination.put(ch);
    }
    m_destination.flush();
  }
  virtual void add(const std::string &bytes) override
  {
    for (auto ch : bytes) { add(ch); }
    m_destination.flush();
  }
  virtual void clear() override
  {
    if (m_destination.is_open()) { m_destination.close(); }
    m_destination.open(m_filename.c_str(), std::ios_base::binary | std::ios_base::trunc);
    if (!m_destination.is_open()) { throw Error("File output stream failed to open or could not be created."); }
  }
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
  std::ofstream m_destination;
  std::string m_filename;
};
}// namespace JSONLib
