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
// ===================================
// Destination classes for JSON parser
// ===================================
// ======
// Buffer
// ======
class BufferDestination : public IDestination
{
public:
  BufferDestination() = default;
  void add(const std::string &bytes) override
  {
    for (auto b : bytes) { m_stringifyBuffer.push_back(b); }
  }
  void add(const char ch) override { m_stringifyBuffer.push_back(ch); }
  void clear() override { m_stringifyBuffer.clear(); }
  [[nodiscard]] std::string getBuffer() const { return (m_stringifyBuffer); }
  // Note: This backup strinks the destination and will not go past the beginning
  virtual void backup()
  {
    if (!m_stringifyBuffer.empty()) { m_stringifyBuffer.pop_back(); }
  }

private:
  std::string m_stringifyBuffer;
};
// ====
// File
// ====
class FileDestination : public IDestination
{
public:
  explicit FileDestination(const std::string &destinationFileName) : m_destinationFileName(destinationFileName)
  {
    m_destination.open(destinationFileName.c_str(), std::ios_base::binary | std::ios_base::trunc);
  }
  void add(const std::string &bytes) override
  {
    m_destination.write(bytes.c_str(), bytes.length());
    m_destination.flush();
  }
  void add(const char ch) override
  {
    m_destination.put(ch);
    m_destination.flush();
  }
  void clear() override
  {
    if (m_destination.is_open()) { m_destination.close(); }
    m_destination.open(m_destinationFileName.c_str(), std::ios_base::binary | std::ios_base::trunc);
    if (!m_destination.is_open()) { throw Error("File output stream failed to open or could not be created."); }
  }
  // Note: This backup does not strink the destination but just changes the next position written to; allowing
  // existing characters to be overwritten (this is for performance reasons). Also note that you cannot backup
  // past the beginning of the file like the buffer variant.
  virtual void backup()
  {
    if (m_destination.tellp() > 0) { m_destination.seekp(-1, std::ios_base::cur); }
  }

private:
  std::ofstream m_destination;
  std::string m_destinationFileName;
};
}// namespace JSONLib