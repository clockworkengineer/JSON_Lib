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
    for (auto b : bytes) { m_buffer.push_back(b); }
  }
  virtual void add(const char ch) override { m_buffer.push_back(ch); }
  virtual void clear() override { m_buffer.clear(); }
  [[nodiscard]] const std::string &getBuffer() const { return (m_buffer); }

  std::string m_buffer;
};
}// namespace JSON_Lib
