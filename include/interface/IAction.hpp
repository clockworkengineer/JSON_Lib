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
  IAction(const IAction &other) = delete;
  IAction &operator=(const IAction &other) = delete;
  IAction(IAction &&IAction) = delete;
  IAction &operator=(IAction &&other) = delete;
  virtual ~IAction() = default;
  // ============================
  // JNode encountered so process
  // ============================
  virtual void onJNode([[maybe_unused]] JNode &jNode) {}
  virtual void onJNode([[maybe_unused]] const JNode &jNode) {}
  // =============================
  // String encountered so process
  // =============================
  virtual void onString([[maybe_unused]] String &jNodeString) {}
  virtual void onString([[maybe_unused]] const String &jNodeString) {}
  // =============================
  // Number encountered so process
  // =============================
  virtual void onNumber([[maybe_unused]] Number &jNodeNumber) {}
  virtual void onNumber([[maybe_unused]] const Number &jNodeNumber) {}
  // ==============================
  // Boolean encountered so process
  // ==============================
  virtual void onBoolean([[maybe_unused]] Boolean &jNodeBoolean) {}
  virtual void onBoolean([[maybe_unused]] const Boolean &jNodeBoolean) {}
  // ===========================
  // Null encountered so process
  // ===========================
  virtual void onNull([[maybe_unused]] Null &jNodeNull) {}
  virtual void onNull([[maybe_unused]] const Null &jNodeNull) {}
  // ============================
  // Array encountered so process
  // ============================
  virtual void onArray([[maybe_unused]] Array &jNodeArray) {}
  virtual void onArray([[maybe_unused]] const Array &jNodeArray) {}
  // =============================
  // Object encountered so process
  // =============================
  virtual void onObject([[maybe_unused]] Object &jNodeObject) {}
  virtual void onObject([[maybe_unused]] const Object &jNodeObject) {}
};
}// namespace JSON_Lib