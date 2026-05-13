#pragma once

#include "JSON_ErrorBase.hpp"

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
/// Implement this interface to inspect or mutate nodes as the traverser
/// visits each node in the JSON tree.  The non-const overloads allow
/// in-place tree modification; the const overloads are for read-only
/// inspection.  Default implementations are no-ops.
///
/// ## Traversal contract
///
/// Traversal is **pre-order depth-first**.  For every node the following
/// sequence is guaranteed:
///
///   1. `onNode(node)` — fires unconditionally for every node.
///   2. Type-specific callback — fires immediately after `onNode`:
///      - Leaf nodes  : `onNumber`, `onString`, `onBoolean`, or `onNull`.
///      - `Object`    : `onObject`, then step 1–2 recurse for each entry
///                      value in insertion order.
///      - `Array`     : `onArray`, then step 1–2 recurse for each element
///                      in index order.
///      - `Hole`      : no type-specific callback.
///
/// Example call sequence for `{ "x": [1, 2] }`:
/// @code
///   onNode(objectNode)
///   onObject(objectNode)
///     onNode(arrayNode)          // value of "x"
///     onArray(arrayNode)
///       onNode(numberNode_1)
///       onNumber(numberNode_1)
///       onNode(numberNode_2)
///       onNumber(numberNode_2)
/// @endcode
class IAction
{
public:
  /// @brief Exception type thrown by IAction implementations.
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("IAction", message)) {}
  };
  virtual ~IAction() = default;

  /// @brief Called for every node visited, before the type-specific callback.
  /// @param jNode The node currently being visited (mutable).
  virtual void onNode(Node &jNode) { }
  /// @brief Called for every node visited, before the type-specific callback.
  /// @param jNode The node currently being visited (read-only).
  virtual void onNode(const Node &jNode) { }

  /// @brief Called when a String node is visited.
  /// @param jNode The String node (mutable).
  virtual void onString(Node &jNode) { }
  /// @brief Called when a String node is visited.
  /// @param jNode The String node (read-only).
  virtual void onString(const Node &jNode) { }

  /// @brief Called when a Number node is visited.
  /// @param jNode The Number node (mutable).
  virtual void onNumber(Node &jNode) { }
  /// @brief Called when a Number node is visited.
  /// @param jNode The Number node (read-only).
  virtual void onNumber(const Node &jNode) { }

  /// @brief Called when a Boolean node is visited.
  /// @param jNode The Boolean node (mutable).
  virtual void onBoolean(Node &jNode) { }
  /// @brief Called when a Boolean node is visited.
  /// @param jNode The Boolean node (read-only).
  virtual void onBoolean(const Node &jNode) { }

  /// @brief Called when a Null node is visited.
  /// @param jNode The Null node (mutable).
  virtual void onNull(Node &jNode) { }
  /// @brief Called when a Null node is visited.
  /// @param jNode The Null node (read-only).
  virtual void onNull(const Node &jNode) { }

  /// @brief Called when an Array node is visited (before its children).
  /// @param jNode The Array node (mutable).
  virtual void onArray(Node &jNode) { }
  /// @brief Called when an Array node is visited (before its children).
  /// @param jNode The Array node (read-only).
  virtual void onArray(const Node &jNode) { }

  /// @brief Called when an Object node is visited (before its children).
  /// @param jNode The Object node (mutable).
  virtual void onObject(Node &jNode) { }
  /// @brief Called when an Object node is visited (before its children).
  /// @param jNode The Object node (read-only).
  virtual void onObject(const Node &jNode) { }
};
}// namespace JSON_Lib