#pragma once

#include "IConverter.hpp"

namespace JSON_Lib {

class JSON_Converter : public IConverter
{
public:
  [[nodiscard]] std::u16string toUtf16(const std::string &utf8) const override;
  [[nodiscard]] std::string toUtf8(const std::u16string &utf16) const override;

private:
};
}// namespace JSON_Lib
