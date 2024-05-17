#pragma once

#include <fstream>
#include <string>

#include "IDestination.hpp"

namespace JSON_Lib {

class BufferDestination : public IDestination
{
public:
  BufferDestination() = default;
  BufferDestination(const BufferDestination &other) = delete;
  BufferDestination &operator=(const BufferDestination &other) = delete;
  BufferDestination(BufferDestination &&other) = delete;
  BufferDestination &operator=(BufferDestination &&other) = delete;
  virtual ~BufferDestination() = default;

  virtual void add(const std::string &bytes) override
  {
    // for (auto b : bytes) { buffer.push_back(b); }
    std::copy(bytes.begin(), bytes.end(), std::back_inserter(buffer));
  }
  virtual void add(const char ch) override { buffer.push_back(ch); }
  virtual void clear() override { buffer.clear(); }

  [[nodiscard]] const std::string toString() const { return (buffer); }
  [[nodiscard]] const std::size_t size() const { return (buffer.size()); }

private:
  std::string buffer;
};
}// namespace JSON_Lib
