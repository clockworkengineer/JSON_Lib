#pragma once

#include "JSON.hpp"

namespace JSON_Lib::Tests {

class Mock_Parser final : public IParser
{
public:
  explicit Mock_Parser(Node root = Node::make<Null>())
    : m_root(std::move(root))
  {
  }

  Node parse(ISource &) override
  {
    ++parseCalls;
    return m_root;
  }

  Result<Node> parseResult(ISource &) override
  {
    ++parseResultCalls;
    return { Status::Ok, std::make_unique<Node>(m_root), {}, { 0, 0 } };
  }

  void setNode(Node root) { m_root = std::move(root); }

  JSON_LIB_NODISCARD const Node &getNode() const noexcept { return m_root; }

  std::size_t parseCalls{ 0 };
  std::size_t parseResultCalls{ 0 };

private:
  Node m_root;
};

} // namespace JSON_Lib::Tests
