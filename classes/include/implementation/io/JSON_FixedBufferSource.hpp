#pragma once

#include <cstddef>
#include "ISource.hpp"
#include "JSON_Char_Constants.hpp"

namespace JSON_Lib {

// ============================================================
// Zero-heap source: borrows a fixed-length byte region.
// Suitable for ROM buffers and statically-allocated SRAM on
// embedded targets.  No ownership — the caller is responsible
// for the lifetime of `data`.
// ============================================================
class FixedBufferSource final : public ISource
{
public:
  FixedBufferSource(const char *data, std::size_t length) : data_(data), length_(length)
  {
    if (data == nullptr || length == 0) { throw Error("Empty source buffer passed to be parsed."); }
  }

  FixedBufferSource() = delete;
  FixedBufferSource(const FixedBufferSource &) = delete;
  FixedBufferSource &operator=(const FixedBufferSource &) = delete;
  FixedBufferSource(FixedBufferSource &&) = delete;
  FixedBufferSource &operator=(FixedBufferSource &&) = delete;
  ~FixedBufferSource() override = default;

  [[nodiscard]] char current() const override
  {
    if (more()) { return data_[position_]; }
    return static_cast<char>(EOF);
  }

  void next() override
  {
    if (!more()) { throw Error("Tried to read past end of buffer."); }
    ++position_;
    ++column;
    if (current() == kLineFeed) {
      ++lineNo;
      column = 1;
    }
  }

  [[nodiscard]] bool more() const override { return position_ < length_; }

  void reset() override
  {
    position_ = 0;
    lineNo = 1;
    column = 1;
  }

  [[nodiscard]] std::size_t position() const override { return position_; }

private:
  void backup(const unsigned long length) override { position_ -= length; }

  const char *data_;
  std::size_t length_;
  std::size_t position_{0};
};

}// namespace JSON_Lib
