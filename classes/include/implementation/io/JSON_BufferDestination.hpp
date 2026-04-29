#pragma once

#include <cstring>
#include <string>
#include <string_view>

namespace JSON_Lib {

class BufferDestination final : public IDestination
{
public:
  BufferDestination() = default;
  BufferDestination(const BufferDestination &other) = delete;
  BufferDestination &operator=(const BufferDestination &other) = delete;
  BufferDestination(BufferDestination &&other) = delete;
  BufferDestination &operator=(BufferDestination &&other) = delete;
  ~BufferDestination() override = default;

  void add(const std::string &bytes) override
  {
    if (!bytes.empty()) {
      buffer.append(bytes);
      lastChar = bytes.back();
    }
  }

  void add(const char *bytes) override
  {
    if (bytes == nullptr) { return; }
    const auto len = std::strlen(bytes);
    if (len == 0) { return; }
    buffer.append(bytes, len);
    lastChar = bytes[len - 1];
  }

  void add(const std::string_view &bytes) override
  {
    if (!bytes.empty()) {
      buffer.append(bytes);
      lastChar = bytes.back();
    }
  }

  void add(const char ch) override
  {
    buffer.push_back(ch);
    lastChar = ch;
  }

  void clear() override { buffer.clear(); }

  [[nodiscard]] std::string toString() const { return buffer; }
  [[nodiscard]] std::size_t size() const { return buffer.size(); }
  [[nodiscard]] char last() override { return buffer.back(); }

private:
  std::string buffer;
  char lastChar{};
};
}// namespace JSON_Lib
