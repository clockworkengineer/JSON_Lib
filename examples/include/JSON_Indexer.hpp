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
class JSON_Indexer : public JSON_Lib::IAction
{
public:
  // ========================
  // Constructors/destructors
  // ========================
  JSON_Indexer() = default;
  virtual ~JSON_Indexer() = default;
  virtual void onNumber([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onBoolean([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onNull([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onArray([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onObject([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onJNode([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  virtual void onString([[maybe_unused]] JSON_Lib::JNode &jNode) override {}
  // Const api not used
  virtual void onNumber([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onBoolean([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onNull([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onArray([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onObject([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onJNode([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}
  virtual void onString([[maybe_unused]] const JSON_Lib::JNode &jNode) override {}

private:
};