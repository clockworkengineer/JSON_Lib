#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "ISource.hpp"

namespace JSON_Lib {

class BufferSource : public ISource
{
public:
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

  virtual char current() const override
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
  virtual bool more() const override { return (m_position < m_buffer.size()); }
  virtual void reset() override
  {
    m_position = 0;
    m_lineNo = 1;
    m_column = 1;
  }
  virtual std::size_t position() const override { return (m_position); }

private:
  virtual void backup(unsigned long length) override { m_position -= length; }

  std::size_t m_position = 0;
  std::string m_buffer;
};
}// namespace JSON_Lib
