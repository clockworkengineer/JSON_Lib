#pragma once

#include "JSON_ErrorBase.hpp"
#include "INodeVisitor.hpp"

namespace JSON_Lib {

// ====================
// Forward declarations
// ====================
struct Node;

// ==========================================================
// Interface for the action events during JSON tree traversal
// ==========================================================
/// @brief Callback interface invoked during JSON tree traversal.
///
/// Inherits from fine-grained INodeVisitor, IValueVisitor, and IContainerVisitor.
class IAction : public INodeVisitor, public IValueVisitor, public IContainerVisitor
{
public:
  /// @brief Exception type thrown by IAction implementations.
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("IAction", message)) {}
  };
  ~IAction() override = default;

  /// @brief Called for every node visited, before the type-specific callback.
  /// @param jNode The node currently being visited (mutable).
  void onNode(Node &) override { }
  /// @brief Called for every node visited, before the type-specific callback.
  /// @param jNode The node currently being visited (read-only).
  void onNode(const Node &) override { }

  /// @brief Called when a String node is visited.
  /// @param jNode The String node (mutable).
  void onString(Node &) override { }
  /// @brief Called when a String node is visited.
  /// @param jNode The String node (read-only).
  void onString(const Node &) override { }

  /// @brief Called when a Number node is visited.
  /// @param jNode The Number node (mutable).
  void onNumber(Node &) override { }
  /// @brief Called when a Number node is visited.
  /// @param jNode The Number node (read-only).
  void onNumber(const Node &) override { }

  /// @brief Called when a Boolean node is visited.
  /// @param jNode The Boolean node (mutable).
  void onBoolean(Node &) override { }
  /// @brief Called when a Boolean node is visited.
  /// @param jNode The Boolean node (read-only).
  void onBoolean(const Node &) override { }

  /// @brief Called when a Null node is visited.
  /// @param jNode The Null node (mutable).
  void onNull(Node &) override { }
  /// @brief Called when a Null node is visited.
  /// @param jNode The Null node (read-only).
  void onNull(const Node &) override { }

  /// @brief Called when an Array node is visited (before its children).
  /// @param jNode The Array node (mutable).
  void onArray(Node &) override { }
  /// @brief Called when an Array node is visited (before its children).
  /// @param jNode The Array node (read-only).
  void onArray(const Node &) override { }

  /// @brief Called when an Object node is visited (before its children).
  /// @param jNode The Object node (mutable).
  void onObject(Node &) override { }
  /// @brief Called when an Object node is visited (before its children).
  /// @param jNode The Object node (read-only).
  void onObject(const Node &) override { }
};
} // namespace JSON_Lib