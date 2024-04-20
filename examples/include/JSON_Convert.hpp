#pragma once

#include <set>
#include <sstream>

#include "JSON.hpp"
#include "JSON_Core.hpp"

class JSON_Convert : public JSON_Lib::IAction
{
public:

  JSON_Convert() = default;
  virtual ~JSON_Convert() = default;
  // Convert Number/Null/Boolean to String
  virtual void onNumber(JSON_Lib::JNode &jNode) override
  {
    jNode = JSON_Lib::JNode::make<JSON_Lib::String>(JRef<JSON_Lib::Number>(jNode).toString());
  }
  virtual void onBoolean(JSON_Lib::JNode &jNode) override
  {
    jNode = JSON_Lib::JNode::make<JSON_Lib::String>(JRef<JSON_Lib::Boolean>(jNode).toString());
  }
  virtual void onNull(JSON_Lib::JNode &jNode) override
  {
    jNode = JSON_Lib::JNode::make<JSON_Lib::String>(JRef<JSON_Lib::Null>(jNode).toString());
  }
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