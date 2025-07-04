#pragma once

#include <set>
#include <sstream>

#include "JSON.hpp"
#include "JSON_Core.hpp"

class JSON_Convert : public JSON_Lib::IAction
{
public:

  JSON_Convert() = default;
  ~JSON_Convert() override = default;
  // Convert Number/Null/Boolean to String
  void onNumber(JSON_Lib::Node &jNode) override
  {
    jNode = JSON_Lib::Node::make<JSON_Lib::String>(JRef<JSON_Lib::Number>(jNode).toString());
  }
  void onBoolean(JSON_Lib::Node &jNode) override
  {
    jNode = JSON_Lib::Node::make<JSON_Lib::String>(JRef<JSON_Lib::Boolean>(jNode).toString());
  }
  void onNull(JSON_Lib::Node &jNode) override
  {
    jNode = JSON_Lib::Node::make<JSON_Lib::String>(JRef<JSON_Lib::Null>(jNode).toString());
  }

private:
};