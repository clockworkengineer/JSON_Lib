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
class IAction
{
public:
  // =============
  // IAction Error
  // =============
  struct Error final : std::runtime_error
  {
    explicit Error(const std::string_view &message) : std::runtime_error(makeTaggedError("IAction", message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IAction() = default;
  // ============================
  // Node encountered so process
  // ============================
  virtual void onNode(Node &) { }
  virtual void onNode(const Node &) { }
  // =============================
  // String encountered so process
  // =============================
  virtual void onString(Node &) { }
  virtual void onString(const Node &) { }
  // =============================
  // Number encountered so process
  // =============================
  virtual void onNumber(Node &) { }
  virtual void onNumber(const Node &) { }
  // ==============================
  // Boolean encountered so process
  // ==============================
  virtual void onBoolean(Node &) { }
  virtual void onBoolean(const Node &) { }
  // ===========================
  // Null encountered so process
  // ===========================
  virtual void onNull(Node &) { }
  virtual void onNull(const Node &) { }
  // ============================
  // Array encountered so process
  // ============================
  virtual void onArray(Node &) { }
  virtual void onArray(const Node &) { }
  // =============================
  // Object encountered so process
  // =============================
  virtual void onObject(Node &) { }
  virtual void onObject(const Node &) { }
};
}// namespace JSON_Lib