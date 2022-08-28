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
    for (auto ch : bytes) {
      if (ch == '\n') {
        m_destination.write("\r\n", 2);
      } else {
        m_destination.put(ch);
      }
    }
    m_destination.flush();
  }
  void add(const char ch) override
  {
    if (ch == '\n') {
      m_destination.write("\r\n", 2);
    } else {
      m_destination.put(ch);
    }
    m_destination.flush();
  }
  void clear() override
  {
    if (m_destination.is_open()) { m_destination.close(); }
    m_destination.open(m_destinationFileName.c_str(), std::ios_base::binary | std::ios_base::trunc);
    if (!m_destination.is_open()) { throw Error("File output stream failed to open or could not be created."); }
  }

private:
  std::ofstream m_destination;
  std::string m_destinationFileName;
};
}// namespace JSONLib