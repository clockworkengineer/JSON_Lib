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
class JSON_Convert : public JSON_Lib::IAction
{
public:
  // ========================
  // Constructors/destructors
  // ========================
  JSON_Convert() = default;
  virtual ~JSON_Convert() = default;
  // Convert Number/Null/Boolean to String
  virtual void onJNode([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onString([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onNumber(JSON_Lib::JNode &jNode) override
  {
    jNode = JSON_Lib::String::make(JRef<JSON_Lib::Number>(jNode).toString());
  }
  virtual void onBoolean(JSON_Lib::JNode &jNode) override
  {
    jNode = JSON_Lib::String::make(JRef<JSON_Lib::Boolean>(jNode).toString());
  }
  virtual void onNull(JSON_Lib::JNode &jNode) override
  {
    jNode = JSON_Lib::String::make(JRef<JSON_Lib::Null>(jNode).toString());
  }
  virtual void onArray([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onObject([[maybe_unused]] JSON_Lib::JNode &jNode) override {}

private:
};