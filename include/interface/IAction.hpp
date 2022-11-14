#pragma once
// =======
// C++ STL
// =======
#include <stdexcept>
#include <string>
// ====
// JSON
// ====
#include "JSON_Core.hpp"
// =================
// LIBRARY NAMESPACE
// =================
namespace JSON_Lib {
// ==========================================================
// Interface for the action events during JSON tree traversal
// ==========================================================
class IAction
{
public:
  // =============
  // IAction Error
  // =============
  struct Error : public std::runtime_error
  {
    explicit Error(const std::string &message) : std::runtime_error("IAction Error: " + message) {}
  };
  // ========================
  // Constructors/destructors
  // ========================
  IAction() = default;
  IAction(const IAction &other) = delete;
  IAction &operator=(const IAction &other) = delete;
  IAction(IAction &&other) = delete;
  IAction &operator=(IAction &&other) = delete;
  virtual ~IAction() = default;
  // ============================
  // JNode encountered so process
  // ============================
  virtual void onJNode([[maybe_unused]] JNode &jNode) = 0;
  virtual void onJNode([[maybe_unused]] const JNode &jNode) = 0;
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] JNode &jNode) = 0;
  virtual void onString([[maybe_unused]] const JNode &jNode) = 0;
  // =============================
  // Number encountered so process
  // =============================
  virtual void onNumber([[maybe_unused]] JNode &jNode) = 0;
  virtual void onNumber([[maybe_unused]] const JNode &jNode) = 0;
  // ==============================
  // Boolean encountered so process
  // ==============================
  virtual void onBoolean([[maybe_unused]] JNode &jNode) = 0;
  virtual void onBoolean([[maybe_unused]] const JNode &jNode) = 0;
  // ===========================
  // Null encountered so process
  // ===========================
  virtual void onNull([[maybe_unused]] JNode &jNode) = 0;
  virtual void onNull([[maybe_unused]] const JNode &jNode) = 0;
  // ============================
  // Array encountered so process
  // ============================
  virtual void onArray([[maybe_unused]] JNode &jNode) = 0;
  virtual void onArray([[maybe_unused]] const JNode &jNode) = 0;
  // =============================
  // Object encountered so process
  // =============================
  virtual void onObject([[maybe_unused]] JNode &jNode) = 0;
  virtual void onObject([[maybe_unused]] const JNode &jNode) = 0;
};
}// namespace JSON_Lib