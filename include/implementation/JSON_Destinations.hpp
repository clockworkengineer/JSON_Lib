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
// ========
// Buffered
// ========
class BufferDestination : public IDestination {
public:
  BufferDestination() = default;
  void add(const std::string &bytes) override {
    for (auto b : bytes) {
      m_stringifyBuffer.push_back(b);
    }
  }
  void add(const char ch) override { m_stringifyBuffer.push_back(ch); }
  [[nodiscard]] std::string getBuffer() const { return (m_stringifyBuffer); }

private:
  std::string m_stringifyBuffer;
};
// ====
// File
// ====
class FileDestination : public IDestination {
public:
  explicit FileDestination(const std::string &destinationFileName) {
    m_destination.open(destinationFileName.c_str(), std::ios_base::binary);
    if (!m_destination.is_open()) {
      throw Error("File output stream failed to open or could not be created.");
    }
  }
  void add(const std::string &bytes) override {
    m_destination.write(bytes.c_str(), bytes.length());
    m_destination.flush();
  }
  void add(const char ch) override {
    m_destination.put(ch);
    m_destination.flush();
  }

private:
  std::ofstream m_destination;
};
} // namespace JSONLib