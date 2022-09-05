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
// ================
// CLASS DEFINITION
// ================
class FileSource : public ISource
{
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // ======================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  explicit FileSource(const std::string &filename) : m_filename(filename)
  {
    m_source.open(filename.c_str(), std::ios_base::binary);
    if (!m_source.is_open()) { throw Error("File input stream failed to open or does not exist."); }
  }
  FileSource() = delete;
  FileSource(const FileSource &other) = delete;
  FileSource &operator=(const FileSource &other) = delete;
  FileSource(FileSource &&other) = delete;
  FileSource &operator=(FileSource &&other) = delete;
  virtual ~FileSource() = default;
  // ==============
  // PUBLIC METHODS
  // ==============
  virtual [[nodiscard]] char current() const override { return (static_cast<char>(m_source.peek())); }
  virtual void next() override
  {
    if (!more()) { throw Error("Tried to read past end of file."); }
    m_source.get();
    if (current() == kCarriageReturn) {
      m_source.get();
      if (current() != kLineFeed) { m_source.unget(); }
    }
    m_column++;
    if (current() == kLineFeed) {
      m_lineNo++;
      m_column = 1;
    }
  }
  virtual bool more() const override { return (m_source.peek() != EOF); }
  virtual void reset() override
  {
    m_lineNo = 1;
    m_column = 1;
    m_source.clear();
    m_source.seekg(0, std::ios_base::beg);
  }
  virtual [[nodiscard]] std::size_t position() const override
  {
    if (more()) { return (static_cast<std::size_t>(m_source.tellg())); }
    return (std::filesystem::file_size(m_filename));
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
  virtual void backup(unsigned long length) override
  {
    m_source.clear();
    m_source.seekg(-static_cast<long>(length), std::ios_base::cur);
  }
  // =================
  // PRIVATE VARIABLES
  // =================
  mutable std::ifstream m_source;
  std::string m_filename;
};
}// namespace JSONLib
