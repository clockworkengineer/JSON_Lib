#pragma once
// =======
// C++ STL
// =======
#include <filesystem>
#include <fstream>
#include <string>

// ================
// Source interface
// ================
#include "ISource.hpp"
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==============================
// Source classes for JSON parser
// ==============================
// ======
// Buffer
// ======
class BufferSource : public ISource {
public:
  explicit BufferSource(const std::string &sourceBuffer)
      : m_parseBuffer(sourceBuffer) {
    if (sourceBuffer.empty()) {
      throw Error("Empty source buffer passed to be parsed.");
    }
  }
  [[nodiscard]] char current() const override {
    if (more()) {
      return (m_parseBuffer[m_bufferPosition]);
    }
    return (EOF);
  }
  void next() override {
    if (!more()) {
      throw Error("Tried to read past and of buffer.");
    }
    m_bufferPosition++;
  }
  [[nodiscard]] bool more() const override {
    return (m_bufferPosition < m_parseBuffer.size());
  }
  void backup(unsigned long length) override {
    // Make sure to dont backup past the beginning
    if (length >= m_bufferPosition) {
      m_bufferPosition = 0;
    } else {
      m_bufferPosition -= length;
    }
  }
  void reset() override { m_bufferPosition = 0; }
  [[nodiscard]] std::size_t position() const override {
    return (m_bufferPosition);
  }

private:
  std::size_t m_bufferPosition = 0;
  std::string m_parseBuffer;
};
// ====
// File
// ====
class FileSource : public ISource {
public:
  explicit FileSource(const std::string &sourceFileName)
      : m_sourceFileName(sourceFileName) {
    m_source.open(sourceFileName.c_str(), std::ios_base::binary);
    if (!m_source.is_open()) {
      throw Error("File input stream failed to open or does not exist.");
    }
  }
  char current() const override { return (static_cast<char>(m_source.peek())); }
  void next() override {
    if (!more()) {
      throw Error("Tried to read past end of file.");
    }
    char c = 0;
    m_source.get(c);
  }
  bool more() const override { return (m_source.peek() != EOF); }
  void backup(unsigned long length) override {
    // Make sure to dont backup past the beginning
    if ((length <= m_source.tellg()) || (current() == (char)EOF)) {
      m_source.clear();
      m_source.seekg(-static_cast<long>(length), std::ios_base::cur);
    } else {
      m_source.seekg(0, std::ios_base::beg);
    }
  }
  void reset() override {
    m_source.clear();
    m_source.seekg(0, std::ios_base::beg);
  }
  std::size_t position() const override {
    if (more()) {
      return (static_cast<std::size_t>(m_source.tellg()));
    }
    return (std::filesystem::file_size(m_sourceFileName));
  }

private:
  mutable std::ifstream m_source;
  std::string m_sourceFileName;
};
} // namespace JSONLib