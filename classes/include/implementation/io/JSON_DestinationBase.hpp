#pragma once

#include "IDestination.hpp"

namespace JSON_Lib {

// ============================================================
// Non-virtual mixin that owns the lastChar_ field and provides
// the last() override required by IDestination.
// Concrete destinations inherit from this instead of
// IDestination directly and call trackLast() at the end of
// each add() overload.
// ============================================================
class DestinationBase : public IDestination
{
public:
  [[nodiscard]] char last() override { return lastChar_; }

protected:
  void trackLast(char ch) noexcept { lastChar_ = ch; }

private:
  char lastChar_{};
};

}// namespace JSON_Lib
