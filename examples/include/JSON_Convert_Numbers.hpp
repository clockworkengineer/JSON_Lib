#pragma once
// =======
// C++ STL
// =======
#include <set>
#include <sstream>
// ====
// JSON
// ====
#include "JSON.hpp"
#include "JSON_Sources.hpp"
#include "JSON_Destinations.hpp"
#include "JSON_Types.hpp"
#include "IAction.hpp"
// ==================
// JSON Tree Analysis
// ==================
class JSON_Convert_Numbers : public JSON_Lib::IAction
{
public:
  // ========================
  // Constructors/destructors
  // ========================
  JSON_Convert_Numbers() = default;
  virtual ~JSON_Convert_Numbers() = default;
  virtual void onJNode([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onString([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onNumber([[maybe_unused]] JSON_Lib::JNode &jNode) override
  {
    std::string string = JRef<JSON_Lib::Number>(jNode).toString();
    jNode = JSON_Lib::String::make(string);
  }
  virtual void onBoolean([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onNull([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onArray([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onObject([[maybe_unused]] JSON_Lib::JNode &jNode) override {}

private:
};