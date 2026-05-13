#pragma once

#include "JSON.hpp"

#include <string>
#include <vector>

namespace JSON_Lib::Tests {

class Mock_Action final : public IAction
{
public:
  std::vector<std::string> callOrder;

  std::size_t onNodeCalls{ 0 };
  std::size_t onStringCalls{ 0 };
  std::size_t onNumberCalls{ 0 };
  std::size_t onBooleanCalls{ 0 };
  std::size_t onNullCalls{ 0 };
  std::size_t onArrayCalls{ 0 };
  std::size_t onObjectCalls{ 0 };

  std::size_t onNodeConstCalls{ 0 };
  std::size_t onStringConstCalls{ 0 };
  std::size_t onNumberConstCalls{ 0 };
  std::size_t onBooleanConstCalls{ 0 };
  std::size_t onNullConstCalls{ 0 };
  std::size_t onArrayConstCalls{ 0 };
  std::size_t onObjectConstCalls{ 0 };

  void reset()
  {
    callOrder.clear();
    onNodeCalls = onStringCalls = onNumberCalls = onBooleanCalls = onNullCalls = onArrayCalls = onObjectCalls = 0;
    onNodeConstCalls = onStringConstCalls = onNumberConstCalls = onBooleanConstCalls = 0;
    onNullConstCalls = onArrayConstCalls = onObjectConstCalls = 0;
  }

  void onNode(Node &) override
  {
    ++onNodeCalls;
    callOrder.emplace_back("onNode");
  }

  void onString(Node &) override
  {
    ++onStringCalls;
    callOrder.emplace_back("onString");
  }

  void onNumber(Node &) override
  {
    ++onNumberCalls;
    callOrder.emplace_back("onNumber");
  }

  void onBoolean(Node &) override
  {
    ++onBooleanCalls;
    callOrder.emplace_back("onBoolean");
  }

  void onNull(Node &) override
  {
    ++onNullCalls;
    callOrder.emplace_back("onNull");
  }

  void onArray(Node &) override
  {
    ++onArrayCalls;
    callOrder.emplace_back("onArray");
  }

  void onObject(Node &) override
  {
    ++onObjectCalls;
    callOrder.emplace_back("onObject");
  }

  void onNode(const Node &) override
  {
    ++onNodeConstCalls;
    callOrder.emplace_back("onNodeConst");
  }

  void onString(const Node &) override
  {
    ++onStringConstCalls;
    callOrder.emplace_back("onStringConst");
  }

  void onNumber(const Node &) override
  {
    ++onNumberConstCalls;
    callOrder.emplace_back("onNumberConst");
  }

  void onBoolean(const Node &) override
  {
    ++onBooleanConstCalls;
    callOrder.emplace_back("onBooleanConst");
  }

  void onNull(const Node &) override
  {
    ++onNullConstCalls;
    callOrder.emplace_back("onNullConst");
  }

  void onArray(const Node &) override
  {
    ++onArrayConstCalls;
    callOrder.emplace_back("onArrayConst");
  }

  void onObject(const Node &) override
  {
    ++onObjectConstCalls;
    callOrder.emplace_back("onObjectConst");
  }
};

} // namespace JSON_Lib::Tests
