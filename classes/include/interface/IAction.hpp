#pragma once

namespace JSON_Lib {

// ====================
// Forward declarations
// ====================
struct JNode;

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
  // JNode encountered so process
  // ============================
  virtual void onJNode([[maybe_unused]] JNode &jNode) { }
  virtual void onJNode([[maybe_unused]] const JNode &jNode) { }
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] JNode &jNode) { }
  virtual void onString([[maybe_unused]] const JNode &jNode) { }
  // =============================
  // Number encountered so process
  // =============================
  virtual void onNumber([[maybe_unused]] JNode &jNode) { }
  virtual void onNumber([[maybe_unused]] const JNode &jNode) { }
  // ==============================
  // Boolean encountered so process
  // ==============================
  virtual void onBoolean([[maybe_unused]] JNode &jNode) { }
  virtual void onBoolean([[maybe_unused]] const JNode &jNode) { }
  // ===========================
  // Null encountered so process
  // ===========================
  virtual void onNull([[maybe_unused]] JNode &jNode) { }
  virtual void onNull([[maybe_unused]] const JNode &jNode) { }
  // ============================
  // Array encountered so process
  // ============================
  virtual void onArray([[maybe_unused]] JNode &jNode) { }
  virtual void onArray([[maybe_unused]] const JNode &jNode) { }
  // =============================
  // Object encountered so process
  // =============================
  virtual void onObject([[maybe_unused]] JNode &jNode) { }
  virtual void onObject([[maybe_unused]] const JNode &jNode) { }
};
}// namespace JSON_Lib