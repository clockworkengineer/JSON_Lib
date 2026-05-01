#pragma once

#include <memory>
#include "IStringify.hpp"
#include "ITranslator.hpp"

namespace JSON_Lib {

// ============================================================
// Non-abstract base for all stringify implementations.
// Owns the translator, declares all special members deleted /
// defaulted so concrete classes don't repeat the boilerplate.
// ============================================================
class StringifierBase : public IStringify
{
public:
  explicit StringifierBase(std::unique_ptr<ITranslator> translator)
    : translator_(std::move(translator))
  {
  }
  StringifierBase(const StringifierBase &) = delete;
  StringifierBase &operator=(const StringifierBase &) = delete;
  StringifierBase(StringifierBase &&) = delete;
  StringifierBase &operator=(StringifierBase &&) = delete;
  ~StringifierBase() override = default;

protected:
  std::unique_ptr<ITranslator> translator_;
};

}// namespace JSON_Lib
