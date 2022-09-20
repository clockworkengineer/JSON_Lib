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
//
// Note: Not strictly an interface in that it does not define a set of
// pure virtual functions that need to be defined but instead declares
// a set of functions with defaults that are overridden where appropriate;
// that throw an error if used. It may be possible to get away with one
// set of declarations using templates; unsure at the moment on this.
//
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
  virtual void onString([[maybe_unused]] JNode &jNode) { throwDefaultHandlerError(); }
  virtual void onString([[maybe_unused]] const JNode &jNode) { throwDefaultHandlerError(); }
  // =============================
  // Number encountered so process
  // =============================
  virtual void onNumber([[maybe_unused]] JNode &jNode) { throwDefaultHandlerError(); }
  virtual void onNumber([[maybe_unused]] const JNode &jNode) { throwDefaultHandlerError(); }
  // ==============================
  // Boolean encountered so process
  // ==============================
  virtual void onBoolean([[maybe_unused]] JNode &jNode) { throwDefaultHandlerError(); }
  virtual void onBoolean([[maybe_unused]] const JNode &jNode) { throwDefaultHandlerError(); }
  // ===========================
  // Null encountered so process
  // ===========================
  virtual void onNull([[maybe_unused]] JNode &jNode) { throwDefaultHandlerError(); }
  virtual void onNull([[maybe_unused]] const JNode &jNode) { throwDefaultHandlerError(); }
  // ============================
  // Array encountered so process
  // ============================
  virtual void onArray([[maybe_unused]] JNode &jNode) { throwDefaultHandlerError(); }
  virtual void onArray([[maybe_unused]] const JNode &jNode) { throwDefaultHandlerError(); }
  // =============================
  // Object encountered so process
  // =============================
  virtual void onObject([[maybe_unused]] JNode &jNode) { throwDefaultHandlerError(); }
  virtual void onObject([[maybe_unused]] const JNode &jNode) { throwDefaultHandlerError(); }

private:
  void throwDefaultHandlerError() { throw Error("Trying to execute default action handler."); }
};
}// namespace JSON_Lib