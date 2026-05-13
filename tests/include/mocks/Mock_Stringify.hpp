#pragma once

#include "JSON.hpp"

#include <string>

namespace JSON_Lib::Tests {

class Mock_Stringify final : public IStringify
{
public:
  explicit Mock_Stringify(std::string payload = "mock-stringify")
    : m_payload(std::move(payload))
  {
  }

  void stringify(const Node &, IDestination &destination, unsigned long indent) const override
  {
    ++stringifyCalls;
    lastIndent = indent;
    destination.add(m_payload);
  }

  void setIndent(const long indent) override { m_indent = indent; }

  JSON_LIB_NODISCARD long getIndent() const JSON_LIB_NOEXCEPT override { return m_indent; }

  void setPayload(std::string payload) { m_payload = std::move(payload); }

  mutable std::size_t stringifyCalls{ 0 };
  mutable unsigned long lastIndent{ 0 };

private:
  std::string m_payload;
  long m_indent{ 0 };
};

} // namespace JSON_Lib::Tests
