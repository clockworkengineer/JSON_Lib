#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "ISource.hpp"

namespace JSON_Lib {

class BufferSource : public ISource
{
public:
  explicit BufferSource(const std::string &buffer) : buffer(buffer)
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
    if (more()) { return (buffer[bufferPosition]); }
    return (EOF);
  }
  virtual void next() override
  {
    if (!more()) { throw Error("Tried to read past and of buffer."); }
    bufferPosition++;
    column++;
    if (current() == kLineFeed) {
      lineNo++;
      column = 1;
    }
  }
  virtual bool more() const override { return (bufferPosition < buffer.size()); }
  virtual void reset() override
  {
    bufferPosition = 0;
    lineNo = 1;
    column = 1;
  }
  virtual std::size_t position() const override { return (bufferPosition); }

private:
  virtual void backup(unsigned long length) override { bufferPosition -= length; }

  std::size_t bufferPosition = 0;
  std::string buffer;
};
}// namespace JSON_Lib
