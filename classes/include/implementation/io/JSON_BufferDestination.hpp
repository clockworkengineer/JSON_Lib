#pragma once

#include <cstring>
#include <string>
#include <string_view>

#include "JSON_DestinationBase.hpp"

namespace JSON_Lib {

class BufferDestination final : public DestinationBase
{
public:
  BufferDestination() = default;

  void add(const std::string &bytes) override
  {
    if (!bytes.empty()) {
      buffer.append(bytes);
      trackLast(bytes.back());
    }
  }

  void add(const char *bytes) override
  {
    if (bytes == nullptr) { return; }
    const auto len = std::strlen(bytes);
    if (len == 0) { return; }
    buffer.append(bytes, len);
    trackLast(bytes[len - 1]);
  }

  void add(const std::string_view &bytes) override
  {
    if (!bytes.empty()) {
      buffer.append(bytes);
      trackLast(bytes.back());
    }
  }

  void add(const char ch) override
  {
    buffer.push_back(ch);
    trackLast(ch);
  }

  void clear() override { buffer.clear(); }

  [[nodiscard]] std::string toString() const { return buffer; }
  [[nodiscard]] std::size_t size() const { return buffer.size(); }

private:
  std::string buffer;
};
}// namespace JSON_Lib
