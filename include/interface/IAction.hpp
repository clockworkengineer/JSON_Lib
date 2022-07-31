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
// =========
// NAMESPACE
// =========
namespace JSONLib {
// ==========================================================
// Interface for the action events during JSON tree traversal
// ==========================================================
class IAction {
public:
  // =============
  // IAction Error
  // =============
  struct Error : public std::runtime_error {
    explicit Error(const std::string &message)
        : std::runtime_error("IAction Error: " + message) {}
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
  virtual void onString(const String &jNodeString) = 0;
  virtual void onNumber(const Number &jNodeNumber) = 0;
  virtual void onBoolean(const Boolean &jNodeBoolean) = 0;
  virtual void onNull(const Null &jNodeNull) = 0;
  virtual void onArray(const Array &jNodeArray) = 0;
  virtual void onObject(const Object &jNodeObject) = 0;
};
} // namespace JSONLib