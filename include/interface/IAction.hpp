#pragma once
// =======
// C++ STL
// =======
#include <stdexcept>
#include <string>
// ====
// JSON
// ====
#include "JSON_Types.hpp"
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
  virtual ~IAction() = default;
  // ============================
  // JNode encountered so process
  // ============================
  virtual void onJNode([[maybe_unused]] JNode &jNode) {}
  virtual void onJNode([[maybe_unused]] const JNode &jNode) {}
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] JNode &jNodeString) {}
  virtual void onString([[maybe_unused]] const JNode &jNodeString) {}
  // =============================
  // Number encountered so process
  // =============================
  virtual void onNumber([[maybe_unused]] JNode &jNodeNumber) {}
  virtual void onNumber([[maybe_unused]] const JNode &jNodeNumber) {}
  // ==============================
  // Boolean encountered so process
  // ==============================
  virtual void onBoolean([[maybe_unused]] JNode &jNodeBoolean) {}
  virtual void onBoolean([[maybe_unused]] const JNode &jNodeBoolean) {}
  // ===========================
  // Null encountered so process
  // ===========================
  virtual void onNull([[maybe_unused]] JNode &jNodeNull) {}
  virtual void onNull([[maybe_unused]] const JNode &jNodeNull) {}
  // ============================
  // Array encountered so process
  // ============================
  virtual void onArray([[maybe_unused]] JNode &jNodeArray) {}
  virtual void onArray([[maybe_unused]] const JNode &jNodeArray) {}
  // =============================
  // Object encountered so process
  // =============================
  virtual void onObject([[maybe_unused]] JNode &jNodeObject) {}
  virtual void onObject([[maybe_unused]] const JNode &jNodeObject) {}
};
}// namespace JSON_Lib