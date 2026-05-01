#pragma once

#include <array>
#include <cstring>
#include <string>
#include <string_view>

#include "JSON_Config.hpp"
#include "JSON_DestinationBase.hpp"

namespace JSON_Lib {

template <std::size_t N>
class FixedBufferDestination final : public DestinationBase
{
public:
  FixedBufferDestination() = default;

  void add(const std::string &bytes) override
  {
    add(bytes.data(), bytes.size());
  }

  void add(const char *bytes) override
  {
    if (bytes == nullptr) { return; }
    add(bytes, std::strlen(bytes));
  }

  void add(const std::string_view &bytes) override
  {
    add(bytes.data(), bytes.size());
  }

  void add(const char ch) override
  {
#if JSON_LIB_NO_EXCEPTIONS
    if (size_ >= N) { overflowed_ = true; return; }
#else
    if (size_ >= N) { throw Error("Fixed buffer destination overflow."); }
#endif
    buffer_[size_++] = ch;
    trackLast(ch);
  }

  void clear() override
  {
    size_ = 0;
    overflowed_ = false;
    trackLast(0);
  }

  [[nodiscard]] const char *data() const noexcept { return buffer_.data(); }
  [[nodiscard]] std::size_t size() const noexcept { return size_; }
  [[nodiscard]] std::string toString() const { return std::string(buffer_.data(), size_); }
#if JSON_LIB_NO_EXCEPTIONS
  [[nodiscard]] bool overflowed() const noexcept { return overflowed_; }
  void clearOverflow() noexcept { overflowed_ = false; }
#endif

private:
  void add(const char *bytes, std::size_t length)
  {
    if (length == 0) { return; }
#if JSON_LIB_NO_EXCEPTIONS
    if (size_ + length > N) { overflowed_ = true; return; }
#else
    if (size_ + length > N) { throw Error("Fixed buffer destination overflow."); }
#endif
    std::memcpy(buffer_.data() + size_, bytes, length);
    trackLast(buffer_[size_ + length - 1]);
    size_ += length;
  }

  std::array<char, N> buffer_{};
  std::size_t size_{};
  bool overflowed_{false};
};

} // namespace JSON_Lib
