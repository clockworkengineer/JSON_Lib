#pragma once
#include "JSON_Throw.hpp"
#include "ISource.hpp"

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
    checkNotEmpty(this->buffer.empty());
  }

  explicit BufferSource(std::string &&buffer) : ownedBuffer(std::move(buffer)), buffer(ownedBuffer)
  {
    checkNotEmpty(ownedBuffer.empty());
  }

  explicit BufferSource(const char *buffer) : buffer(buffer, buffer ? std::strlen(buffer) : 0)
  {
    checkNotEmpty(buffer == nullptr || buffer[0] == '\0');
  }

  explicit BufferSource(const std::string_view &buffer) : buffer(buffer)
  {
    checkNotEmpty(buffer.empty());
  }

  BufferSource(const char *buffer, std::size_t length) : buffer(buffer, length)
  {
    checkNotEmpty(buffer == nullptr || length == 0);
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
    if (!more()) { JSON_THROW(Error("Tried to read past and of buffer.")); }
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
  static void checkNotEmpty(bool empty)
  {
    if (empty) { JSON_THROW(Error("Empty source buffer passed to be parsed.")); }
  }
  void backup(const unsigned long length) override { bufferPosition -= length; }

  std::size_t bufferPosition = 0;
  std::string ownedBuffer;
  std::string_view buffer;
};
}// namespace JSON_Lib
