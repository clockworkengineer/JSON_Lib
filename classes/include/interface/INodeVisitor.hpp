#pragma once

#include "JSON_ErrorBase.hpp"

namespace JSON_Lib {

struct Node;

/// @brief Fine-grained interface for generic node entry visitation.
class INodeVisitor
{
public:
  virtual ~INodeVisitor() = default;
  virtual void onNode(Node &) {}
  virtual void onNode(const Node &) {}
};

/// @brief Fine-grained interface for primitive value node visitation (String, Number, Boolean, Null).
class IValueVisitor
{
public:
  virtual ~IValueVisitor() = default;
  virtual void onString(Node &) {}
  virtual void onString(const Node &) {}
  virtual void onNumber(Node &) {}
  virtual void onNumber(const Node &) {}
  virtual void onBoolean(Node &) {}
  virtual void onBoolean(const Node &) {}
  virtual void onNull(Node &) {}
  virtual void onNull(const Node &) {}
};

/// @brief Fine-grained interface for container node visitation (Object and Array).
class IContainerVisitor
{
public:
  virtual ~IContainerVisitor() = default;
  virtual void onArray(Node &) {}
  virtual void onArray(const Node &) {}
  virtual void onObject(Node &) {}
  virtual void onObject(const Node &) {}
};

} // namespace JSON_Lib
