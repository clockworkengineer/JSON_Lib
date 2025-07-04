#pragma once

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
    explicit Error(const std::string_view &message) : std::runtime_error(std::string("IAction Error: ").append(message)) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  virtual ~IAction() = default;
  // ============================
  // Node encountered so process
  // ============================
  virtual void onNode([[maybe_unused]] Node &jNode) { }
  virtual void onNode([[maybe_unused]] const Node &jNode) { }
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] Node &jNode) { }
  virtual void onString([[maybe_unused]] const Node &jNode) { }
  // =============================
  // Number encountered so process
  // =============================
  virtual void onNumber([[maybe_unused]] Node &jNode) { }
  virtual void onNumber([[maybe_unused]] const Node &jNode) { }
  // ==============================
  // Boolean encountered so process
  // ==============================
  virtual void onBoolean([[maybe_unused]] Node &jNode) { }
  virtual void onBoolean([[maybe_unused]] const Node &jNode) { }
  // ===========================
  // Null encountered so process
  // ===========================
  virtual void onNull([[maybe_unused]] Node &jNode) { }
  virtual void onNull([[maybe_unused]] const Node &jNode) { }
  // ============================
  // Array encountered so process
  // ============================
  virtual void onArray([[maybe_unused]] Node &jNode) { }
  virtual void onArray([[maybe_unused]] const Node &jNode) { }
  // =============================
  // Object encountered so process
  // =============================
  virtual void onObject([[maybe_unused]] Node &jNode) { }
  virtual void onObject([[maybe_unused]] const Node &jNode) { }
};
}// namespace JSON_Lib