#pragma once


#include "JSON.hpp"
#include "JSON_Core.hpp"
#include "JSON_Translator.hpp"

namespace JSON_Lib {

class JSON_Stringify final : public IStringify
{
public:
  JSON_Stringify() = default;
  JSON_Stringify(const JSON_Stringify &other) = delete;
  JSON_Stringify &operator=(const JSON_Stringify &other) = delete;
  JSON_Stringify(JSON_Stringify &&other) = delete;
  JSON_Stringify &operator=(JSON_Stringify &&other) = delete;
  ~JSON_Stringify() override = default;

  void stringify(const JNode &jNode, IDestination &destination, long indent) const override;

  // Set print ident value
  void setIndent(const long indent) override
  {
    if (indent < 0) { throw JSON_Lib::Error("Invalid print indentation value."); }
    printIndent = indent;
  }

  long getIndent() const override { return printIndent; }

private:

  JSON_Translator jsonTranslator;

  inline static long printIndent{ 4 };
};

}// namespace JSON_Lib
