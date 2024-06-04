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

private:
};