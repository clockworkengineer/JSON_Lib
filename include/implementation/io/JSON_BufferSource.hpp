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
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ================
// CLASS DEFINITION
// ================
class BufferSource : public ISource
{
public:
  // ==========================
  // PUBLIC TYPES AND CONSTANTS
  // ==========================
  // CONSTRUCTOR/DESTRUCTOR
  // ======================
  explicit BufferSource(const std::string &buffer) : m_buffer(buffer)
  {
    if (buffer.empty()) { throw Error("Empty source buffer passed to be parsed."); }
  }
  BufferSource() = delete;
  BufferSource(const BufferSource &other) = delete;
  BufferSource &operator=(const BufferSource &other) = delete;
  BufferSource(BufferSource &&other) = delete;
  BufferSource &operator=(BufferSource &&other) = delete;
  virtual ~BufferSource() = default;
  // ==============
  // PUBLIC METHODS
  // ==============
  virtual [[nodiscard]] char current() const override
  {
    if (more()) { return (m_buffer[m_position]); }
    return (EOF);
  }
  virtual void next() override
  {
    if (!more()) { throw Error("Tried to read past and of buffer."); }
    m_position++;
    m_column++;
    if (current() == kLineFeed) {
      m_lineNo++;
      m_column = 1;
    }
  }
  virtual [[nodiscard]] bool more() const override { return (m_position < m_buffer.size()); }
  virtual void reset() override
  {
    m_position = 0;
    m_lineNo = 1;
    m_column = 1;
  }
  virtual [[nodiscard]] std::size_t position() const override { return (m_position); }
  /// <summary>
  /// Open a JSON file and read its contents into the buffer.
  /// </summary>
  /// <param name="jsonFileName">JSON file name</param>
  void fromFile(const std::string &jsonFileName)
  {
    std::ifstream jsonFile;
    jsonFile.open(jsonFileName, std::ios_base::binary);
    std::ostringstream jsonFileBuffer;
    jsonFileBuffer << jsonFile.rdbuf();
    m_buffer = jsonFileBuffer.str();
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
  virtual void backup(unsigned long length) override { m_position -= length; }
  // =================
  // PRIVATE VARIABLES
  // =================
  std::size_t m_position = 0;
  std::string m_buffer;
};
}// namespace JSON_Lib
