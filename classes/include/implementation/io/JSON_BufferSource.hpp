#pragma once
#include "JSON_Throw.hpp"
#include "ISource.hpp"

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

  /// @brief String-literal overload — uses compile-time size so no strlen is
  /// needed and no embedded-null truncation can occur.  Preferred over any
  /// runtime (const char*) path for string literals.
  template<std::size_t N>
  explicit BufferSource(const char (&literal)[N]) : buffer(literal, N - 1)
  {
    checkNotEmpty(N <= 1);
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

  JSON_LIB_NODISCARD char current() const JSON_LIB_NOEXCEPT override
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

  JSON_LIB_NODISCARD bool more() const JSON_LIB_NOEXCEPT override { return bufferPosition < buffer.size(); }

  void reset() override
  {
    bufferPosition = 0;
    lineNo = 1;
    column = 1;
  }

  JSON_LIB_NODISCARD std::size_t position() const JSON_LIB_NOEXCEPT override { return bufferPosition; }

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
