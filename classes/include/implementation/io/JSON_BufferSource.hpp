#pragma once

#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>

namespace JSON_Lib {

class BufferSource final : public ISource
{
public:
  explicit BufferSource(const std::string &buffer) : ownedBuffer(buffer), buffer(ownedBuffer)
  {
    if (this->buffer.empty()) { throw Error("Empty source buffer passed to be parsed."); }
  }

  explicit BufferSource(std::string &&buffer) : ownedBuffer(std::move(buffer)), buffer(ownedBuffer)
  {
    if (ownedBuffer.empty()) { throw Error("Empty source buffer passed to be parsed."); }
  }

  explicit BufferSource(const char *buffer) : buffer(buffer, buffer ? std::strlen(buffer) : 0)
  {
    if (buffer == nullptr || buffer[0] == '\0') { throw Error("Empty source buffer passed to be parsed."); }
  }

  explicit BufferSource(const std::string_view &buffer) : buffer(buffer)
  {
    if (buffer.empty()) { throw Error("Empty source buffer passed to be parsed."); }
  }

  BufferSource(const char *buffer, std::size_t length) : buffer(buffer, length)
  {
    if (buffer == nullptr || length == 0) { throw Error("Empty source buffer passed to be parsed."); }
  }

  BufferSource() = delete;
  BufferSource(const BufferSource &other) = delete;
  BufferSource &operator=(const BufferSource &other) = delete;
  BufferSource(BufferSource &&other) = delete;
  BufferSource &operator=(BufferSource &&other) = delete;
  ~BufferSource() override = default;

  [[nodiscard]] char current() const override
  {
    if (more()) { return buffer[bufferPosition]; }
    return EOF;
  }

  void next() override
  {
    if (!more()) { throw Error("Tried to read past and of buffer."); }
    bufferPosition++;
    column++;
    if (current() == kLineFeed) {
      lineNo++;
      column = 1;
    }
  }

  [[nodiscard]] bool more() const override { return bufferPosition < buffer.size(); }

  void reset() override
  {
    bufferPosition = 0;
    lineNo = 1;
    column = 1;
  }

  [[nodiscard]] std::size_t position() const override { return bufferPosition; }

private:
  void backup(const unsigned long length) override { bufferPosition -= length; }

  std::size_t bufferPosition = 0;
  std::string ownedBuffer;
  std::string_view buffer;
};
}// namespace JSON_Lib
