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
  virtual void onJNode([[maybe_unused]] JNode &jNode) { throwDefaultHandlerError(); }
  virtual void onJNode([[maybe_unused]] const JNode &jNode) { throwDefaultHandlerError(); }
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] JNode &jNodeString) { throwDefaultHandlerError(); }
  virtual void onString([[maybe_unused]] const JNode &jNodeString) { throwDefaultHandlerError(); }
  // =============================
  // Number encountered so process
  // =============================
  virtual void onNumber([[maybe_unused]] JNode &jNodeNumber) { throwDefaultHandlerError(); }
  virtual void onNumber([[maybe_unused]] const JNode &jNodeNumber) { throwDefaultHandlerError(); }
  // ==============================
  // Boolean encountered so process
  // ==============================
  virtual void onBoolean([[maybe_unused]] JNode &jNodeBoolean) { throwDefaultHandlerError(); }
  virtual void onBoolean([[maybe_unused]] const JNode &jNodeBoolean) { throwDefaultHandlerError(); }
  // ===========================
  // Null encountered so process
  // ===========================
  virtual void onNull([[maybe_unused]] JNode &jNodeNull) { throwDefaultHandlerError(); }
  virtual void onNull([[maybe_unused]] const JNode &jNodeNull) { throwDefaultHandlerError(); }
  // ============================
  // Array encountered so process
  // ============================
  virtual void onArray([[maybe_unused]] JNode &jNodeArray) { throwDefaultHandlerError(); }
  virtual void onArray([[maybe_unused]] const JNode &jNodeArray) { throwDefaultHandlerError(); }
  // =============================
  // Object encountered so process
  // =============================
  virtual void onObject([[maybe_unused]] JNode &jNodeObject) { throwDefaultHandlerError(); }
  virtual void onObject([[maybe_unused]] const JNode &jNodeObject) { throwDefaultHandlerError(); }

private:
  void throwDefaultHandlerError() { throw Error("Trying to execute default action handler."); }
};
}// namespace JSON_Lib